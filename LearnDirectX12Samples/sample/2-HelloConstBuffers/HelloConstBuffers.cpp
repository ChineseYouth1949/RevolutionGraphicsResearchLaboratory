#include "HelloConstBuffers.h"

HelloConstBuffers::HelloConstBuffers(UINT width, UINT height, std::wstring name)
    : HelloTriangle(width, height, name), m_pCbvDataBegin(nullptr), m_constantBufferData{} {}

void HelloConstBuffers::OnUpdate() {
  const float translationSpeed = 0.005f;
  const float offsetBounds = 1.25f;

  m_constantBufferData.offset.x += translationSpeed;
  if (m_constantBufferData.offset.x > offsetBounds) {
    m_constantBufferData.offset.x = -offsetBounds;
  }

  memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
}

void HelloConstBuffers::LoadPipeline() {
  CreateRootSignature();

  HelloTriangle::CreatePSO();
  HelloTriangle::CreateVertexBuffer();

  CreateConstantBufferView();
}

void HelloConstBuffers::CreateRootSignature() {
  D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

  featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

  if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData)))) {
    featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
  }

  CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
  CD3DX12_ROOT_PARAMETER1 rootParameters[1];

  ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);
  rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_VERTEX);

  D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
      D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
      D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS |
      D3D12_ROOT_SIGNATURE_FLAG_DENY_PIXEL_SHADER_ROOT_ACCESS;

  CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
  rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

  ComPtr<ID3DBlob> signature;
  ComPtr<ID3DBlob> error;
  ThrowIfFailed(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error));
  ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}

void HelloConstBuffers::CreateConstantBufferView() {
  D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc = {};
  cbvHeapDesc.NumDescriptors = 1;
  cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
  ThrowIfFailed(m_device->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&m_cbvHeap)));

  const UINT constantBufferSize = sizeof(SceneConstantBuffer);

  ThrowIfFailed(m_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
                                                  &CD3DX12_RESOURCE_DESC::Buffer(constantBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                  IID_PPV_ARGS(&m_constantBuffer)));

  D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
  cbvDesc.BufferLocation = m_constantBuffer->GetGPUVirtualAddress();
  cbvDesc.SizeInBytes = constantBufferSize;
  m_device->CreateConstantBufferView(&cbvDesc, m_cbvHeap->GetCPUDescriptorHandleForHeapStart());

  CD3DX12_RANGE readRange(0, 0);
  ThrowIfFailed(m_constantBuffer->Map(0, &readRange, reinterpret_cast<void**>(&m_pCbvDataBegin)));
  memcpy(m_pCbvDataBegin, &m_constantBufferData, sizeof(m_constantBufferData));
}

void HelloConstBuffers::PopulateCommandList() {
  ThrowIfFailed(m_commandAllocator->Reset());

  ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

  m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

  ID3D12DescriptorHeap* ppHeaps[] = {m_cbvHeap.Get()};
  m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

  m_commandList->SetGraphicsRootDescriptorTable(0, m_cbvHeap->GetGPUDescriptorHandleForHeapStart());
  m_commandList->RSSetViewports(1, &m_viewport);
  m_commandList->RSSetScissorRects(1, &m_scissorRect);

  m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT,
                                                                          D3D12_RESOURCE_STATE_RENDER_TARGET));

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
  m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

  const float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
  m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
  m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
  m_commandList->DrawInstanced(3, 1, 0, 0);

  m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                                          D3D12_RESOURCE_STATE_PRESENT));

  ThrowIfFailed(m_commandList->Close());
}