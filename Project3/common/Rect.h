#pragma once
#include "Vector2.h"

// ÉTÉCÉY
struct Size
{
	int w, h;
	Size() :w(0), h(0) {};
	Size(int inw, int inh) :w(inw), h(inh) {};
};

struct Rect
{
	Vector2 pos;
	Size size;

	Rect() :pos(0, 0), size(0, 0) {};
	Rect(const Vector2& p, const Size& s) :pos(p), size(s) {};
	Rect(int inx, int iny, int inw, int inh) :pos(inx, iny), size(inw, inh) {};

	int Top()const
	{
		return pos.y;
	}

	int Bottom()const
	{
		return pos.y + size.h;
	}

	int Left()const
	{
		return pos.x;
	}

	int Right()const
	{
		return pos.x + size.w;
	}

	int Width()const
	{
		return size.w;
	}

	int Highth()const
	{
		return size.h;
	}

	Vector2 Center()
	{
		return Vector2(pos.x + size.w / 2, pos.y + size.h / 2);
	}

	bool IsHitWithPoint(Vector2 point);
};

