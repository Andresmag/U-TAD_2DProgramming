#include  "litegfx.h"
#include "vec2.h"
#include <algorithm>
#include "collider.h"

// Check circle
bool Collider::checkCircleCircle(const Vec2& pos1, float radius1, const Vec2& pos2, float radius2) const
{
	return (pos1.Distance(pos2) <= radius1 + radius2);
}

bool Collider::checkCircleRect(const Vec2& circlePos, float circleRadius, const Vec2& rectPos, const Vec2& rectSize) const
{
	// Hallamos el punto del rectangulo mas cercano al circulo
	Vec2 nearestPoint(std::max(rectPos.GetXCoor(), std::min(circlePos.GetXCoor(), rectPos.GetXCoor() + rectSize.GetXCoor())),
		std::max(rectPos.GetYCoor(), std::min(circlePos.GetYCoor(), rectPos.GetYCoor() + rectSize.GetYCoor())));
	return checkCircleCircle(circlePos, circleRadius, nearestPoint, 0);
}

bool Collider::checkCirclePixels(const Vec2& circlePos, float circleRadius, const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const
{
	Vec2 topLeftCorner(circlePos.GetXCoor() - circleRadius, circlePos.GetYCoor() - circleRadius);

	Vec2 intersecRectTopLeftPos(std::max(pixelsPos.GetXCoor(), topLeftCorner.GetXCoor()), std::max(pixelsPos.GetYCoor(), topLeftCorner.GetYCoor()));
	Vec2 intersecRectBottomRightPos(std::min(pixelsPos.GetXCoor() + pixelsSize.GetXCoor(), topLeftCorner.GetXCoor() + circleRadius*2),
		std::min(pixelsPos.GetYCoor() + pixelsSize.GetYCoor(), topLeftCorner.GetYCoor() + circleRadius*2));
	Vec2 intersecRectSize(intersecRectBottomRightPos.GetXCoor() - intersecRectTopLeftPos.GetXCoor(), intersecRectBottomRightPos.GetYCoor() - intersecRectTopLeftPos.GetYCoor());
	
	//lgfx_drawrect(intersecRectTopLeftPos.GetXCoor(), intersecRectTopLeftPos.GetYCoor(), intersecRectSize.GetXCoor(), intersecRectSize.GetYCoor());
	
	if (intersecRectSize.GetXCoor() > 0 && intersecRectSize.GetYCoor() > 0)
	{
		for (unsigned int i = 0; i < intersecRectSize.GetYCoor(); ++i)
		{
			for (unsigned int j = 0; j < intersecRectSize.GetXCoor(); ++j)
			{
				Vec2 pixel((intersecRectTopLeftPos.GetXCoor() + j), (intersecRectTopLeftPos.GetYCoor() + i));
				if (pixel.Distance(circlePos) <= circleRadius)
				{
					Vec2 coor((intersecRectTopLeftPos.GetXCoor() + j) - pixelsPos.GetXCoor(), (intersecRectTopLeftPos.GetYCoor() + i) - pixelsPos.GetYCoor());
					unsigned int alphaPosInPixels = 4 * (coor.GetXCoor() + coor.GetYCoor() * pixelsSize.GetXCoor()) + 3;

					if (pixels[alphaPosInPixels] > 0)
					{
						return true;
						/*lgfx_setcolor(1, 0, 0, 1);
						lgfx_drawpoint(coor.GetXCoor(), coor.GetYCoor());*/
					}
					/*else
					{
						lgfx_setcolor(0, 1, 0, 1);
						lgfx_drawpoint(coor.GetXCoor(), coor.GetYCoor());
					}*/
				}
			}
		}
	}

	return false;
}

// Check Rect
bool Collider::checkRectRect(const Vec2& rectPos1, const Vec2& rectSize1, const Vec2& rectPos2, const Vec2& rectSize2) const
{
	return (rectPos1.GetXCoor() < rectPos2.GetXCoor() + rectSize2.GetXCoor() && rectPos1.GetXCoor() + rectSize1.GetXCoor() > rectPos2.GetXCoor()
		&& rectPos1.GetYCoor() < rectPos2.GetYCoor() + rectSize2.GetYCoor() && rectPos1.GetYCoor() + rectSize1.GetYCoor() > rectPos2.GetYCoor());
}

// Check Pixels
bool Collider::checkPixelsPixels(const Vec2& pixelsPos1, const Vec2& pixelsSize1, const uint8_t* pixels1, const Vec2& pixelsPos2, const Vec2& pixelsSize2, const uint8_t* pixels2) const
{
	// Hallamos el rectangulo interseccion entre los dos sprites
	Vec2 intersecRectTopLeftPos(std::max(pixelsPos1.GetXCoor(), pixelsPos2.GetXCoor()), std::max(pixelsPos1.GetYCoor(), pixelsPos2.GetYCoor()));
	Vec2 intersecRectBottomRightPos(std::min(pixelsPos1.GetXCoor() + pixelsSize1.GetXCoor(), pixelsPos2.GetXCoor() + pixelsSize2.GetXCoor()),
		std::min(pixelsPos1.GetYCoor() + pixelsSize1.GetYCoor(), pixelsPos2.GetYCoor() + pixelsSize2.GetYCoor()));
	Vec2 intersecRectSize(intersecRectBottomRightPos.GetXCoor() - intersecRectTopLeftPos.GetXCoor(), intersecRectBottomRightPos.GetYCoor() - intersecRectTopLeftPos.GetYCoor());

	if (intersecRectSize.GetXCoor() > 0 && intersecRectSize.GetYCoor() > 0)
	{
		for (unsigned int i = 0; i < intersecRectSize.GetYCoor(); ++i)
		{
			for (unsigned int j = 0; j < intersecRectSize.GetXCoor(); ++j)
			{
				// Pasamos las coordenenadas del rectangulo de interseccion al valor correspondiente en cada uno de los otros rectagulos de los sprites
				Vec2 coorIn1((intersecRectTopLeftPos.GetXCoor() + j) - pixelsPos1.GetXCoor(), (intersecRectTopLeftPos.GetYCoor() + i) - pixelsPos1.GetYCoor());
				Vec2 coorIn2((intersecRectTopLeftPos.GetXCoor() + j) - pixelsPos2.GetXCoor(), (intersecRectTopLeftPos.GetYCoor() + i) - pixelsPos2.GetYCoor());

				// Pasamos de coordenadas de matriz a coordenadas de vector y consideramos solo el valor de alpha
				unsigned int alphaPosInPixels1 = 4 * (coorIn1.GetXCoor() + coorIn1.GetYCoor() * pixelsSize1.GetXCoor()) + 3;
				unsigned int alphaPosInPixels2 = 4 * (coorIn2.GetXCoor() + coorIn2.GetYCoor() * pixelsSize2.GetXCoor()) + 3;

				if (pixels1[alphaPosInPixels1] > 0 && pixels2[alphaPosInPixels2] > 0)
				{
					return true;
				}
			}
		}
	}
	
	return false;
}

bool Collider::checkPixelsRect(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels, const Vec2& rectPos, const Vec2& rectSize) const
{
	Vec2 intersecRectTopLeftPos(std::max(pixelsPos.GetXCoor(), rectPos.GetXCoor()), std::max(pixelsPos.GetYCoor(), rectPos.GetYCoor()));
	Vec2 intersecRectBottomRightPos(std::min(pixelsPos.GetXCoor() + pixelsSize.GetXCoor(), rectPos.GetXCoor() + rectSize.GetXCoor()),
		std::min(pixelsPos.GetYCoor() + pixelsSize.GetYCoor(), rectPos.GetYCoor() + rectSize.GetYCoor()));
	Vec2 intersecRectSize(intersecRectBottomRightPos.GetXCoor() - intersecRectTopLeftPos.GetXCoor(), intersecRectBottomRightPos.GetYCoor() - intersecRectTopLeftPos.GetYCoor());
	
	if (intersecRectSize.GetXCoor() > 0 && intersecRectSize.GetYCoor() > 0)
	{
		for (unsigned int i = 0; i < intersecRectSize.GetYCoor(); ++i)
		{
			for (unsigned int j = 0; j < intersecRectSize.GetXCoor(); ++j)
			{
				Vec2 coor((intersecRectTopLeftPos.GetXCoor() + j) - pixelsPos.GetXCoor(), (intersecRectTopLeftPos.GetYCoor() + i) - pixelsPos.GetYCoor());
				unsigned int alphaPosInPixels = 4 * (coor.GetXCoor() + coor.GetYCoor() * pixelsSize.GetXCoor()) + 3;

				if (pixels[alphaPosInPixels] > 0)
				{
					return true;
					/*lgfx_setcolor(1, 0, 0, 1);
					lgfx_drawpoint(coor.GetXCoor(), coor.GetYCoor());*/
				}
				/*else
				{
					lgfx_setcolor(0, 1, 0, 1);
					lgfx_drawpoint(coor.GetXCoor(), coor.GetYCoor());
				}*/
			}
		}
	}

	return false;
}