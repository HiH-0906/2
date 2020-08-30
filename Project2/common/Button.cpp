#include <DxLib.h>
#include "Button.h"
#include "../Scene/SceneMng.h"


Button::Button(Vector2&& pos, Vector2&& size)
{
	pos_ = pos;
	size_ = size;
	color = 0xffffff;
	screen = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, true);
	func_ = [](Vector2& pos) {return true; };
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
	auto pos = lpSceneMng.screenSize() / 2;
	lpSceneMng.AddDrawList({ pos,screen ,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });

	SetDrawScreen(idBuff);
}

bool Button::CheckHitButton(Vector2& pos)
{
	// ‚ ‚Á‚Ä‚écH
	return (pos.x > pos_.x && pos.x < (pos_.x + size_.x) && pos.y > pos_.y && pos.y < (pos_.y + size_.y));
}

