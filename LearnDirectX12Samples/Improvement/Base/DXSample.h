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

  virtual void OnKeyDown(UINT8 key);
  virtual void OnKeyUp(UINT8 key);

  virtual void OnMouseMove(UINT x, UINT y);
  virtual void OnLeftButtonDown(UINT x, UINT y);
  virtual void OnLeftButtonUp(UINT x, UINT y);
  virtual void OnRightButtonDown(UINT x, UINT y);
  virtual void OnRightButtonUp(UINT x, UINT y);

  virtual void OnDisplayChanged();
  virtual void OnWindowMoved(int x, int y);
  virtual void OnSizeChanged(UINT width, UINT height, bool minimized) = 0;

  virtual void ParseCommandLineArgs(WCHAR* argv[], int argc);

  UINT GetWidth() const;
  UINT GetHeight() const;
  std::wstring GetTitle() const;
  void SetWindowBounds(int left, int top, int right, int bottom);

 protected:
  std::wstring GetAssetFullPath(std::wstring assetName);
  void SetCustomWindowText(std::wstring text);
  void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter, bool requestHighPerformanceAdapter = false);

  RECT GetWindowBounds() const;
  bool GetTearingSupport() const { return m_tearingSupport; }
  void CheckTearingSupport();

  UINT m_width;
  UINT m_height;
  float m_aspectRatio;
  bool m_useWarpDevice;

  bool m_tearingSupport;

 private:
  std::wstring m_assetsPath;
  std::wstring m_title;

  RECT m_windowBounds;
};