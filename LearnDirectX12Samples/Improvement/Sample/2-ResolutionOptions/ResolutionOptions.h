#pragma once

#include "DXSample.h"

#include <vector>

using namespace DirectX;

class ResolutionOptions : public DXSample {
 public:
  ResolutionOptions(UINT width, UINT height, std::wstring name);

  void OnInit() override;
  void OnUpdate() override;
  void OnRender() override;
  void OnDestroy() override;

  void OnKeyDown(UINT8 key) override;
  void OnSizeChanged(UINT width, UINT height, bool minimized) override;

 private:
  static const UINT FrameCount = 2;
  static const float QuadWidth;
  static const float QuadHeight;
  static const float LetterboxColor[4];
  static const float ClearColor[4];

  struct SceneVertex {
    XMFLOAT3 position;
    XMFLOAT4 color;
  };

  struct PostVertex {
    XMFLOAT4 position;
    XMFLOAT2 uv;
  };

  struct SceneConstantBuffer {
    XMFLOAT4X4 transform;
    XMFLOAT4 offset;
    UINT padding[44];
  };

  struct Resolution {
    UINT width;
    UINT height;
  };

  std::vector<Resolution> m_resolutionOptions;
  UINT m_resolutionIndex;

  ComPtr<IDXGISwapChain3> m_swapChain;
  ComPtr<ID3D12Device> m_device;
  ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
  ComPtr<ID3D12CommandQueue> m_commandQueue;
  ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
  ComPtr<ID3D12DescriptorHeap> m_cbvSrvHeap;
  UINT m_rtvDescriptorSize;
  UINT m_cbvSrvDescriptorSize;

  ComPtr<ID3D12Resource> m_intermediateRenderTarget;

  CD3DX12_VIEWPORT m_sceneViewport;
  CD3DX12_RECT m_sceneScissorRect;
  ComPtr<ID3D12CommandAllocator> m_sceneCommandAllocators[FrameCount];
  ComPtr<ID3D12RootSignature> m_sceneRootSignature;
  ComPtr<ID3D12PipelineState> m_scenePipelineState;
  ComPtr<ID3D12GraphicsCommandList> m_sceneCommandList;
  ComPtr<ID3D12Resource> m_sceneVertexBuffer;
  D3D12_VERTEX_BUFFER_VIEW m_sceneVertexBufferView;
  ComPtr<ID3D12Resource> m_sceneConstantBuffer;
  SceneConstantBuffer m_sceneConstantBufferData;
  UINT8* m_pCbvDataBegin;

  CD3DX12_VIEWPORT m_postViewport;
  CD3DX12_RECT m_postScissorRect;
  ComPtr<ID3D12CommandAllocator> m_postCommandAllocators[FrameCount];
  ComPtr<ID3D12RootSignature> m_postRootSignature;
  ComPtr<ID3D12PipelineState> m_postPipelineState;
  ComPtr<ID3D12GraphicsCommandList> m_postCommandList;
  ComPtr<ID3D12Resource> m_postVertexBuffer;
  D3D12_VERTEX_BUFFER_VIEW m_postVertexBufferView;

  UINT m_frameIndex;
  HANDLE m_fenceEvent;
  ComPtr<ID3D12Fence> m_fence;
  UINT64 m_fenceValues[FrameCount];

  bool m_windowVisible;

  void LoadCoreInterface();
  void LoadPipeline();

  void LoadSizeDependentResources();
  void LoadSceneResolutionDependentResources();

  void UpdatePostViewAndScissor();
  void UpdateTitle();

  void PopulateCommandLists();
  void MoveToNextFrame();
  void WaitForGpu();

  void RestoreD3DResources();
  void ReleaseD3DResources();
};