#include <DxLib.h>
#include "SceneMng.h"

std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::S_instance(new SceneMng());

void SceneMng::Run()
{
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
	_playErea.emplace_back(std::make_unique<PleyErea>());
	return true;
}

SceneMng::SceneMng():screenX(800),screenY(600)
{
}

SceneMng::~SceneMng()
{
}
