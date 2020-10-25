#include "vec2.h"
#include "pixelsCollider.h"

PixelsCollider::PixelsCollider(const Vec2& _vTopLeftCorner, const Vec2& _vSize, uint8_t* _pPixels)
	: m_vTopLeftCorner(_vTopLeftCorner)
	, m_vSize(_vSize)
	, m_pPixels(_pPixels)
{
}

bool PixelsCollider::collides(const Collider& other) const
{
	return other.collides(m_vTopLeftCorner, m_vSize, m_pPixels);
}

bool PixelsCollider::collides(const Vec2& circlePos, float circleRadius) const
{
	return checkCirclePixels(circlePos, circleRadius, m_vTopLeftCorner, m_vSize, m_pPixels);
}

bool PixelsCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
	return checkPixelsRect(m_vTopLeftCorner, m_vSize, m_pPixels, rectPos, rectSize);
}

bool PixelsCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
	return checkPixelsPixels(m_vTopLeftCorner, m_vSize, m_pPixels, pixelsPos, pixelsSize, pixels);
}