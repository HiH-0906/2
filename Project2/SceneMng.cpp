#include <DxLib.h>
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::S_instance(new SceneMng());

void SceneMng::Run()
{
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		//_dbgStartDraw();
		for (int i = 0; i < _playErea.size(); i++)
		{
			_playErea[i]->UpDate();
		}
		Draw();
	}
	DxLib::DxLib_End();
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	for (int i = 0; i < _playErea.size(); i++)
	{
		DrawGraph(_playErea[i]->offset().x, _playErea[i]->offset().y, _playErea[i]->GetScreenID(), true);
	}

	ScreenFlip();
}

bool SceneMng::SysInit(void)
{
	ChangeWindowMode(true);
	SetWindowText("1916035_‹´–{‘å‹P");
	SetGraphMode(screenX, screenY, 16);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	Vector2 size = Vector2{ 288,576 };
	Vector2 offset = Vector2{ 0,0 };
	_playErea.emplace_back(std::make_unique<PleyErea>(std::move(size), std::move(offset)));
	size = Vector2{ 288,576 };
	offset = Vector2{ screenX - 288,0 };
	_playErea.emplace_back(std::make_unique<PleyErea>(std::move(size), std::move(offset)));
	return true;
}

SceneMng::SceneMng():screenX(800),screenY(600)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
