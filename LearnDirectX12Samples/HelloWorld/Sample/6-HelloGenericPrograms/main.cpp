#include "HelloGenericPrograms.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  HelloGenericPrograms sample(1280, 720, L"Hello Generic Programs");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}