#include "vec2.h"
#include "text.h"
#include "font.h"
#include <cstring>

// Constructors
Text::Text()
	: m_sText(nullptr)
	, m_pFont(nullptr)
	, m_uSpeed(0)
{
	m_pColor[0] = 0;
	m_pColor[1] = 0;
	m_pColor[2] = 0;
}

Text::Text(const char* _sText, const Font* _pFont, const Vec2& _vPos, const unsigned int& _uSpeed, const float* _pColor)
{
	m_sText = new char[strlen(_sText) + 1]; // +1 para el \0 y evitar heap corruption al eliminar
	strcpy(const_cast<char*>(m_sText), _sText);
	m_pFont = _pFont;
	m_vPos = _vPos;
	m_uSpeed = _uSpeed;
	m_pColor[0] = _pColor[0];
	m_pColor[1] = _pColor[1];
	m_pColor[2] = _pColor[2];
}

// Destructor
Text::~Text()
{
	if (m_sText != nullptr)
	{
		delete[] m_sText;
	}
}

// Getters
const char* Text::getText() 
{ 
	return m_sText; 
}

const Font* Text::getFont() 
{
	return m_pFont;
}

const Vec2 Text::getPos() 
{
	return m_vPos;
}

float* Text::getColor() 
{
	return m_pColor; 
}

// Update position
void Text::UpdatePosition(float _fSeconds)
{
	m_vPos.SetXCoor(m_vPos.GetXCoor() - m_uSpeed * _fSeconds);
}

// Draw text
void Text::Draw()
{
	m_pFont->Draw(m_sText, m_vPos);
}