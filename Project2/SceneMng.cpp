#include <DxLib.h>
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "State/CON_ID.h"


std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::S_instance(new SceneMng());

void SceneMng::Run()
{
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		//_dbgStartDraw();
		for (auto&& erea:_playErea)
		{
			erea->UpDate();
		}
		Draw();
	}
	DxLib::DxLib_End();
}

const Vector2 SceneMng::screenSize(void) const
{
	return Vector2{ screenX,screenY };
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	for (size_t i = 0; i < _playErea.size(); i++)
	{
		DrawGraph(_playErea[i]->offset().x, _playErea[i]->offset().y, _playErea[i]->GetScreenID(), true);
	}

	ScreenFlip();
}

bool SceneMng::SysInit(void)
{
	ChangeWindowMode(true);
	SetWindowText("1916035_���{��P");
	SetGraphMode(screenX, screenY, 16);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	Vector2 size = { 512,512 };
	Vector2 offset = { 100,100 };
	_playErea.emplace_back(std::make_unique<PleyErea>(std::move(size),std::move(offset),CON_ID::KEY));
	offset = { 612,100 };
	_playErea.emplace_back(std::make_unique<PleyErea>(std::move(size), std::move(offset),CON_ID::KEY));
	return true;
}

SceneMng::SceneMng():screenX(1024),screenY(768)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
