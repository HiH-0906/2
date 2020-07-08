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
	_playErea.emplace_back(std::make_unique<PleyErea>(std::move(Vector2{ 288,576 }), std::move(Vector2{ 96,96 }), std::move(PLAYER_ID::ONE)));
	_playErea.emplace_back(std::make_unique<PleyErea>(std::move(Vector2{ 288,576 }), std::move(Vector2{ screenX - (288 + 96),96 }), std::move(PLAYER_ID::SECOND)));
	return true;
}

SceneMng::SceneMng():screenX(1024),screenY(768)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
