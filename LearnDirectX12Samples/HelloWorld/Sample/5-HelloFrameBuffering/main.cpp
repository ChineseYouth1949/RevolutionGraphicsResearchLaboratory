#include "HelloFrameBuffering.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  HelloFrameBuffering sample(1280, 720, L"Hello FrameBuffering");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}