#pragma once

#include "DXSample.h"

using namespace DirectX;

class SupportResolutionOptions : public DXSample {
 public:
  SupportResolutionOptions(UINT width, UINT height, std::wstring name);

  void OnInit() override;
  void OnUpdate() override;
  void OnRender() override;
  void OnDestroy() override;

  void OnKeyDown(UINT8 key) override;
  void OnSizeChanged(UINT width, UINT height, bool minimized) override;

 private:
  static const UINT FrameCount = 2;

  struct Vertex {
    XMFLOAT3 position;
    XMFLOAT4 color;
  };

  CD3DX12_VIEWPORT m_viewport;
  CD3DX12_RECT m_scissorRect;
  ComPtr<IDXGISwapChain3> m_swapChain;
  ComPtr<ID3D12Device> m_device;
  ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
  ComPtr<ID3D12CommandQueue> m_commandQueue;
  ComPtr<ID3D12CommandAllocator> m_commandAllocator[FrameCount];
  UINT64 m_fenceValues[FrameCount];
  ComPtr<ID3D12RootSignature> m_rootSignature;
  ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
  ComPtr<ID3D12PipelineState> m_pipelineState;
  ComPtr<ID3D12GraphicsCommandList> m_commandList;
  UINT m_rtvDescriptorSize;

  ComPtr<ID3D12Resource> m_vertexBuffer;
  D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;

  UINT m_frameIndex;
  HANDLE m_fenceEvent;
  ComPtr<ID3D12Fence> m_fence;

  void LoadCoreInterface();
  void LoadPipeline();

  void PopulateCommandList();
  void WaitForGpu();
  void MoveToNextFrame();

  void CreateRootSignature();
  void CreatePSO();
  void CreateVertexBuffer();

  void ResizeSwapChainToWindow(UINT width, UINT height);
};