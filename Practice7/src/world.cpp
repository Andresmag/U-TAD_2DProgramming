#include "vec2.h"
#include "sprite.h"
#include "world.h"
#include "utils.h"
#include "rectCollider.h"

World::World(float clearRed, float clearGreen, float clearBlue, const ltex_t* back0, const ltex_t* back1,
	const ltex_t* back2, const ltex_t* back3)
	: m_fClearRed(clearRed)
	, m_fClearGreen(clearGreen)
	, m_fClearBlue(clearBlue)
{
	if (back0)
	    m_backgrounds.push_back(new Background(back0, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));
	if (back1)
        m_backgrounds.push_back(new Background(back1, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));
	if (back2)
        m_backgrounds.push_back(new Background(back2, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));
	if (back3)
        m_backgrounds.push_back(new Background(back3, 0.0f, Vec2(0.0f, 0.0f), Vec2(0.0f, 0.0f)));

	m_vCameraPosition = Vec2(0.0f, 0.0f);
	m_Map = nullptr;
}

World::~World()
{
	for (Sprite* tile : m_Map->m_vTiles)
	{
		delete tile;
	}
	m_Map->m_vTiles.clear();

	delete m_Map;
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
	/*Vec2 newCameraPos(clamp<float>(m_lSprites.front()->getPosition().GetXCoor() - screenSize.GetXCoor() / 2, 0.0f, static_cast<float>(m_backgrounds.back()->m_pTexture->width - screenSize.GetXCoor()))
		, clamp<float>(m_lSprites.front()->getPosition().GetYCoor() - screenSize.GetYCoor() / 2, 0.0f, static_cast<float>(m_backgrounds.back()->m_pTexture->height - screenSize.GetYCoor())));

    setCameraPosition(newCameraPos);*/

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
			float u1 = u0 + m_Map->m_vSize.GetXCoor() / static_cast<float>(back->m_pTexture->width);
			float v1 = v0 + m_Map->m_vSize.GetYCoor() / static_cast<float>(back->m_pTexture->height);

			// Dibujamos la textura
			lgfx_setblend(BLEND_ALPHA);
			ltex_drawrotsized(back->m_pTexture, pos.GetXCoor(), pos.GetYCoor(), 0.0f, 0.0f, 0.0f, m_Map->m_vSize.GetXCoor()
				, m_Map->m_vSize.GetYCoor(), u0, v0, u1, v1);
	    }
	}

	for (auto tile : m_Map->m_vTiles)
	{
		tile->draw();

		// Debug Collision
		/*lgfx_setcolor(0, 1, 0, 1);
		RectCollider* collider = (RectCollider*)tile->getCollider();
		lgfx_drawrect(collider->m_vTopLeftCorner.GetXCoor(), collider->m_vTopLeftCorner.GetYCoor(), collider->m_vSize.GetXCoor(), collider->m_vSize.GetYCoor());*/
	}
	
	for (auto element : m_lSprites)
	{
		// Debug collision
		/*RectCollider* collider = (RectCollider*)element->getCollider();
		lgfx_setcolor(1, 0, 0, 1);
		lgfx_drawrect(collider->m_vTopLeftCorner.GetXCoor(), collider->m_vTopLeftCorner.GetYCoor(), collider->m_vSize.GetXCoor(), collider->m_vSize.GetYCoor());
		lgfx_setcolor(1, 1, 1, 1);*/

		element->draw();
	}
}

bool World::loadMap(const char* filename, uint16_t firstColId)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (result)
	{
		m_Map = new Map();
		pugi::xml_node mapInfo = doc.child("map");
		m_Map->m_vSize.SetXCoor(mapInfo.attribute("width").as_float() * mapInfo.attribute("tilewidth").as_float());
		m_Map->m_vSize.SetYCoor(mapInfo.attribute("height").as_float() * mapInfo.attribute("tileheight").as_float());
		pugi::xml_node imageNode = mapInfo.child("tileset").child("image");
		std::string imageName = imageNode.attribute("source").as_string();
		std::string path = extractPath(filename) + imageName;
		const ltex_t* mapTexture = LoadTexture(path.c_str());
		int spriteColumns = mapInfo.child("tileset").attribute("columns").as_int();
		int spriteRows = mapInfo.child("tileset").attribute("tilecount").as_int() / spriteColumns;
		int firstGid = mapInfo.child("tileset").attribute("firstgid").as_int();
		int tileWidth = mapInfo.child("tileset").attribute("tilewidth").as_int();
		int tileHeight = mapInfo.child("tileset").attribute("tileheight").as_int();

		int tileNum = 0;
		for (pugi::xml_node tileNode = mapInfo.child("layer").child("data").child("tile"); tileNode; tileNode = tileNode.next_sibling("tile"))
		{
			int frame = tileNode.attribute("gid").as_int() - firstGid;

			if (frame >= 0)
			{
			    m_Map->m_vTiles.push_back(new Sprite(mapTexture, spriteColumns, spriteRows));
			    m_Map->m_vTiles.back()->setFps(spriteColumns * spriteRows);
			    m_Map->m_vTiles.back()->setCurrentFrame(frame);
			    m_Map->m_vTiles.back()->setPivot(Vec2(0, 0));
			       
			    Vec2 position;
			    position.SetXCoor((tileNum % static_cast<unsigned int>(m_Map->m_vSize.GetXCoor() / tileWidth)) * tileWidth);
			    position.SetYCoor(static_cast<int>(tileNum / (m_Map->m_vSize.GetXCoor() / tileHeight)) * tileHeight);
			    m_Map->m_vTiles.back()->setPosition(position);

			    m_Map->m_vTiles.back()->setCollisionType(Sprite::COLLISION_RECT);
			}

			tileNum++;
		}
	}
	else
	{
		printf("Error: %s\n", result.description());
		return false;
	}
}

Vec2 World::getMapSize() const
{
    if (m_Map)
	{
		return m_Map->m_vSize;
    }

	return Vec2(0, 0);;
}

bool World::moveSprite(Sprite& sprite, const Vec2& amount)
{
	bool collisionDetected = false;
	Vec2 previousPos = sprite.getPosition();

	// Movement on X axis
	sprite.setPosition(Vec2(previousPos.GetXCoor() + amount.GetXCoor(), previousPos.GetYCoor()));
	sprite.setCollisionType(sprite.getCollisionType());
	for (auto tile : m_Map->m_vTiles)
	{
		collisionDetected = sprite.collides(*tile);
		if (collisionDetected)
		{
			sprite.setPosition(previousPos);
			sprite.setCollisionType(sprite.getCollisionType());
			break;
		}
	}

	// Movement on Y axis
	previousPos = sprite.getPosition();
	sprite.setPosition(Vec2(previousPos.GetXCoor(), previousPos.GetYCoor() + amount.GetYCoor()));
	sprite.setCollisionType(sprite.getCollisionType());
	for (auto tile : m_Map->m_vTiles)
	{
		collisionDetected = sprite.collides(*tile);
		if (collisionDetected)
		{
			sprite.setPosition(previousPos);
			sprite.setCollisionType(sprite.getCollisionType());
			break;
		}
	}

	return collisionDetected;
}
