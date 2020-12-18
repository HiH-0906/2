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
	befora_= std::chrono::system_clock::now();
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		now_ = std::chrono::system_clock::now();
		_dbgStartDraw();
		/*auto daluta = std::chrono::duration_cast<std::chrono::milliseconds>(now_ - befora_).count();*/

		activeScene_ = (*activeScene_).Update(std::move(activeScene_), now_);
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



SceneMng::SceneMng() :screenX(800), screenY(600)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
