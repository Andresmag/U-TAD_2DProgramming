#include "vec2.h"
#include "rectCollider.h"

RectCollider::RectCollider(const Vec2& _vTopLeftCorner, const Vec2& _vSize)
	: m_vTopLeftCorner(_vTopLeftCorner)
	, m_vSize(_vSize)
{
}

bool RectCollider::collides(const Collider& other) const
{
	return other.collides(m_vTopLeftCorner, m_vSize);
}

bool RectCollider::collides(const Vec2& circlePos, float circleRadius) const
{
	return checkCircleRect(circlePos, circleRadius, m_vTopLeftCorner, m_vSize);
}

bool RectCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
	return checkRectRect(m_vTopLeftCorner, m_vSize, rectPos, rectSize);
}

bool RectCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
	return checkPixelsRect(pixelsPos, pixelsSize, pixels, m_vTopLeftCorner, m_vSize);
}