#pragma once

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "ConfigDefine.h"

namespace Engine::Math {

class PackedVector {
 public:
  INLINE PackedVector() {};
  INLINE explicit Scalar(float f) { m_vec = DirectX::XMVectorReplicate(f); }
  INLINE explicit PackedVector(float x, float y, float z) { m_vec = DirectX::XMVectorSet(x, y, z, z); }
  INLINE explicit PackedVector(float x, float y, float z, float w) { m_vec = DirectX::XMVectorSet(x, y, z, w); }

  INLINE PackedVector(const PackedVector& pv) { m_vec = pv.m_vec; }

 private:
  DirectX::XMVECTOR m_vec;
};

}  // namespace Engine::Math