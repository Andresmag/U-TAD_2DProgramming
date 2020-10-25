#include "vec2.h"
#include "circleCollider.h"

CircleCollider::CircleCollider(const Vec2& _vCenter, float _fRadius)
	: m_vCenter(_vCenter)
	, m_fRadius(_fRadius)
{
}

bool CircleCollider::collides(const Collider& other) const
{
	return other.collides(m_vCenter, m_fRadius);
}

bool CircleCollider::collides(const Vec2& circlePos, float circleRadius) const
{
	return checkCircleCircle(m_vCenter, m_fRadius, circlePos, circleRadius);
}

bool CircleCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const
{
	return checkCircleRect(m_vCenter, m_fRadius, rectPos, rectSize);
}

bool CircleCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
	return checkCirclePixels(m_vCenter, m_fRadius, pixelsPos, pixelsSize, pixels);
}