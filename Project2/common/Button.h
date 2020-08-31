#pragma once
#include <memory>
#include <string>
#include <map>
#include <functional>
#include "Vector2.h"

using ButtonFunc = std::function<void(void)>;


enum class BUTTON_MOVE
{
	NOMAL,
	SCALING,
};

class Button
{
public:
	Button(Vector2&& pos,float&& rad,float&& rate, std::string&& key);
	~Button();
	bool Update(Vector2& pos,bool flag);
	Vector2& pos(void);

private:
	void Draw(void);
	int screen;
protected:
	int color;

	// •\Ž¦ŠÖŒW
	Vector2 pos_;
	Vector2 size_;
	float rad_;
	float rate_;
	std::string key_;

	std::map<BUTTON_MOVE, ButtonFunc> func_;
	BUTTON_MOVE mode_;

	float addNum_;

	bool CheckHitButton(Vector2& pos);
};

using sharedButton = std::shared_ptr<Button>;