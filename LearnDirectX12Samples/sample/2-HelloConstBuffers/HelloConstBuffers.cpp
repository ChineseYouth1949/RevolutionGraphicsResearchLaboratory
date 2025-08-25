#include "HelloConstBuffers.h"

HelloConstBuffers::HelloConstBuffers(UINT width, UINT height, std::wstring name) : HelloTriangle(width, height, name) {}

// void HelloConstBuffers::LoadPipeline() {
//   //   {
//   //     D3D12_FEATURE_ROOT_SIGNATURE featureData = {};

//   //     featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

//   //     if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE))) {
//   //       featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
//   //     }

//   //     CD3DX12_DESCRIPTOR_RANGE1 ranges[1];
//   //   }
// }

// void HelloConstBuffers::PopulateCommandList() {}