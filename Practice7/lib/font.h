#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "stb_truetype.h"
#include <litegfx.h>
#define LITE

class Vec2;

class Font 
{
private:
	float m_fHeight;
	ltex_t* m_pTex;
	stbtt_bakedchar* m_pCharData;

	Font(float _fHeight, ltex_t* texture, stbtt_bakedchar* _pCharData);

public:
	static Font* Load(const char* _pFileName, float _fHeight);
	float GetHeight() const;
	Vec2 GetTextSize(const char* _pText) const;
	void Draw(const char* _pText, const Vec2& _vPos) const;
	~Font();
};