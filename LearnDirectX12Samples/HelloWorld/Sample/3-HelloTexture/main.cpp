#include "HelloTexture.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  HelloTexture sample(1280, 720, L"Hello Texture");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}