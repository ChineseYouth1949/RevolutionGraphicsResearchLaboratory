#include "SyncSwapChainAndWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  SyncSwapChainAndWindow sample(800, 600, L"SyncSwapChainAndWindow");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}