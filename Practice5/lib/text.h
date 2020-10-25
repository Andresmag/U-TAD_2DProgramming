#pragma once

class Font;

class Text
{
public:
	Text();
	Text(const char* _sText, const Font* _pFont, const Vec2& _vPos, const unsigned int& _uSpeed, const float* _pColor);
	~Text();
	const char* getText();
	const Font* getFont();
	const Vec2 getPos();
	float* getColor();
	void UpdatePosition(float _fSeconds);
	void Draw();

private:
	const char* m_sText;
	const Font* m_pFont;
	Vec2 m_vPos;
	unsigned int m_uSpeed;
	float m_pColor[3];
};