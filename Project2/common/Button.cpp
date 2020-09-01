#include <DxLib.h>
#include "Button.h"
#include "../Scene/SceneMng.h"
#include "../ImageMng.h"


Button::Button(Vector2&& pos, float&& rad, float&& rate, std::string&& key)
{
	pos_ = pos;
	rad_ = rad;
	rate_ = rate;
	addNum_ = 0.003f;
	key_ = key;
	GetGraphSize(IMAGE_ID(key)[0], &size_.x, &size_.y);
	// 何も入れて無いとまずいのでとりあえず
	moveFunc_.try_emplace(BUTTON_MOVE::NOMAL, [&]() {rate_ = 1.0f; addNum_ = 0.003f; });
	moveFunc_.try_emplace(BUTTON_MOVE::SCALING, [&]() {
		rate_ += addNum_;
		if (rate_ >= 1.1f || rate_ <= 0.9f)
		{
			addNum_ *= -1.0f;
		}
	});
	mode_ = BUTTON_MOVE::NOMAL;
}

Button::~Button()
{
}

void Button::Update(Vector2& pos)
{
	if (CheckHitButton(pos))
	{
		mode_ = BUTTON_MOVE::SCALING;
	}
	else
	{
		mode_ = BUTTON_MOVE::NOMAL;
	}
	moveFunc_[mode_]();
	Draw();
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
	// あってる…？
	auto halfSize = size_ / 2;
	return (pos.x > pos_.x - halfSize.x && pos.x < (pos_.x + halfSize.x) && pos.y > pos_.y - halfSize.y && pos.y < (pos_.y + halfSize.y));
}

