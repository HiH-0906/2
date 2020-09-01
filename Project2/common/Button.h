#pragma once
#include <memory>
#include <string>
#include <map>
#include <functional>
#include "../Scene/BaseScene.h"
#include "Vector2.h"

using ButtonMoveFunc = std::function<void(void)>;


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
	void Update(Vector2& pos);
	Vector2& pos(void);
	bool CheckHitButton(Vector2& pos);
private:
	void Draw(void);
protected:
	int color;

	// •\Ž¦ŠÖŒW
	Vector2 pos_;
	Vector2 size_;
	float rad_;
	float rate_;
	std::string key_;

	std::map<BUTTON_MOVE, ButtonMoveFunc> moveFunc_;

	BUTTON_MOVE mode_;

	float addNum_;
};

using sharedButton = std::shared_ptr<Button>;