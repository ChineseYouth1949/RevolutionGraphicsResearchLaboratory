#pragma once

#include "stdafx.h"

class DXSample;

enum struct WindowMode : uint8_t { Normal = 0, Borderless, FullScreen, Num };

class Win32Application {
 public:
  static int Run(DXSample* pSample, HINSTANCE hInstance, int nCmdShow);
  static HWND GetHwnd() { return m_hwnd; }

  static WindowMode GetWindowMode() { return m_windowMode; }
  static void SetWindowMode(WindowMode windowMode, IDXGISwapChain* pSwapChain);

 protected:
  static LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

 private:
  static HWND m_hwnd;

  static WindowMode m_windowMode;
  static const UINT m_windowStyle = WS_OVERLAPPEDWINDOW;
  static RECT m_windowRect;
};