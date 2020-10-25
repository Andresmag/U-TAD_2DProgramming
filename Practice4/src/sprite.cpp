#include "vec2.h"
#include "sprite.h"

// Num frames que tiene la imagen del sprite
Sprite::Sprite(const ltex_t* _tex, int _iHFrames, int _iVFrames)
{
	setTexture(_tex, _iHFrames, _iVFrames);
	setAngle(0.0f);
	setScale(Vec2(1.f, 1.f));
	setBlend(BLEND_ALPHA);
	setCurrentFrame(0);
	setColor(1, 1, 1, 1);
	setPivot(Vec2(0.5f, 0.5f));
	setCallback(nullptr);
}

Sprite::~Sprite()
{
	if (m_texture != nullptr)
	{
		delete m_texture;
	}
}

// Puntero a la funcion callback
void Sprite::setCallback(CallbackFunc _func)
{
	m_func = _func;
}

// Puntero generico a datos (datos del sprite que va a utilizar la función callback)
void* Sprite::getUserData()
{
	return m_userData;
}

void Sprite::setUserData(void* _Data)
{
	m_userData = _Data;
}

const ltex_t* Sprite::getTexture() const
{
	return m_texture;
}

void Sprite::setTexture(const ltex_t* _tex, int _iHFrames, int _iVFrames)
{
	m_texture = _tex;
	m_iHFrames = _iHFrames;
	m_iVFrames = _iVFrames;
}

lblend_t Sprite::getBlend() const
{
	return m_blendMode;
}

void Sprite::setBlend(lblend_t _bMode)
{
	m_blendMode = _bMode;
}

float Sprite::getRed() const
{
	return m_fRed;
}

float Sprite::getGreen() const
{
	return m_fGreen;
}

float Sprite::getBlue() const
{
	return m_fBlue;
}

float Sprite::getAlpha() const
{
	return m_fAlpha;
}

void Sprite::setColor(float _fR, float _fG, float _fB, float _fA)
{
	m_fRed = _fR;
	m_fGreen = _fG;
	m_fBlue = _fB;
	m_fAlpha = _fA;
}

const Vec2& Sprite::getPosition() const
{
	return m_vPosition;
}

void Sprite::setPosition(const Vec2& _vPos)
{
	m_vPosition = _vPos;
}

float Sprite::getAngle() const
{
	return m_fAngle;
}

void Sprite::setAngle(float _fAngle)
{
	m_fAngle = _fAngle;
}

const Vec2& Sprite::getScale() const
{
	return m_vScale;
}

void Sprite::setScale(const Vec2& _vScale)
{
	m_vScale = _vScale;
}

// Tamaño de un frame multiplicado por la escala
Vec2 Sprite::getSize() const
{
	Vec2 vFrameSize(m_texture->width / m_iHFrames, m_texture->height/m_iVFrames);
	return (m_vScale * vFrameSize);
}

// Valor para indicar el pivote de rotaxion en el pintado
const Vec2& Sprite::getPivot() const
{
	return m_vPivot;
}

void Sprite::setPivot(const Vec2& _vPivot)
{
	m_vPivot = _vPivot;
}

int Sprite::getHframes() const
{
	return m_iHFrames;
}

int Sprite::getVframes() const
{
	return m_iVFrames;
}

// Veces por segundo que se cambia el frame de animacion
int Sprite::getFps() const
{
	return m_iFPS;
}

void Sprite::setFps(int _iFPS)
{
	m_iFPS = _iFPS;
}

// Frame actual de animacion
int Sprite::getCurrentFrame() const
{
	return m_iFrame;
}

void Sprite::setCurrentFrame(int _iFrame)
{
	m_iFrame = _iFrame;
}

void Sprite::update(float _fDeltaTime, const Vec2& _vMousePos)
{
	if (m_func != nullptr)
	{
		m_func(*this, _fDeltaTime);
	}
}

void Sprite::draw() const
{
	Vec2 frameSize = getSize();
	float u0 = static_cast<float>((m_iFrame % m_iHFrames)) / m_iHFrames;
	float v0 = static_cast<float>((m_iFrame / m_iHFrames)) / m_iVFrames;
	float u1 = static_cast<float>(((m_iFrame % m_iHFrames) + 1)) / m_iHFrames;
	float v1 = static_cast<float>(((m_iFrame / m_iHFrames) + 1)) / m_iVFrames;
	lgfx_setblend(m_blendMode);
	lgfx_setcolor(m_fRed, m_fGreen, m_fBlue, m_fAlpha);
	ltex_drawrotsized(m_texture, m_vPosition.GetXCoor(), m_vPosition.GetYCoor(), m_fAngle, m_vPivot.GetXCoor(), m_vPivot.GetYCoor(), frameSize.GetXCoor(), frameSize.GetYCoor(), u0, v0, u1, v1);
	lgfx_setcolor(1, 1, 1, 1);
}