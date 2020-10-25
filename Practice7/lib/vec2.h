#pragma once

#define PI 3.14159265

class Vec2 
{
private:
	float m_x;
	float m_y;

public:
	Vec2();
	Vec2(float _x, float _y);
	Vec2(const Vec2& _vValue);
	float GetXCoor() const;
	float GetYCoor() const;
	void SetXCoor(const float &_fValue);
	void SetYCoor(const float &_fValue);
	void SetCoords(const float &_fXValue, const float &_fYValue);
	float Length() const;
	float Dot(const Vec2 & _vOther) const;
	float Angle(const Vec2& other) const;
	float Distance(const Vec2& other) const;
	void Normalize();
	void Abs();
	//-------------
	const Vec2 operator+ (const Vec2 & _vOther) const;
	const Vec2 operator- (const Vec2& _vOther) const;
	Vec2 & operator+= (const Vec2 & _vOther);
	const Vec2 operator* (const Vec2& _vOther) const;
	const Vec2 operator* (float _fValue) const;
	bool operator== (const Vec2& _vOther) const;
};