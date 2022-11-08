#pragma once

#include <intrin.h>


namespace Math
{
    struct FOG_API DirectX::XMMATRIX;

    INLINE DirectX::XMVECTOR SplatZero() { return DirectX::XMVectorZero(); }

#if !defined(_XM_NO_INTRINSICS_) && defined(_XM_SSE_INTRINSICS_)

    INLINE DirectX::XMVECTOR SplatOne(DirectX::XMVECTOR zero = SplatZero())
    {
        __m128i allBits = _mm_castps_si128(_mm_cmpeq_ps(zero, zero));
        return _mm_castsi128_ps(_mm_slli_epi32(_mm_srli_epi32(allBits, 25), 23));
    }

#if defined(_XM_SSE4_INTRINSICS_)
    INLINE DirectX::XMVECTOR CreateXUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        return _mm_insert_ps(one, one, 0x0E);
    }
    INLINE DirectX::XMVECTOR CreateYUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        return _mm_insert_ps(one, one, 0x0D);
    }
    INLINE DirectX::XMVECTOR CreateZUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        return _mm_insert_ps(one, one, 0x0B);
    }
    INLINE DirectX::XMVECTOR CreateWUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        return _mm_insert_ps(one, one, 0x07);
    }
    INLINE DirectX::XMVECTOR SetWToZero(DirectX::FXMVECTOR vec)
    {
        return _mm_insert_ps(vec, vec, 0x08);
    }
    INLINE DirectX::XMVECTOR SetWToOne(DirectX::FXMVECTOR vec)
    {
        return _mm_blend_ps(vec, SplatOne(), 0x8);
    }
#else
    INLINE DirectX::XMVECTOR CreateXUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        return _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(one), 12));
    }
    INLINE DirectX::XMVECTOR CreateYUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        DirectX::XMVECTOR unitx = CreateXUnitVector(one);
        return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(unitx), 4));
    }
    INLINE DirectX::XMVECTOR CreateZUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        DirectX::XMVECTOR unitx = CreateXUnitVector(one);
        return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(unitx), 8));
    }
    INLINE DirectX::XMVECTOR CreateWUnitVector(DirectX::XMVECTOR one = SplatOne())
    {
        return _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(one), 12));
    }
    INLINE DirectX::XMVECTOR SetWToZero(DirectX::FXMVECTOR vec)
    {
        __m128i MaskOffW = _mm_srli_si128(_mm_castps_si128(_mm_cmpeq_ps(vec, vec)), 4);
        return _mm_and_ps(vec, _mm_castsi128_ps(MaskOffW));
    }
    INLINE DirectX::XMVECTOR SetWToOne(DirectX::FXMVECTOR vec)
    {
        return _mm_movelh_ps(vec, _mm_unpackhi_ps(vec, SplatOne()));
    }
#endif

#else // !_XM_SSE_INTRINSICS_

    INLINE DirectX::XMVECTOR SplatOne() { return DirectX::XMVectorSplatOne(); }
    INLINE DirectX::XMVECTOR CreateXUnitVector() { return DirectX::g_XMIdentityR0; }
    INLINE DirectX::XMVECTOR CreateYUnitVector() { return DirectX::g_XMIdentityR1; }
    INLINE DirectX::XMVECTOR CreateZUnitVector() { return DirectX::g_XMIdentityR2; }
    INLINE DirectX::XMVECTOR CreateWUnitVector() { return DirectX::g_XMIdentityR3; }
    INLINE DirectX::XMVECTOR SetWToZero(FXMVECTOR vec) { return DirectX::XMVectorAndInt(vec, DirectX::g_XMMask3); }
    INLINE DirectX::XMVECTOR SetWToOne(FXMVECTOR vec) { return DirectX::XMVectorSelect(DirectX::g_XMIdentityR3, vec, DirectX::g_XMMask3); }

#endif
}