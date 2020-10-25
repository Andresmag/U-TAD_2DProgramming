#pragma once

#ifndef LITE
#define LITE
#include "litegfx.h"
#endif
#include <vector>
#include <list>

class Vec2;
class Sprite;

struct Background
{
	const ltex_t* m_pTexture;
	float m_fScrollRatio;
	Vec2 m_vScrollSpeed;
	Vec2 m_vPosition;

	Background(const ltex_t* back, float ratio, Vec2 speed, Vec2 pos)
		: m_pTexture(back)
	    , m_fScrollRatio(ratio)
	    , m_vScrollSpeed(speed)
	    , m_vPosition(pos)
	{}
};

class World
{
private:
	float m_fClearRed, m_fClearGreen, m_fClearBlue;
	std::vector<Background*> m_backgrounds;
	Vec2 m_vCameraPosition;
	std::list<Sprite*> m_lSprites;
	
public:
	World(float clearRed = 0.15f, float clearGreen = 0.15f, float clearBlue = 0.15f, 
		const ltex_t* back0 = nullptr, const ltex_t* back1 = nullptr, 
		const ltex_t* back2 = nullptr, const ltex_t* back3 = nullptr);

	float getClearRed() const;
	float getClearGreen() const;
	float getClearBlue() const;

	const ltex_t* getBackground(size_t layer) const;

	float getScrollRatio(size_t layer) const;
	void setScrollRatio(size_t layer, float ratio);

	const Vec2& getScrollSpeed(size_t layer) const;
	void setScrollSpeed(size_t layer, const Vec2& speed);

	const Vec2& getCameraPosition() const;
	void setCameraPosition(const Vec2& pos);

	void addSprite(Sprite& sprite);
	void removeSprite(Sprite& sprite);
	
	void update(float deltaTime);
	void draw(const Vec2& screenSize);
};
