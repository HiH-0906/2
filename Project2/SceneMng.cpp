#include <DxLib.h>
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "State/CON_ID.h"
#include "EffectMng.h"


std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::s_instance(new SceneMng());

void SceneMng::Run()
{
	_dbgSetup(screenX, screenY, 16);
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_dbgStartDraw();
		for (auto&& erea:playErea_)
		{
			erea->UpDate();
		}
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
	for (size_t i = 0; i < playErea_.size(); i++)
	{
		DrawGraph(static_cast<int>(i) * 512, 0, playErea_[i]->GetScreenID(), true);
	}
	lpEffectMng.Draw();
	_dbgAddDraw();
	ScreenFlip();
}

void SceneMng::RunRensaQue(void)
{
	int id, rensa, cnt;
	for (auto que : rensaQue_)
	{
		std::tie(id, rensa, cnt) = que;
		for (auto&& stage:playErea_)
		{
			if (stage->playerID() == id)
			{
				continue;
			}
			stage->ozyamaCnt(rensa);
		}
	}
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
	Vector2 size = { 512, 768 };
	Vector2 offset = { 100,64 };
	playErea_.emplace_back(std::make_unique<PleyErea>(std::move(size),std::move(offset),CON_ID::KEY));
	playErea_.emplace_back(std::make_unique<PleyErea>(std::move(size), std::move(offset),CON_ID::KEY));
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
