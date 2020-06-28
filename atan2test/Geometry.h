#pragma once

struct Vector2
{
	int x, y;
	Vector2() :x(0), y(0) {}
	Vector2(int x, int y) :x(x), y(y) {}
};
Vector2 operator +(const Vector2& lvel, const Vector2& levl2);
Vector2 operator -(const Vector2& lvel, const Vector2& levl2);

typedef Vector2 Position2;

/// <summary>
/// 矩形（長方形）を表す
/// </summary>
struct Rect
{
	int left;	// 左
	int top;	// 上
	int width;	// 幅
	int height;	// 高さ
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
	Position2 center;	// 中心座標
	float r;			// 半径
};
