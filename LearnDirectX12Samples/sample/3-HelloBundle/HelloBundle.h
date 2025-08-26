#pragma once

#include "HelloTriangle.h"

using namespace DirectX;

class HelloBundle : public HelloTriangle {
 public:
  HelloBundle(UINT width, UINT height, std::wstring name);

 protected:
  ComPtr<ID3D12CommandAllocator> m_bundleAllocator;
  ComPtr<ID3D12GraphicsCommandList> m_bundleCommandList;

  void LoadPipeline() override;
  void PopulateCommandList() override;
};