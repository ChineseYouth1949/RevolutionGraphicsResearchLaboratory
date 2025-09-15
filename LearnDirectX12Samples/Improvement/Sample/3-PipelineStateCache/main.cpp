#include "PipelineStateCache.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  PipelineStateCache sample(800, 600, L"PipelineStateCache");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}