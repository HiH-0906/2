#include <DxLib.h>
#include "Button.h"
#include "ImageMng.h"
#include "../Scene/BaseScene.h"

std::map<BTN_TYPE, std::map<BTN_STATE, std::string>> Button::strmap_;

bool Rect::IsHitWithPoint(Vector2 point)
{
	auto& p = point;
	return !((p.y < Top()) || (p.y > Bottom()) || (p.x < Left()) || (p.x > Right()));
}

Button::Button(const Rect& rect, const std::function<bool(void)>& func, const BTN_TYPE& type, BaseScene& scene) :rect_(rect),func_(func),type_(type),scene_(scene)
{
	state_ = BTN_STATE::NON;
	StrInit();
}

Button::~Button()
{
}

bool Button::Update(const Vector2& pos,bool flag)
{
	if (rect_.IsHitWithPoint(pos))
	{
		if (flag)
		{
			state_ = BTN_STATE::PUSH;
			return func_();
		}
		else
		{
			state_ = BTN_STATE::RIDE;
		}
	}
	else
	{
		state_ = BTN_STATE::NON;
	}
	scene_.AddDrawQue({ rect_.pos, {rect_.size.w,rect_.size.h},1.0,0, lpImageMng.GetID(strmap_[type_][state_])[0],0 });
	return true;
}

bool Button::CheckHitButton(const Vector2& pos)
{
	return rect_.IsHitWithPoint(pos);
}

void Button::StrInit(void)
{
	std::map<BTN_STATE, std::string> iniTmp;
	iniTmp.try_emplace(BTN_STATE::NON, "HBtn");
	iniTmp.try_emplace(BTN_STATE::RIDE, "HBtnR");
	iniTmp.try_emplace(BTN_STATE::PUSH, "HBtnD");
	strmap_.try_emplace(BTN_TYPE::HOST, iniTmp);
	iniTmp.clear();
	iniTmp.try_emplace(BTN_STATE::NON, "GBtn");
	iniTmp.try_emplace(BTN_STATE::RIDE, "GBtnR");
	iniTmp.try_emplace(BTN_STATE::PUSH, "GBtnD");
	strmap_.try_emplace(BTN_TYPE::GUEST, iniTmp);
	iniTmp.clear();
	iniTmp.try_emplace(BTN_STATE::NON, "OBtn");
	iniTmp.try_emplace(BTN_STATE::RIDE, "OBtnR");
	iniTmp.try_emplace(BTN_STATE::PUSH, "OBtnD");
	strmap_.try_emplace(BTN_TYPE::OFFLINE, iniTmp);
	iniTmp.clear();
	iniTmp.try_emplace(BTN_STATE::NON, "LBtn");
	iniTmp.try_emplace(BTN_STATE::RIDE, "LBtnR");
	iniTmp.try_emplace(BTN_STATE::PUSH, "LBtnD");
	strmap_.try_emplace(BTN_TYPE::LAST_HOST, iniTmp);
	iniTmp.clear();
}
