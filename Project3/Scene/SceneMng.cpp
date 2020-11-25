#include <time.h>
#include <DxLib.h>
#include <iostream>
#include "SceneMng.h"
#include "LoginScene.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"



std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::s_instance_(new SceneMng());

void SceneMng::Run()
{
	_dbgSetup(screenX, screenY, 255);

	activeScene_ = std::make_unique<LoginScene>();
	activeScene_->Init();
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_dbgStartDraw();
		nowTime_ = std::chrono::system_clock::now();
		activeScene_ = (*activeScene_).Update(std::move(activeScene_),nowTime_);
		Draw();
	}
	DxLib::DxLib_End();
}

Vector2 SceneMng::GetScreenSize(void)
{
	return { screenX,screenY };
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



SceneMng::SceneMng() :screenX(32 * 21), screenY(32 * 17)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
