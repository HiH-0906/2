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

	activeScene_ = std::make_unique<GameScene>();

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		activeScene_ = (*activeScene_).Update(std::move(activeScene_));
		lpEffectMng.Update();
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

	Vector2 pos;
	int data;
	SCREEN_ID id;
	std::pair<int, Vector2> tmp;
	for (auto que : drawList_)
	{
		std::tie(pos, data, id) = que;
		tmp = { data,pos };
		DrawMap[id].emplace_back(tmp);
	}
	auto List = lpEffectMng.GeteffectList();
	for (auto que : List)
	{
		EffectMap[que.second].emplace_back(que.first);
	}
	for (auto id:SCREEN_ID())
	{

	}
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
	lpEffectMng.Init({ screenX ,screenY });
	return true;
}

void SceneMng::AddDrawList(DrawQueT&& que)
{
	drawList_.emplace_back(que);
}

SceneMng::SceneMng():screenX(1024),screenY(768)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
