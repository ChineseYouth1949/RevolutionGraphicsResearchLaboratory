#include "HelloTriangle.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  HelloTriangle sample(1280, 720, L"Hello Triangle");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}