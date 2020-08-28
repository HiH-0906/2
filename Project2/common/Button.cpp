#include <DxLib.h>
#include "Button.h"
#include "../Scene/SceneMng.h"


Button::Button(Vector2&& pos, Vector2&& size)
{
	pos_ = pos;
	size_ = size;
	color = 0xffffff;
	screen = MakeScreen(1024, 512, true);
}

Button::~Button()
{
}

bool Button::Update(Vector2& pos, bool flag)
{
	color = 0xffffff;
	bool tmpflag = false;
	if (CheckHitButton(pos))
	{
		color = 0x888888;
		tmpflag = func_(pos);
	}
	Draw();
	return tmpflag;
}

void Button::SetButtonFunc(ButtonFunc&& func)
{
	func_ = func;
}

void Button::Draw(void)
{
	int idBuff = GetDrawScreen();

	SetDrawScreen(screen);
	ClsDrawScreen();
	// ‰æ‘œì‚ë‚¤ƒl
	DrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, color, true);
	auto tmppos = pos_ + (size_ / 2);
	lpSceneMng.AddDrawList({ tmppos,screen ,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });

	SetDrawScreen(idBuff);
}

bool Button::CheckHitButton(Vector2& pos)
{
	// ‚ ‚Á‚Ä‚écH
	return (pos.x > pos_.x && pos.x < (pos_.x + size_.x) && pos.y > pos_.y && pos.y < (pos_.y + size_.y));
}

