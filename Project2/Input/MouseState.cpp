#include <DxLib.h>
#include <math.h>
#include "MouseState.h"
#include "../ImageMng.h"
#include "../Scene/SceneMng.h"

MouseState::MouseState()
{
	lpImageMng.GetID("マウス", "image/cursol.png");
	lpImageMng.GetID("マウス1", "image/mouseFream.png");
	lpImageMng.GetID("マウス2", "image/mouse2.png");
	mouseScreen_ = MakeScreen(128, 128, true);
	_mouseData = 0;
	_mouseDataOld = 0;
	cnt_ = 0;
	rad_ = 32;
}

MouseState::~MouseState()
{
}

CON_ID MouseState::GetID(void)
{
	return CON_ID::MOUSE;
}

void MouseState::Update(void)
{
	_mouseDataOld = _mouseData;
	_mouseData = GetMouseInput();
	for (auto id : INPUT_ID())
	{
		_input[id].second = _input[id].first;
		_input[id].first = false;
	}
	int mposx, mposy;
	GetMousePoint(&mposx, &mposy);
	// 延々とたまるbuff
	_buff = _stanPos - Vector2{ mposx,mposy };
	auto buff = GetMouseWheelRotVol();
	if (abs(buff))
	{
		cnt_ = 0;
	}
	/*if (cnt_ % 30 == 0)
	{*/
		// ごり押し
		if (/*_buff.x >= rad_*/buff<0)
		{
			_input[INPUT_ID::LEFT].first = true;
			_input[INPUT_ID::LEFT].second = false;
		}
		else if (/*_buff.x <= -rad_*/buff > 0)
		{
			_input[INPUT_ID::RIGHT].first = true;
			_input[INPUT_ID::RIGHT].second = false;
		}
		if (_buff.y >= rad_)
		{
			_input[INPUT_ID::UP].first = true;
			_input[INPUT_ID::UP].second = false;
		}
	//}
	if (_buff.y <= -rad_)
	{
		_input[INPUT_ID::DOWN].first = true;
		_input[INPUT_ID::DOWN].second = true;
	}
	_input[INPUT_ID::LROTA].first = GetMouseTrg(MOUSE_INPUT_LEFT);
	_input[INPUT_ID::RROTA].first = GetMouseTrg(MOUSE_INPUT_RIGHT);
	if (GetMouseTrg(MOUSE_INPUT_MIDDLE))
	{
		// ﾎｲｰﾙｸﾘｯｸで基準位置変更
		GetMousePoint(&_stanPos.x, &_stanPos.y);
	}
	//Draw({ mposx, mposy });
	cnt_++;
}

void MouseState::Setting(const int& p_id, const int& pad_id)
{
	_buff = { 0, 0 };
	_stanPos = { 512,384 };
	p_id_ = p_id;
	pad_id_ = pad_id;
}

void MouseState::Draw(Vector2&& pos)
{
	int idBuff = GetDrawScreen();
	SetDrawScreen(mouseScreen_);
	ClsDrawScreen();
	DrawRotaGraph(64, 64, 1.0, 0.0, IMAGE_ID("マウス1")[0], true);
	auto tmpPos = pos - _stanPos;
	auto angle = atan2(tmpPos.y, tmpPos.x);

	auto tmpRenge = tmpPos.x * tmpPos.x + tmpPos.y * tmpPos.y;
	tmpRenge = static_cast<int>(sqrt(tmpRenge));
	auto rad = min(rad_, tmpRenge);
	rad = min(rad, 16);
	tmpPos = { static_cast<int>(cos(angle) * rad),static_cast<int>(sin(angle) * rad) };
	DrawRotaGraph(64 + tmpPos.x, 64 + tmpPos.y, 1.0, 0.0, IMAGE_ID("マウス2")[0], true);

	lpSceneMng.AddDrawList({ _stanPos,mouseScreen_,1.0,0.0,8,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });

	SetDrawScreen(idBuff);
}

bool MouseState::GetMouseTrg(int id)
{
	return ((_mouseData & id) != 0 && (_mouseDataOld & id) == 0);
}
