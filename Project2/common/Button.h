#pragma once
#include <memory>
#include <string>
#include <map>
#include <functional>
#include "../Scene/BaseScene.h"
#include "Vector2.h"

using ButtonMoveFunc = std::function<void(void)>;
using ButtonFunc = std::function<void(void)>;


enum class BUTTON_MOVE
{
	NOMAL,
	SCALING,
	UPDOWN
};

class Button
{
public:
	Button(Vector2&& pos,float&& speed, std::string&& key,BUTTON_MOVE&& move);
	~Button();
	void Update(Vector2& pos);
	Vector2& pos(void);
	bool CheckHitButton(Vector2& pos);
	void SetButonFunc(ButtonFunc func);
private:
	void Draw(void);
protected:
	int color;

	// •\Ž¦ŠÖŒW
	Vector2 pos_;
	Vector2 defPos_;
	Vector2 size_;
	float rad_;
	float rate_;
	std::string key_;
	int cnt_;

	std::map<BUTTON_MOVE, ButtonMoveFunc> moveFunc_;
	ButtonFunc func_;

	BUTTON_MOVE mode_;
	BUTTON_MOVE change_;

	float speed_;
};

using sharedButton = std::shared_ptr<Button>;