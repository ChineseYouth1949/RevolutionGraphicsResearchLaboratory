#include "SupportResolutionOptions.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  SupportResolutionOptions sample(800, 600, L"SupportResolutionOptions");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}