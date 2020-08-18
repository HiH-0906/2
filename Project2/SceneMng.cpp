#include <time.h>
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "State/CON_ID.h"
#include "EffectMng.h"


std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::s_instance(new SceneMng());

void SceneMng::Run()
{
	_dbgSetup(screenX, screenY, 16);

	_activeScene = std::make_unique<TitleScene>();

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		/*_dbgStartDraw();
		bool gameFlag = true;
		for (auto&& erea:playErea_)
		{
			gameFlag &= erea->UpDate();
		}
		if (!gameFlag)
		{
			for (auto&& erea : playErea_)
			{
				erea->SetWinner(true);
			}
		}*/
		_activeScene = (*_activeScene).Update(std::move(_activeScene));
		lpEffectMng.Update();
		RunRensaQue();
		Draw();
	}
	DxLib::DxLib_End();
}

void SceneMng::AddRensaQue(RENSA_QUE&& que)
{
	rensaQue_.push_back(std::move(que));
}

const Vector2 SceneMng::screenSize(void) const
{
	return Vector2{ screenX,screenY };
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	_activeScene->Draw();
	_dbgAddDraw();
	ScreenFlip();
}

void SceneMng::RunRensaQue(void)
{
	_activeScene->RunRensaQue(rensaQue_);
	rensaQue_.clear();
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
	lpEffectMng.Init({ screenX ,screenY });

	return true;
}

SceneMng::SceneMng():screenX(1024),screenY(768)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
