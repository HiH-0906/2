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
	screen = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, true);
	// ‰½‚à“ü‚ê‚Ä–³‚¢‚Æ‚Ü‚¸‚¢‚Ì‚Å‚Æ‚è‚ ‚¦‚¸
	func_.try_emplace(BUTTON_MOVE::NOMAL, [&]() {rate_ = 1.0f; addNum_ = 0.003f; });
	func_.try_emplace(BUTTON_MOVE::SCALING, [&]() {
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

bool Button::Update(Vector2& pos, bool flag)
{
	bool tmpflag = false;
	if (CheckHitButton(pos))
	{
		mode_ = BUTTON_MOVE::SCALING;
		tmpflag = true;
	}
	else
	{
		mode_ = BUTTON_MOVE::NOMAL;
	}
	func_[mode_]();
	Draw();
	return tmpflag;
}

Vector2& Button::pos(void)
{
	return pos_;
}

void Button::Draw(void)
{
	int idBuff = GetDrawScreen();

	SetDrawScreen(screen);
	ClsDrawScreen();
	// ‰æ‘œì‚ë‚¤ƒl
	DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, color, true);
	lpSceneMng.AddDrawList({ pos_,IMAGE_ID(key_)[0] ,rate_,rad_,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });

	SetDrawScreen(idBuff);
}

bool Button::CheckHitButton(Vector2& pos)
{
	// ‚ ‚Á‚Ä‚écH
	auto halfSize = size_ / 2;
	return (pos.x > pos_.x - halfSize.x && pos.x < (pos_.x + halfSize.x) && pos.y > pos_.y - halfSize.y && pos.y < (pos_.y + halfSize.y));
}

