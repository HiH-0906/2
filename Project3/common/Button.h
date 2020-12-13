#pragma once
#include <functional>
#include <string>
#include <map>
#include "Vector2.h"


class BaseScene;

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

enum class BTN_TYPE
{
	NON,
	HOST,
	GUEST,
	OFFLINE,
};

enum class BTN_STATE
{
	NON,
	RIDE,
	PUSH,
};


class Button
{
public:
	Button(const Rect& rect,const std::function<bool(void)>& func, const BTN_TYPE& type,BaseScene& scene);
	~Button();

	bool Update(const Vector2& pos,bool flag);
	bool CheckHitButton(const Vector2& pos);
private:
	void StrInit(void);
	Rect rect_;
	std::string str_;
	std::function<bool(void)> func_;
	BaseScene& scene_;
	BTN_TYPE type_;
	BTN_STATE state_;
	static std::map<BTN_TYPE, std::map<BTN_STATE, std::string>> strmap_;
};

