#include "HelloConstBuffers.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  HelloConstBuffers sample(1280, 720, L"Hello ConstBuffer");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}