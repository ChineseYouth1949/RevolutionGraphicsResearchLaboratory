#include "Win32Application.h"

#include "DXSample.h"

HWND Win32Application::m_hwnd = nullptr;

RECT Win32Application::m_windowRect;
WindowMode Win32Application::m_windowMode = WindowMode::Normal;

int Win32Application::Run(DXSample* pSample, HINSTANCE hInstance, int nCmdShow) {
  try {
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    pSample->ParseCommandLineArgs(argv, argc);
    LocalFree(argv);

    WNDCLASSEX windowClass = {0};
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = hInstance;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.lpszClassName = L"DXSampleClass";
    RegisterClassEx(&windowClass);

    RECT windowRect = {0, 0, static_cast<LONG>(pSample->GetWidth()), static_cast<LONG>(pSample->GetHeight())};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindow(windowClass.lpszClassName, pSample->GetTitle().c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                          windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, hInstance, pSample);

    pSample->OnInit();

    ShowWindow(m_hwnd, nCmdShow);

    MSG msg = {};
    while (msg.message != WM_QUIT) {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }

    pSample->OnDestroy();

    return static_cast<char>(msg.wParam);
  } catch (std::exception& e) {
    OutputDebugString(L"Application hit a problem: ");
    OutputDebugStringA(e.what());
    OutputDebugString(L"\nTerminating.\n");

    pSample->OnDestroy();
    return EXIT_FAILURE;
  }
}

void Win32Application::SetWindowMode(WindowMode windowMode, IDXGISwapChain* pSwapChain) {
  if (m_windowMode != windowMode) {
    if (m_windowMode != WindowMode::FullScreen) {
      // Save the old window rect so we can restore it when exiting fullscreen mode.
      GetWindowRect(m_hwnd, &m_windowRect);
    }

    if (windowMode == WindowMode::FullScreen) {
      // Make the window borderless so that the client area can fill the screen.
      SetWindowLong(m_hwnd, GWL_STYLE, m_windowStyle & ~(WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME));

      RECT fullscreenWindowRect;
      try {
        if (pSwapChain) {
          // Get the settings of the display on which the app's window is currently displayed
          ComPtr<IDXGIOutput> pOutput;
          ThrowIfFailed(pSwapChain->GetContainingOutput(&pOutput));
          DXGI_OUTPUT_DESC Desc;
          ThrowIfFailed(pOutput->GetDesc(&Desc));
          fullscreenWindowRect = Desc.DesktopCoordinates;
        } else {
          // Fallback to EnumDisplaySettings implementation
          throw HrException(S_FALSE);
        }
      } catch (HrException& e) {
        UNREFERENCED_PARAMETER(e);

        // Get the settings of the primary display
        DEVMODE devMode = {};
        devMode.dmSize = sizeof(DEVMODE);
        EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);

        fullscreenWindowRect = {devMode.dmPosition.x, devMode.dmPosition.y, devMode.dmPosition.x + static_cast<LONG>(devMode.dmPelsWidth),
                                devMode.dmPosition.y + static_cast<LONG>(devMode.dmPelsHeight)};
      }

      SetWindowPos(m_hwnd, HWND_TOPMOST, fullscreenWindowRect.left, fullscreenWindowRect.top, fullscreenWindowRect.right, fullscreenWindowRect.bottom,
                   SWP_FRAMECHANGED | SWP_NOACTIVATE);

      ShowWindow(m_hwnd, SW_MAXIMIZE);

      pSwapChain->SetFullscreenState(true, nullptr);

    } else if (windowMode == WindowMode::Borderless) {
      SetWindowLong(m_hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
      SetWindowPos(m_hwnd, HWND_NOTOPMOST, m_windowRect.left, m_windowRect.top, m_windowRect.right - m_windowRect.left,
                   m_windowRect.bottom - m_windowRect.top, SWP_FRAMECHANGED | SWP_NOACTIVATE);
      ShowWindow(m_hwnd, SW_NORMAL);
    } else if (windowMode == WindowMode::Normal) {
      SetWindowLong(m_hwnd, GWL_STYLE, m_windowStyle);
      SetWindowPos(m_hwnd, HWND_NOTOPMOST, m_windowRect.left, m_windowRect.top, m_windowRect.right - m_windowRect.left,
                   m_windowRect.bottom - m_windowRect.top, SWP_FRAMECHANGED | SWP_NOACTIVATE);
      ShowWindow(m_hwnd, SW_NORMAL);
    }

    m_windowMode = windowMode;
  }
}

LRESULT CALLBACK Win32Application::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  DXSample* pSample = reinterpret_cast<DXSample*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  switch (message) {
    case WM_CREATE: {
      LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
      SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }
      return 0;

    case WM_PAINT:
      if (pSample) {
        pSample->OnUpdate();
        pSample->OnRender();
      }
      return 0;

    case WM_SIZE:
      if (pSample) {
        RECT windowRect = {};
        GetWindowRect(hwnd, &windowRect);
        pSample->SetWindowBounds(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);

        RECT clientRect = {};
        GetClientRect(hwnd, &clientRect);
        pSample->OnSizeChanged(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top, wParam == SIZE_MINIMIZED);
      }
      return 0;

    case WM_MOVE:
      if (pSample) {
        RECT windowRect = {};
        GetWindowRect(hwnd, &windowRect);
        pSample->SetWindowBounds(windowRect.left, windowRect.top, windowRect.right, windowRect.bottom);

        int xPos = (int)(short)LOWORD(lParam);
        int yPos = (int)(short)HIWORD(lParam);
        pSample->OnWindowMoved(xPos, yPos);
      }
      return 0;

    case WM_DISPLAYCHANGE:
      if (pSample) {
        pSample->OnDisplayChanged();
      }
      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;

    case WM_KEYDOWN:
      if (pSample) {
        pSample->OnKeyDown(static_cast<UINT8>(wParam));
      }
      return 0;

    case WM_KEYUP:
      if (pSample) {
        pSample->OnKeyUp(static_cast<UINT8>(wParam));
      }
      return 0;

    case WM_MOUSEMOVE:
      if (pSample) {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        pSample->OnMouseMove(x, y);
      }
      return 0;

    case WM_LBUTTONDOWN:
      if (pSample) {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        pSample->OnLeftButtonDown(x, y);
      }
      return 0;

    case WM_LBUTTONUP:
      if (pSample) {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        pSample->OnLeftButtonUp(x, y);
      }
      return 0;

    case WM_RBUTTONDOWN:
      if (pSample) {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        pSample->OnRightButtonDown(x, y);
      }
      return 0;

    case WM_RBUTTONUP:
      if (pSample) {
        UINT x = LOWORD(lParam);
        UINT y = HIWORD(lParam);
        pSample->OnRightButtonUp(x, y);
      }
      return 0;
  }

  return DefWindowProc(hwnd, message, wParam, lParam);
}