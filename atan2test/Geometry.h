#pragma once

struct Vector2
{
	int x, y;
	Vector2() :x(0), y(0) {}
	Vector2(int x, int y) :x(x), y(y) {}
	Vector2 operator +(const Vector2& lvel);
	Vector2 operator -(const Vector2& lvel);
};

struct Vector2f
{
	float x, y;
	Vector2f() :x(0),y(0) {}
	Vector2f(float x, float y) :x(x), y(y) {}
	float Magnitude()const;
	void Normarize();
	Vector2f Normarized()const;
	Vector2f operator +(const Vector2f& lvel);
	Vector2f operator -(const Vector2f& lvel);
	Vector2f operator *(const float& num);
};



typedef Vector2 Position2;
typedef Vector2f position2f; 

/// <summary>
/// ��`�i�����`�j��\��
/// </summary>
struct Rect
{
	int left;	// ��
	int top;	// ��
	int width;	// ��
	int height;	// ����
	int Left()const
	{
		return left;
	}
	int Top()const
	{
		return top;
	}
	int Right()const
	{
		return left + width;
	}
	int Bottom()const
	{
		return top + height;
	}
	Vector2 Center()const
	{
		return Vector2(left+width / 2, top+height / 2);
	}
};

struct Circle
{
	Position2 center;	// ���S���W
	float r;			// ���a
};
