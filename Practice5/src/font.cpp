#include "font.h"
#include "vec2.h"
#include <cstdio>
#include <cstring>

// Constructor
Font::Font(float _fHeight, ltex_t* texture, stbtt_bakedchar* _pCharData)
	: m_fHeight(_fHeight)
	, m_pTex(texture)
	, m_pCharData(_pCharData)
{
}

// Destructor
Font::~Font()
{
	m_fHeight = NULL;
	delete m_pTex;
	delete[] m_pCharData;
}

// Load
Font* Font::Load(const char* _pFileName, float _fHeight)
{
	FILE* pFontFile = fopen(_pFileName, "rb"); // Lo abrimos en modo binario para que uBytesRead sea igual a uFileSize
	if (pFontFile != nullptr)
	{
		fseek(pFontFile, 0, SEEK_END); // posicionamos el puntero al final del fichero
		unsigned int uFileSize = static_cast<unsigned int>(ftell(pFontFile)); // obtenemos el tamanio del fichero
		rewind(pFontFile); // devolvemos el puntero al comienzo del fichero
		unsigned char* pBufferRead = new unsigned char[uFileSize];
		if (pBufferRead != nullptr)
		{
			unsigned int uBytesRead = fread(pBufferRead, sizeof(unsigned char), uFileSize, pFontFile);
			if (uBytesRead == uFileSize)
			{
				unsigned int uPixelsLenght = 512 * 512;
				unsigned char* pPixels = new unsigned char[uPixelsLenght];
				stbtt_bakedchar* pChardata = new stbtt_bakedchar[128];
				int result = stbtt_BakeFontBitmap(pBufferRead, 0, _fHeight, pPixels, 512, 512, 0, 127, pChardata);
				fclose(pFontFile);
				delete[] pBufferRead;
				
				// Aniadir RGB a pixels
				unsigned char* pColoredPixels = new unsigned char[uPixelsLenght *4];
				for (unsigned int uIndex = 0; uIndex < uPixelsLenght; ++uIndex)
				{
					unsigned int uTuplaIndex = 0;
					while (uTuplaIndex < 4)
					{
						if (uTuplaIndex != 3)
						{
							*(pColoredPixels + (uIndex * 4) + uTuplaIndex) = 255;
						}
						else
						{
							*(pColoredPixels + (uIndex * 4) + uTuplaIndex) = *(pPixels + uIndex);
						}

						uTuplaIndex++;
					}
				}

				delete[] pPixels;

				ltex_t *texture = ltex_alloc(512, 512, 0);
				ltex_setpixels(texture, pColoredPixels);

				delete[] pColoredPixels;

				Font* font = new Font(_fHeight, texture, pChardata);

				return font;
			}

			return nullptr;
		}

		return nullptr;
	}

	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// Getter
float Font::GetHeight() const
{
	return m_fHeight;
}

///////////////////////////////////////////////////////////////////////////////
Vec2 Font::GetTextSize(const char* _pText) const
{
	stbtt_aligned_quad quad = {0,0,0,0,0,0,0,0};
	float fXPos = 0;
	float fYPos = 0;
	float fInitialX = 0;
	for (unsigned int uIndex = 0; uIndex < strlen(_pText); ++uIndex)
	{
		stbtt_GetBakedQuad(m_pCharData, m_pTex->width, m_pTex->height, _pText[uIndex], &fXPos, &fYPos, &quad, true);
		if (uIndex == 0)
		{
			fInitialX = quad.x0;
		}
	}

	return Vec2(quad.x1 - fInitialX, quad.y1 - quad.y0);
}

void Font::Draw(const char* _pText, const Vec2& _vPos) const 
{
	stbtt_aligned_quad quad;
	float fXPos = _vPos.GetXCoor();
	float fYPos = _vPos.GetYCoor();
	for (unsigned int uIndex = 0; uIndex < strlen(_pText); ++uIndex) 
	{
		stbtt_GetBakedQuad(m_pCharData, m_pTex->width, m_pTex->height, _pText[uIndex], &fXPos, &fYPos, &quad, true);
		ltex_drawrotsized(m_pTex, quad.x0, quad.y0, 0,0,0, quad.x1 - quad.x0, quad.y1 - quad.y0, quad.s0, quad.t0, quad.s1, quad.t1);
	}

}