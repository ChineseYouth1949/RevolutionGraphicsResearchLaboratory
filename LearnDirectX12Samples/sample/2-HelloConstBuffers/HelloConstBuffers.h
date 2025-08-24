#pragma once

#include "DXSample.h"
#include "HelloTriangle.h"

using namespace DirectX;

class HelloConstBuffers : public HelloTriangle {
 public:
  HelloConstBuffers(UINT width, UINT height, std::wstring name);

  // virtual void OnInit();
  // virtual void OnUpdate();
  // virtual void OnRender();
  // virtual void OnDestroy();

  //  protected:
  //   struct SceneConstantBuffer {
  //     XMFLOAT4 offset;
  //     float padding[60];
  //   };

  //   static_assert((sizeof(SceneConstantBuffer) % 256) == 0, "Constant Buffer size must be 256-byte aligned");
};