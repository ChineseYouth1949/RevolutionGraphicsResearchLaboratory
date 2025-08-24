#pragma once

#include "DXSample.h"
#include "HelloTriangle.h"

using namespace DirectX;

class HelloConstBuffers : public HelloTriangle {
 public:
  HelloConstBuffers(UINT width, UINT height, std::wstring name);

 protected:
  struct SceneConstantBuffer {
    XMFLOAT4 offset;
    float padding[60];
  };

  static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

  ComPtr<ID3D12Resource> m_constantBuffer;
  SceneConstantBuffer m_constaneBufferData;
  UINT8* m_pCbvDataBegin;

  void LoadPipeline() override;
  void PopulateCommandList() override;
};