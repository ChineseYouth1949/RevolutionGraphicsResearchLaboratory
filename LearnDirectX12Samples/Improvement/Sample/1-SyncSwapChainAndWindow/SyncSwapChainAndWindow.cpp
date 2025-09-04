#include "SyncSwapChainAndWindow.h"

SyncSwapChainAndWindow::SyncSwapChainAndWindow(UINT width, UINT height, std::wstring name)
    : DXSample(width, height, name),
      m_frameIndex(0),
      m_viewport(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height)),
      m_scissorRect(0, 0, static_cast<LONG>(width), static_cast<LONG>(height)),
      m_fenceValues{},
      m_rtvDescriptorSize(0) {}

void SyncSwapChainAndWindow::OnInit() {
  SetCustomWindowText(L"Stretch to enlarge the window and press Q to adjust the swap chain size to the window");

  LoadCoreInterface();
  LoadPipeline();
}

void SyncSwapChainAndWindow::OnUpdate() {}
void SyncSwapChainAndWindow::OnRender() {
  PopulateCommandList();

  ID3D12CommandList* ppCommandList[] = {m_commandList.Get()};
  m_commandQueue->ExecuteCommandLists(_countof(ppCommandList), ppCommandList);

  ThrowIfFailed(m_swapChain->Present(1, 0));

  MoveToNextFrame();
}

void SyncSwapChainAndWindow::OnDestroy() {
  WaitForGpu();
  CloseHandle(m_fenceEvent);
}

void SyncSwapChainAndWindow::LoadCoreInterface() {
  UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
  {
    ComPtr<ID3D12Debug> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
      debugController->EnableDebugLayer();

      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
  }
#endif

  ComPtr<IDXGIFactory4> factory;
  ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

  if (m_useWarpDevice) {
    ComPtr<IDXGIAdapter> warpAdapter;
    ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

    ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
  } else {
    ComPtr<IDXGIAdapter1> hardwareAdapter;
    GetHardwareAdapter(factory.Get(), &hardwareAdapter, true);

    ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));
  }

  D3D12_COMMAND_QUEUE_DESC queueDesc = {};
  queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
  queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

  ThrowIfFailed(m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));

  DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
  swapChainDesc.BufferCount = FrameCount;
  swapChainDesc.Width = m_width;
  swapChainDesc.Height = m_height;
  swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  swapChainDesc.SampleDesc.Count = 1;
  swapChainDesc.Scaling = DXGI_SCALING_NONE;

  ComPtr<IDXGISwapChain1> swapChain;
  ThrowIfFailed(factory->CreateSwapChainForHwnd(m_commandQueue.Get(), Win32Application::GetHwnd(), &swapChainDesc, nullptr, nullptr, &swapChain));

  ThrowIfFailed(swapChain.As(&m_swapChain));
  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

  {
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap)));

    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
  }

  {
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

    for (UINT n = 0; n < FrameCount; n++) {
      ThrowIfFailed(m_swapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
      m_device->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
      rtvHandle.Offset(1, m_rtvDescriptorSize);
    }
  }

  for (size_t i = 0; i < FrameCount; i++) {
    ThrowIfFailed(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator[i])));
  }

  {
    ThrowIfFailed(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator[0].Get(), nullptr, IID_PPV_ARGS(&m_commandList)));
    ThrowIfFailed(m_commandList->Close());
  }

  {
    ThrowIfFailed(m_device->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    m_fenceValues[m_frameIndex]++;

    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr) {
      ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
  }
}

void SyncSwapChainAndWindow::LoadPipeline() {
  CreateRootSignature();
  CreatePSO();
  CreateVertexBuffer();

  WaitForGpu();
}

void SyncSwapChainAndWindow::CreateRootSignature() {
  CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
  rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

  ComPtr<ID3DBlob> signature;
  ComPtr<ID3DBlob> error;
  ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));
  ThrowIfFailed(m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature)));
}

void SyncSwapChainAndWindow::CreatePSO() {
  ComPtr<ID3DBlob> vertexShader;
  ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
  UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
  UINT compileFlags = 0;
#endif

  std::wstring vertexShaderPath = GetAssetFullPath(L"shaders.hlsl");
  ThrowIfFailed(D3DCompileFromFile(vertexShaderPath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));

  std::wstring pixelShaderPath = GetAssetFullPath(L"shaders.hlsl");
  ThrowIfFailed(D3DCompileFromFile(pixelShaderPath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));

  D3D12_INPUT_ELEMENT_DESC inputElementDescs[] = {{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
                                                  {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}};

  D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
  psoDesc.InputLayout = {inputElementDescs, _countof(inputElementDescs)};
  psoDesc.pRootSignature = m_rootSignature.Get();
  psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
  psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
  psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
  psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
  psoDesc.DepthStencilState.DepthEnable = FALSE;
  psoDesc.DepthStencilState.StencilEnable = FALSE;
  psoDesc.SampleMask = UINT_MAX;
  psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
  psoDesc.NumRenderTargets = 1;
  psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
  psoDesc.SampleDesc.Count = 1;
  ThrowIfFailed(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
}

void SyncSwapChainAndWindow::CreateVertexBuffer() {
  Vertex triangleVertices[] = {{{0.0f, 0.25f * m_aspectRatio, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
                               {{0.25f, -0.25f * m_aspectRatio, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
                               {{-0.25f, -0.25f * m_aspectRatio, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}};

  const UINT vertexBufferSize = sizeof(triangleVertices);

  ThrowIfFailed(m_device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
                                                  &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
                                                  IID_PPV_ARGS(&m_vertexBuffer)));

  UINT8* pVertexDataBegin;
  CD3DX12_RANGE readRange(0, 0);
  ThrowIfFailed(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
  memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
  m_vertexBuffer->Unmap(0, nullptr);

  m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
  m_vertexBufferView.StrideInBytes = sizeof(Vertex);
  m_vertexBufferView.SizeInBytes = vertexBufferSize;
}

void SyncSwapChainAndWindow::PopulateCommandList() {
  ThrowIfFailed(m_commandAllocator[m_frameIndex]->Reset());

  ThrowIfFailed(m_commandList->Reset(m_commandAllocator[m_frameIndex].Get(), m_pipelineState.Get()));

  m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
  m_commandList->RSSetViewports(1, &m_viewport);
  m_commandList->RSSetScissorRects(1, &m_scissorRect);

  m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT,
                                                                          D3D12_RESOURCE_STATE_RENDER_TARGET));

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
  m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

  const float clearColor[] = {0.0, 0.2f, 0.4f, 1.0f};
  m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
  m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  m_commandList->IASetVertexBuffers(0, 1, &m_vertexBufferView);
  m_commandList->DrawInstanced(3, 1, 0, 0);

  m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                                          D3D12_RESOURCE_STATE_PRESENT));

  ThrowIfFailed(m_commandList->Close());
}

void SyncSwapChainAndWindow::WaitForGpu() {
  ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]));

  ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
  WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);

  m_fenceValues[m_frameIndex]++;
}

void SyncSwapChainAndWindow::MoveToNextFrame() {
  const UINT64 currentFenceValue = m_fenceValues[m_frameIndex];
  ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), currentFenceValue));

  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

  if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex]) {
    ThrowIfFailed(m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_fenceEvent));
    WaitForSingleObjectEx(m_fenceEvent, INFINITE, FALSE);
  }

  m_fenceValues[m_frameIndex] = currentFenceValue + 1;
}

void SyncSwapChainAndWindow::OnSizeChanged(UINT width, UINT height, bool minimized) {
  if (!minimized) {
    m_width = width;
    m_height = height;
  }
}

void SyncSwapChainAndWindow::OnKeyDown(UINT8 key) {
  // Q event
  if (key == 0x51) {
    ResizeSwapChainToWindow(m_width, m_height);
  }
}

void SyncSwapChainAndWindow::ResizeSwapChainToWindow(UINT width, UINT height) {
  WaitForGpu();

  for (size_t i = 0; i < FrameCount; i++) {
    m_renderTargets[i].Reset();
    m_fenceValues[i] = m_fenceValues[m_frameIndex];
  }

  DXGI_SWAP_CHAIN_DESC desc = {};
  m_swapChain->GetDesc(&desc);
  ThrowIfFailed(m_swapChain->ResizeBuffers(FrameCount, width, height, desc.BufferDesc.Format, desc.Flags));

  m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

  m_width = width;
  m_height = height;
  m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);

  // m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
  m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));

  CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
  for (size_t i = 0; i < FrameCount; i++) {
    ThrowIfFailed(m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i])));
    m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
    rtvHandle.Offset(1, m_rtvDescriptorSize);
  }
}