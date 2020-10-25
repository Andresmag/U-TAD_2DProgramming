#pragma once

#include "collider.h"

class Vec2;

class CircleCollider : public Collider
{
public:
	CircleCollider(const Vec2& _vCenter, float _fRadius);

	virtual bool collides(const Collider& other) const override;
	virtual bool collides(const Vec2& circlePos, float circleRadius) const override;
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;

private:
	Vec2 m_vCenter;
	float m_fRadius;
};