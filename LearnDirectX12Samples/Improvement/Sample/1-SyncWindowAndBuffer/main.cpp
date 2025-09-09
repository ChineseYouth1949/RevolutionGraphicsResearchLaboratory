#include "SyncWindowAndBuffer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
  SyncWindowAndBuffer sample(800, 600, L"SyncWindowAndBuffer");
  return Win32Application::Run(&sample, hInstance, nCmdShow);
}