#pragma once

#include "DXSampleHelper.h"
#include "Win32Application.h"

class DXSample {
 public:
  DXSample(UINT width, UINT height, std::wstring name);
  virtual ~DXSample();

  virtual void OnInit() = 0;
  virtual void OnUpdate() = 0;
  virtual void OnRender() = 0;
  virtual void OnDestroy() = 0;

  virtual void OnKeyDown(UINT8) {}
  virtual void OnKeyUp(UINT8) {}

  UINT GetWidth() const { return m_width; };
  UINT GetHeight() const { return m_height; };

  std::wstring GetTitle() const { return m_title; }

  void ParseCommandLineArgs(WCHAR* argv[], int argc);

 protected:
  std::wstring GetAssetFullPath(std::wstring assetName);
  void SetCustomWindowText(std::wstring text);
  void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);

  UINT m_width;
  UINT m_height;
  float m_aspectRatio;
  bool m_useWarpDevice;

 private:
  std::wstring m_assetsPath;
  std::wstring m_title;
};