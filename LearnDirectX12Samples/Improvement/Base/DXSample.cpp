#include "DXSample.h"

#include "stdafx.h"

using namespace Microsoft::WRL;

DXSample::DXSample(UINT width, UINT height, std::wstring name) : m_width(width), m_height(height), m_title(name), m_useWarpDevice(false) {
  m_assetsPath = GetAppPath();
  m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

DXSample::~DXSample() {}

void DXSample::OnKeyDown(UINT8 key) {}
void DXSample::OnKeyUp(UINT8 key) {}
void DXSample::OnMouseMove(UINT x, UINT y) {}
void DXSample::OnLeftButtonDown(UINT x, UINT y) {}
void DXSample::OnLeftButtonUp(UINT x, UINT y) {}
void DXSample::OnRightButtonDown(UINT x, UINT y) {}
void DXSample::OnRightButtonUp(UINT x, UINT y) {}
void DXSample::OnDisplayChanged() {}
void DXSample::OnWindowMoved(int x, int y) {}

UINT DXSample::GetWidth() const {
  return m_width;
}
UINT DXSample::GetHeight() const {
  return m_height;
}
std::wstring DXSample::GetTitle() const {
  return m_title;
}
RECT DXSample::GetWindowBounds() const {
  return m_windowBounds;
}

void DXSample::SetWindowBounds(int left, int top, int right, int bottom) {
  m_windowBounds.left = left;
  m_windowBounds.top = top;
  m_windowBounds.right = right;
  m_windowBounds.bottom = bottom;
}

std::wstring DXSample::GetAssetFullPath(std::wstring assetName) {
  return m_assetsPath + assetName;
}
void DXSample::SetCustomWindowText(std::wstring text) {
  std::wstring windowText = m_title + L": " + text;
  SetWindowText(Win32Application::GetHwnd(), windowText.c_str());
}
void DXSample::GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter) {
  *ppAdapter = nullptr;

  ComPtr<IDXGIAdapter1> adapter;

  ComPtr<IDXGIFactory6> factory6;
  if (SUCCEEDED(pFactory->QueryInterface(IID_PPV_ARGS(&factory6)))) {
    for (UINT adapterIndex = 0; SUCCEEDED(factory6->EnumAdapterByGpuPreference(
             adapterIndex, requestHighPerformanceAdapter == true ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
             IID_PPV_ARGS(&adapter)));
         ++adapterIndex) {
      DXGI_ADAPTER_DESC1 desc;
      adapter->GetDesc1(&desc);

      if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
        // Don't select the Basic Render Driver adapter.
        // If you want a software adapter, pass in "/warp" on the command line.
        continue;
      }

      // Check to see whether the adapter supports Direct3D 12, but don't create the
      // actual device yet.
      if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
        break;
      }
    }
  }

  if (adapter.Get() == nullptr) {
    for (UINT adapterIndex = 0; SUCCEEDED(pFactory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex) {
      DXGI_ADAPTER_DESC1 desc;
      adapter->GetDesc1(&desc);

      if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
        // Don't select the Basic Render Driver adapter.
        // If you want a software adapter, pass in "/warp" on the command line.
        continue;
      }

      // Check to see whether the adapter supports Direct3D 12, but don't create the
      // actual device yet.
      if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
        break;
      }
    }
  }

  *ppAdapter = adapter.Detach();
}

void DXSample::ParseCommandLineArgs(WCHAR* argv[], int argc) {
  for (int i = 1; i < argc; ++i) {
    if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 || _wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0) {
      m_useWarpDevice = true;
      m_title = m_title + L" (WARP)";
    }
  }
}