#include "vec2.h"
#include "sprite.h"
#include "world.h"
#include "utils.h"

World::World(float clearRed, float clearGreen, float clearBlue, const ltex_t* back0, const ltex_t* back1,
	const ltex_t* back2, const ltex_t* back3)
	: m_fClearRed(clearRed)
	, m_fClearGreen(clearGreen)
	, m_fClearBlue(clearBlue)
{
	m_backgrounds.push_back(new Background(back0, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));
    m_backgrounds.push_back(new Background(back1, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));
    m_backgrounds.push_back(new Background(back2, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));
    m_backgrounds.push_back(new Background(back3, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));

	m_vCameraPosition = Vec2(0.0f, 0.0f);
}

float World::getClearRed() const
{
	return m_fClearRed;
}

float World::getClearGreen() const
{
	return m_fClearGreen;
}

float World::getClearBlue() const
{
	return m_fClearBlue;
}

const ltex_t* World::getBackground(size_t layer) const
{
	return m_backgrounds[layer]->m_pTexture;
}

float World::getScrollRatio(size_t layer) const
{
	return m_backgrounds[layer]->m_fScrollRatio;
}

void World::setScrollRatio(size_t layer, float ratio)
{
	m_backgrounds[layer]->m_fScrollRatio = ratio;
}

const Vec2& World::getScrollSpeed(size_t layer) const
{
	return m_backgrounds[layer]->m_vScrollSpeed;
}

void World::setScrollSpeed(size_t layer, const Vec2& speed)
{
	m_backgrounds[layer]->m_vScrollSpeed = speed;
}

const Vec2& World::getCameraPosition() const
{
	return m_vCameraPosition;
}

void World::setCameraPosition(const Vec2& pos)
{
	m_vCameraPosition = pos;
	lgfx_setorigin(m_vCameraPosition.GetXCoor(), m_vCameraPosition.GetYCoor());
}

void World::addSprite(Sprite& sprite)
{
	m_lSprites.push_back(&sprite);
}

void World::removeSprite(Sprite& sprite)
{	
	m_lSprites.remove(&sprite);
}

void World::update(float deltaTime)
{
	// Actualizamos la posicion de los backs con su velocidad automatica
	for (auto back : m_backgrounds)
	{
	    if (back->m_pTexture != nullptr)
	    {
			back->m_vPosition += back->m_vScrollSpeed * deltaTime;
	    }
	}

	// Actualizamos los sprites
	for (auto sprite : m_lSprites)
	{
		sprite->update(deltaTime);
	}

}

void World::draw(const Vec2& screenSize)
{
	// Reposicionar camara
	Vec2 newCameraPos(clamp<float>(m_lSprites.front()->getPosition().GetXCoor() - screenSize.GetXCoor() / 2, 0.0f, static_cast<float>(m_backgrounds.back()->m_pTexture->width - screenSize.GetXCoor()))
		, clamp<float>(m_lSprites.front()->getPosition().GetYCoor() - screenSize.GetYCoor() / 2, 0.0f, static_cast<float>(m_backgrounds.back()->m_pTexture->height - screenSize.GetYCoor())));

    setCameraPosition(newCameraPos);

	// Limpiar el backbuffer
	lgfx_clearcolorbuffer(m_fClearRed, m_fClearGreen, m_fClearBlue);

	for (auto back : m_backgrounds)
	{
	    if (back->m_pTexture != nullptr)
	    {
			// Calculamos el ratio de scroll con respecto del movimiento de la camara
			Vec2 pos(m_vCameraPosition.GetXCoor() * (1 - back->m_fScrollRatio), m_vCameraPosition.GetYCoor() * (1 - back->m_fScrollRatio));

			// Calculamos las UVs
			float u0 = -back->m_vPosition.GetXCoor() / static_cast<float>(back->m_pTexture->width);
			float v0 = -back->m_vPosition.GetYCoor() / static_cast<float>(back->m_pTexture->height);
			float u1 = u0 + static_cast<float>(m_backgrounds.back()->m_pTexture->width) / static_cast<float>(back->m_pTexture->width);
			float v1 = v0 + static_cast<float>(m_backgrounds.back()->m_pTexture->height) / static_cast<float>(back->m_pTexture->height);

			// Dibujamos la textura
			ltex_drawrotsized(back->m_pTexture, pos.GetXCoor(), pos.GetYCoor(), 0.0f, 0.0f, 0.0f, static_cast<float>(m_backgrounds.back()->m_pTexture->width)
				, static_cast<float>(m_backgrounds.back()->m_pTexture->height), u0, v0, u1, v1);
	    }
	}
	
	for (auto element : m_lSprites)
	{
		element->draw();
	}
}
