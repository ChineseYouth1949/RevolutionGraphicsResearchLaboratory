#pragma once

#include "DXSample.h"

using namespace DirectX;

class HelloConstBuffers : public DXSample {
 public:
  HelloConstBuffers(UINT width, UINT height, std::wstring name);

  virtual void OnInit();
  virtual void OnUpdate();
  virtual void OnRender();
  virtual void OnDestroy();

 private:
  static const UINT FrameCount = 2;

  struct Vertex {
    XMFLOAT3 position;
    XMFLOAT4 color;
  };

  struct SceneConstantBuffer {
    XMFLOAT4 offset;
    float padding[60];
  };

  static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");
};