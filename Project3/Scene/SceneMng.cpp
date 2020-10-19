#include <time.h>
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"


std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::s_instance_(new SceneMng());

void SceneMng::Run()
{
	_dbgSetup(screenX, screenY, 16);

	activeScene_ = std::make_unique<TitleScene>();
	activeScene_->Init();
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		activeScene_ = (*activeScene_).Update(std::move(activeScene_));
		Draw();
	}
	DxLib::DxLib_End();
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	activeScene_->Draw();
	_dbgAddDraw();
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
	SetAlwaysRunFlag(TRUE);
	return true;
}



SceneMng::SceneMng():screenX(800),screenY(600)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
