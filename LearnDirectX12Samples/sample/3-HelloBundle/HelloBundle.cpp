#include "HelloBundle.h"

HelloBundle::HelloBundle(UINT width, UINT height, std::wstring name) : HelloTriangle(width, height, name) {}

void HelloBundle::LoadPipeline() {
  HelloTriangle::LoadPipeline();

  ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_bundleAllocator)));

  ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_BUNDLE, m_bundleAllocator.Get(), m_pipelineState.Get(),
                                            IID_PPV_ARGS(&m_bundleCommandList)));
  m_bundleCommandList->SetGraphicsRootSignature(m_rootSignature.Get());
  m_bundleCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  m_bundleCommandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
  m_bundleCommandList->DrawInstanced(3, 1, 0, 0);
  ThrowIfFailed(m_bundleCommandList->Close());
}

void HelloBundle::PopulateCommandList() {
  ThrowIfFailed(m_commandAllocator->Reset());

  ThrowIfFailed(m_commandList->Reset(m_commandAllocator.Get(), m_pipelineState.Get()));

  m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
  m_commandList->RSSetViewports(1, &m_viewport);
  m_commandList->RSSetScissorRects(1, &m_scissorRect);

  m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT,
                                                                          D3D12_RESOURCE_STATE_RENDER_TARGET));

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
  m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

  const float clearColor[] = {0.0, 0.2f, 0.4f, 1.0f};
  m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

  m_commandList->ExecuteBundle(m_bundleCommandList.Get());

  m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                                          D3D12_RESOURCE_STATE_PRESENT));

  ThrowIfFailed(m_commandList->Close());
}