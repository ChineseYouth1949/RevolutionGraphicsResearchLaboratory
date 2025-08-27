#include "HelloBundle.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  HelloBundle sample(1280, 720, L"Hello Bundle");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}