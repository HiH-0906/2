#include "Geometry.h"
#include <math.h>

Vector2 Vector2::operator+(const Vector2& lvel)
{
	return Vector2(x + lvel.x, y + lvel.y);
}

Vector2 Vector2::operator-(const Vector2& lvel)
{
	return Vector2(x - lvel.x, y - lvel.y);
}

Vector2f Vector2f::operator+(const Vector2f& lvel)
{
	return Vector2f(x + lvel.x, y + lvel.y);
}

Vector2f Vector2f::operator-(const Vector2f& lvel)
{
	return Vector2f(x - lvel.x, y-lvel.y);
}

Vector2f Vector2f::operator *(const float& num)
{
	return Vector2f(x * num, y * num);
}


float Vector2f::Magnitude() const
{
	return hypotf(x,y);
}

void Vector2f::Normarize() 
{
	float tmp = Magnitude();
	x / tmp;
	y / tmp;
}

Vector2f Vector2f::Normarized() const
{
	float tmp = Magnitude();
	return Vector2f(x / tmp, y / tmp);
}
