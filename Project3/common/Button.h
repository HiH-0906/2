#pragma once
#include <functional>
#include <string>
#include <map>
#include "Vector2.h"
#include "Rect.h"


class BaseScene;

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

