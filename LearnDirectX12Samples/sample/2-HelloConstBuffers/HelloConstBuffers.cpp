#include "HelloConstBuffers.h"

HelloConstBuffers::HelloConstBuffers(UINT width, UINT height, std::wstring name) : HelloTriangle(width, height, name) {}

void HelloConstBuffers::LoadCoreInterface() {
  HelloTriangle::LoadCoreInterface();
}

void HelloConstBuffers::WaitForPreviousFrame() {
  HelloTriangle::WaitForPreviousFrame();
}

void HelloConstBuffers::LoadPipeline() {
  CreateRootSignature();

  HelloTriangle::CreatePSO();
  HelloTriangle::CreateVertexBuffer();

  CreateUniformBuffer();

  WaitForPreviousFrame();
}

void HelloConstBuffers::CreateRootSignature() {}
void HelloConstBuffers::CreateUniformBuffer() {}

void HelloConstBuffers::PopulateCommandList() {}