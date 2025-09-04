#include "Win32Application.h"

#include "DXSample.h"

HWND Win32Application::m_hwnd = nullptr;

int Win32Application::Run(DXSample* pSample, HINSTANCE hInstance, int nCmdShow) {
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