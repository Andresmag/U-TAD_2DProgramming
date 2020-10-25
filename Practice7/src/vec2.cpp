#include "vec2.h"
#include "math.h"

// Constructors
Vec2::Vec2()
	: m_x(0)
	, m_y(0)
{
}

Vec2::Vec2(float _x, float _y) 
	: m_x(_x)
	, m_y(_y)
{
}

Vec2::Vec2(const Vec2& _vValue)
	: m_x(_vValue.m_x)
	, m_y(_vValue.m_y)
{
}

///////////////////////////////////////////////////////////////////////////////
// Getter X
float Vec2::GetXCoor() const
{
	return m_x;
}

// Getter Y
float Vec2::GetYCoor() const
{
	return m_y;
}
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Setter X
void Vec2::SetXCoor(const float &_fValue)
{
	m_x = _fValue;
}

// Setter Y
void Vec2::SetYCoor(const float &_fValue)
{
	m_y = _fValue;
}

// Setter both
void Vec2::SetCoords(const float &_fXValue, const float &_fYValue)
{
	m_x = _fXValue;
	m_y = _fYValue;
}
///////////////////////////////////////////////////////////////////////////////

// Vector Length
float Vec2::Length() const
{
	return( static_cast<float>(sqrt((double)m_x*m_x + (double)m_y*m_y)) );
}


// Dot product
float Vec2::Dot(const Vec2 & _vOther) const
{
	return (m_x*_vOther.m_x + m_y*_vOther.m_y);
}

// Angle between 2 vecs in degrees
float Vec2::Angle(const Vec2& _vOther) const
{
	return ( static_cast<float>(atan2((double)m_y - _vOther.m_y, (double)m_x - _vOther.m_x) * 180 / PI) );
}

// Distance between 2 vecs
float Vec2::Distance(const Vec2& _vOther) const
{
	Vec2 distanceVec(_vOther.m_x - m_x, _vOther.m_y - m_y);
	return (distanceVec.Length());
}

// Normalize vector
void Vec2::Normalize()
{
	float length = Length();
	m_x /= length;
	m_y /= length;
}

void Vec2::Abs()
{
	m_x = fabs(m_x);
	m_y = fabs(m_y);
}

///////////////////////////////////////////////////////////////////////////////
// Overload operator+
const Vec2 Vec2::operator+ (const Vec2 & _vOther) const
{
	return Vec2(m_x + _vOther.m_x, m_y + _vOther.m_y);
}

// Overload operator-
const Vec2 Vec2::operator- (const Vec2& _vOther) const
{
	return Vec2(m_x - _vOther.m_x, m_y - _vOther.m_y);
}

// Overload operator+=
Vec2 & Vec2::operator+= (const Vec2 & _vOther) 
{
	m_x += _vOther.m_x;
	m_y += _vOther.m_y;
	return *this;
}

// Overload operator*
const Vec2 Vec2::operator* (const Vec2& _vOther) const
{
	return Vec2(m_x * _vOther.m_x ,m_y * _vOther.m_y);
}

const Vec2 Vec2::operator*(float _fValue) const
{
	return Vec2(m_x*_fValue, m_y*_fValue);
}

// Overload operator== 
bool Vec2::operator==(const Vec2& _vOther) const
{
	return (m_x == _vOther.m_x && m_y == _vOther.m_y);
}
