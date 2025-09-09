#include "ResolutionOptions.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  ResolutionOptions sample(800, 600, L"ResolutionOptions");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}