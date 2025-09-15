#pragma once

#include <DirectXMath.h>
#include <DirectXMathVector.inl>

#include "ConfigDefine.h"

using namespace DirectX;

namespace MiniEngine::Math {

class Vector3;

using Point3 = Vector3;

class Vector3 {
 public:
  INLINE constexpr Vector3() noexcept : m_vec(XMFLOAT3(0, 0, 0)) {}
  INLINE constexpr Vector3(float x, float y, float z) noexcept : m_vec(XMFLOAT3(x, y, z));

  INLINE Vector3(const Point3& start, const Point3& end) noexcept : Vector3(end - start) {};
  INLINE Vector3(const Point3& start, float length) noexcept : Vector3(start, start + length) {}

  INLINE explicit Vector3(const XMFLOAT3& vec) noexcept { m_vec = vec; }
  INLINE explicit Vector3(FXMVECTOR vect) noexcept { XMStoreFloat3(&m_vec, vect); }

  INLINE Vector3(const Vector3& v) noexcept { m_vec = v.m_vec };

  INLINE XMVECTOR GetXMVector() const {
    XMVECTOR result;
    XMStoreFloat3(m_vec, result);
    return result;
  }

  INLINE Vector3 operator-() const { return XMVectorNegate(GetXMVector()); }
  INLINE Vector3 operator+(const Vector3& v2) const { return XMVectorAdd(GetXMVector(), v2); }

  INLINE Vector3 operator-(const Vector3& v2) const { ; }

  // INLINE operator XMVECTOR() const {
  //   XMVECTOR result;
  //   return XMLoadFloat3(&result);
  // }

  // INLINE operator XMFLOAT3() const { return m_vec; }

 private:
  XMFLOAT3 m_vec;
};

}  // namespace MiniEngine::Math