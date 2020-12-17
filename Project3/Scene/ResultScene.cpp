#include "ResultScene.h"
#include "LoginScene.h"
#include "CheckeredBlock.h"
#include "SceneMng.h"
#include "../NetWork/NetWork.h"
#include "../_debug/_DebugConOut.h"

ResultScene::ResultScene()
{
	Init();
}

ResultScene::~ResultScene()
{
}

uniqueBase ResultScene::Update(uniqueBase own, const Time& now)
{
	if (lpNetWork.GetRevResult())
	{
		data_ = lpNetWork.GetResult();
		lpNetWork.EndOfNetWork();
	}
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		lpNetWork.EndOfNetWork();
		return std::make_unique<CheckeredBlock>(std::move(own), std::make_unique<LoginScene>());
	}
	DrawOwnScene();
	return std::move(own);
}

void ResultScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();
	Vector2 pos(390, 50);
	int cnt = 1;
	DrawFormatString(pos.x, pos.y, 0xffffff, "����");
	pos.x -= 20;
	pos.y += 50;
	for (auto data : data_)
	{
		if (data != -1)
		{
			DrawFormatString(pos.x, pos.y, 0xffffff, "%d�ʁF%d", cnt, data / 5 + 1);
		}
		else
		{
			DrawFormatString(pos.x, pos.y, 0xffffff, "%d�ʁF-", cnt);
		}
		pos.y += 50;
		cnt++;
	}
}

void ResultScene::Init(void)
{
	drawScreen_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, true);
	data_ = lpNetWork.GetResult();
}
