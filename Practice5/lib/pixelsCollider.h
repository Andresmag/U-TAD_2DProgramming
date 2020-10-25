#pragma once

#include "collider.h"

class Vec2;

class PixelsCollider : public Collider
{
public:
	PixelsCollider(const Vec2& _vTopLeftCorner, const Vec2& _vSize, uint8_t* _pPixels);

	virtual bool collides(const Collider& other) const override;
	virtual bool collides(const Vec2& circlePos, float circleRadius) const override;
	virtual bool collides(const Vec2& rectPos, const Vec2& rectSize) const override;
	virtual bool collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const override;

private:
	Vec2 m_vTopLeftCorner;
	Vec2 m_vSize;
	uint8_t* m_pPixels;
};