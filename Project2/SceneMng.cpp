#include <DxLib.h>
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::S_instance(new SceneMng());

void SceneMng::Run()
{
	//_dbgStartDraw();
	_playErea[0]->UpDate();
	Draw();
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	DrawGraph(0, 0, _playErea[0]->GetScreenID(), true);

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
	//_dbgSetup(screenX, screenY, 128);
	_playErea.emplace_back(std::make_unique<PlayErea>(Vector2{0,0}, Vector2{ 288,576 }));
	return true;
}

SceneMng::SceneMng():screenX(800),screenY(600)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
