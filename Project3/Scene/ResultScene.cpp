#include "ResultScene.h"
#include "LoginScene.h"
#include "CheckeredBlock.h"
#include "SceneMng.h"
#include "../NetWork/NetWork.h"
#include "../_debug/_DebugConOut.h"

ResultScene::ResultScene()
{
	Init();
	DrawOwnScene();
}

ResultScene::~ResultScene()
{
}

uniqueBase ResultScene::Update(uniqueBase own, const Time& now)
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
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
	int cnt = 0;
	DrawFormatString(pos.x, pos.y, 0xffffff, "����");
	pos.x -= 20;
	pos.y += 50;
	for (auto data : data_)
	{
		cnt++;
		if (data != -1)
		{
			DrawFormatString(pos.x, pos.y, 0xffffff, "%d�ʁF%d", cnt, data / 5 + 1);
		}
		else
		{
			DrawFormatString(pos.x, pos.y, 0xffffff, "%d�ʁF-", cnt);
		}
		pos.y += 50;
	}
}

void ResultScene::Init(void)
{
	drawScreen_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, true);
	data_ = lpNetWork.GetResult();
	lpNetWork.EndOfNetWork();
}
