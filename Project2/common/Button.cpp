#include <DxLib.h>
#include "Button.h"
#include "../Scene/SceneMng.h"
#include "../ImageMng.h"


Button::Button(Vector2&& pos, float&& speed, std::string&& key, BUTTON_MOVE&& move)
{
	pos_ = pos;
	defPos_ = pos;
	rad_ = 0.0;
	rate_ = 1.0;
	speed_ = speed;
	key_ = key;
	cnt_ = 0;
	GetGraphSize(IMAGE_ID(key)[0], &size_.x, &size_.y);
	// ‰½‚à“ü‚ê‚Ä–³‚¢‚Æ‚Ü‚¸‚¢‚Ì‚Å‚Æ‚è‚ ‚¦‚¸
	moveFunc_.try_emplace(BUTTON_MOVE::NOMAL, [&]() {rate_ = 1.0f; pos_ = defPos_; });
	moveFunc_.try_emplace(BUTTON_MOVE::SCALING, [&]() {
		rate_ += speed_;
		if (rate_ >= 1.1f || rate_ <= 0.9f)
		{
			speed_ *= -1.0f;
		}
	});
	moveFunc_.try_emplace(BUTTON_MOVE::UPDOWN, [&]() {
		if (cnt_ % 2)
		{
			pos_.y += static_cast<int>(speed_);
		}
		if (cnt_ % 90 == 0)
		{
			speed_ *= -1;
		}
	});
	func_ = []() {};
	mode_ = BUTTON_MOVE::NOMAL;
	change_ = move;
}

Button::~Button()
{
}

void Button::Update(Vector2& pos)
{
	if (CheckHitButton(pos))
	{
		mode_ = change_;
	}
	else
	{
		mode_ = BUTTON_MOVE::NOMAL;
	}
	moveFunc_[mode_]();
	Draw();
	cnt_++;
}

Vector2& Button::pos(void)
{
	return pos_;
}

void Button::Draw(void)
{
	lpSceneMng.AddDrawList({ pos_,IMAGE_ID(key_)[0] ,rate_,rad_,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
}

bool Button::CheckHitButton(Vector2& pos)
{
	// ‚ ‚Á‚Ä‚écH
	auto halfSize = size_ / 2;
	return (pos.x > pos_.x - halfSize.x && pos.x < (pos_.x + halfSize.x) && pos.y > pos_.y - halfSize.y && pos.y < (pos_.y + halfSize.y));
}

void Button::SetButonFunc(ButtonFunc func)
{
	func_ = func;
}

