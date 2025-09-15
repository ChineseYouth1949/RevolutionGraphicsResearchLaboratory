#include "DynamicIndexing.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  DynamicIndexing sample(800, 600, L"DynamicIndexing");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}