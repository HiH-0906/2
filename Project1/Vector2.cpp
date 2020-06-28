#include "Vector2.h"

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2(int x, int y)
{
	this->x = x;
	this->y = y;
}

Vector2::~Vector2()
{
}

Vector2& Vector2::operator=(const Vector2& vec)
{
	x = vec.x;
	y = vec.y;
	return (*this);
}

int& Vector2::operator[](int num)
{
	if (num == 0)
	{
		return x;
	}
	else if (num == 1)
	{
		return y;
	}
	else
	{
		// ‘z’è‚µ‚Ä‚¢‚È‚¢“Y‚¦Žš‚ª—ˆ‚½ê‡‚Æ‚è‚ ‚¦‚¸x‚ð‚©‚¦‚·
		return x;
	}
}

Vector2& Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

Vector2& Vector2::operator*=(int k)
{
	x *= k;
	y *= k;
	return *this;
}

Vector2& Vector2::operator/=(int k)
{
	x /= k;
	y /= k;
	return *this;
}

Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-this->x, -this->y);
}

Vector2& Vector2::operator+=(const int u)
{
	this->x += u;
	this->y += u;
	return *this;
}

bool Vector2::operator==(const Vector2& vec) const
{
	return ((this->x == vec.x) && (this->y == vec.y));
}

bool Vector2::operator!=(const Vector2& vec) const
{
	return !((this->x == vec.x) && (this->y == vec.y));
}

bool Vector2::operator<=(const Vector2& vec) const
{
	return ((this->x <= vec.x) && (this->y <= vec.y));
}

bool Vector2::operator<(const Vector2& vec) const
{
	return ((this->x < vec.x) && (this->y < vec.y));
}

bool Vector2::operator>=(const Vector2& vec) const
{
	return ((this->x >= vec.x) && (this->y >= vec.y));
}

bool Vector2::operator>(const Vector2& vec) const
{
	return ((this->x > vec.x) && (this->y > vec.y));
}

Vector2 operator+(const Vector2& vec, int u)
{
	return Vector2(vec.x + u, vec.y + u);
}

Vector2 operator-(const Vector2& vec, int u)
{
	return Vector2(vec.x - u, vec.y - u);
}
Vector2 operator*(const Vector2& vec, int u)
{
	return Vector2(vec.x * u, vec.y * u);
}

Vector2 operator/(const Vector2& vec, int u)
{
	return Vector2(vec.x / u, vec.y / u);
}

Vector2 operator%(const Vector2& vec, int u)
{
	return Vector2(vec.x % u, vec.y % u);
}

Vector2 operator*(int u, const Vector2& vec)
{
	return Vector2(u * vec.x, u * vec.y);
}

Vector2 operator+(const Vector2& veca, const Vector2& vecb)
{
	return Vector2(veca.x + vecb.x, veca.y + vecb.y);
}

Vector2 operator-(const Vector2& veca, const Vector2& vecb)
{
	return Vector2(veca.x - vecb.x, veca.y - vecb.y);
}

Vector2 operator*(const Vector2& veca, const Vector2& vecb)
{
	return Vector2(veca.x * vecb.x, veca.y * vecb.y);
}

Vector2 operator/(const Vector2& veca, const Vector2& vecb)
{
	return Vector2(veca.x / vecb.x, veca.y / vecb.y);
}

Vector2 operator%(const Vector2& veca, const Vector2& vecb)
{
	return Vector2(veca.x % vecb.x, veca.y % vecb.y);
}
