#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "../ImageMng.h"
#include "../SoundMng.h"
#include "../Input/keyState.h"
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../_debug/_DebugConOut.h"

SelectScene::SelectScene(void)
{
	for (int i = 0; i < 2; i++)
	{
		idVec_.emplace_back(CON_ID::KEY);
		start_.emplace_back(false);
	}
	lpImageMng.GetID("きー1", "image/KeyPL1.png");
	lpImageMng.GetID("きー2", "image/KeyPL2.png");
	lpImageMng.GetID("ぱっど", "image/pad.png");
	lpImageMng.GetID("まうす", "image/mouse.png");
	lpImageMng.GetID("まうす2", "image/mousePL2.png");
	lpImageMng.GetID("準備完了", "image/完了.png");
	lpImageMng.GetID("SelectBG", "image/SelectBG.png");
	lpImageMng.GetID("強調", "image/強調.png");
	imageKey_[0].try_emplace(CON_ID::KEY, "きー1");
	imageKey_[0].try_emplace(CON_ID::PAD, "ぱっど");
	imageKey_[0].try_emplace(CON_ID::MOUSE, "まうす");
	imageKey_[1].try_emplace(CON_ID::KEY, "きー2");
	imageKey_[1].try_emplace(CON_ID::PAD, "ぱっど");
	imageKey_[1].try_emplace(CON_ID::MOUSE, "まうす2");
	for (int i = 0; i < input_.size(); i++)
	{
		input_[i] = std::make_unique<keyState>();
		input_[i]->Setting(i, i);
		padNum_[i] = 0;
	}
	game_ = false;
	FadeInit(false);
}

SelectScene::~SelectScene()
{
}

unipueBase SelectScene::Update(unipueBase own)
{
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	// 現在選択中操作方法確認用
	lpSceneMng.AddDrawList({ {256,384},IMAGE_ID(imageKey_[0][idVec_[0]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ {768,384},IMAGE_ID(imageKey_[1][idVec_[1]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	if (!FadeUpdate())
	{
		if (game_)
		{
			for (int i = 0; i < start_.size(); i++)
			{
				if (start_[i])
				{
					lpSceneMng.AddDrawList({ { 256 + (512 * i),384 }, IMAGE_ID("準備完了")[0], 1.0, 0.0, 1, SCREEN_ID::FRONT, DATA_TYPE::IMG, true });
					lpSceneMng.AddDrawList({ { 256 + (512 * i),384 }, IMAGE_ID("SelectBG")[0], 1.0, 0.0, 1, SCREEN_ID::PLAY, DATA_TYPE::IMG, true });
				}
			}
			// 操作方法通知
			playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 0,0 }, idVec_[0]));
			playErea_[0]->padNum( padNum_[0]);
			playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 512, 0 }, idVec_[1]));
			playErea_[1]->padNum(padNum_[1]);
			return std::make_unique<GameScene>(std::move(playErea_));
		}
		game_ = Setting();
		for (int i = 0; i < start_.size(); i++)
		{
			if (start_[i])
			{
				lpSceneMng.AddDrawList({ { 256 + (512 * i),384 }, IMAGE_ID("準備完了")[0], 1.0, 0.0, 1, SCREEN_ID::FRONT, DATA_TYPE::IMG, true });
				lpSceneMng.AddDrawList({ { 256 + (512 * i),384 }, IMAGE_ID("SelectBG")[0], 1.0, 0.0, 1, SCREEN_ID::PLAY, DATA_TYPE::IMG, true });
			}
		}
	}
	
	return std::move(own);
}

bool SelectScene::Setting(void)
{
	// 接続されていない場合の待機
	if (lpSceneMng.fCnt() / 60 % 2)
	{
		ReSetupJoypad();
	}
	// ﾌﾟﾚｲﾔｰ人数分回す
	for (int i = 0; i < 2; i++)
	{
		// そのﾌﾟﾚｲﾔｰが決定状態なら更新しない
		if (start_[i])
		{
			continue;
		};
		// Inputのｱｯﾌﾟﾃﾞｰﾄ更新
		input_[i]->Update();
		// 変えやすいようにいったんintに
		int tmpid = static_cast<int>(idVec_[i]);
		// 祖須佐方法選択はｷｰﾎﾞｰﾄﾞで
		if (input_[i]->GetKeyTrg(INPUT_ID::LEFT))
		{
			--tmpid;
			if (tmpid < 0)
			{
				tmpid = 2;
			}
			TRACE("PL%d:%d\n", i + 1, tmpid);
		}
		if (input_[i]->GetKeyTrg(INPUT_ID::RIGHT))
		{
			++tmpid;
			if (tmpid >= 3)
			{
				tmpid = 0;
			}
			TRACE("PL%d:%d\n", i + 1, tmpid);
		}
		idVec_[i] = static_cast<CON_ID>(tmpid);
		// 選択された操作方法がﾊﾟｯﾄﾞの場合どのｺﾝﾄﾛｰﾗｰが誰なのか識別する必要がああるので分岐
		if (idVec_[i] == CON_ID::PAD)
		{
			for (int j = 1; j <= GetJoypadNum(); j++)
			{
				// 二人ともﾊﾟｯﾄﾞの場合先に押した人が1Pになっちゃう
				if ((GetJoypadInputState(j) & PAD_INPUT_2))
				{
					// ほかの人と被ってないか 現状だと二人ﾌﾟﾚｲ想定
					if (padNum_[i ^ 1] != j)
					{
						PlaySoundMem(lpSoundMng.GetHandle("太鼓"), DX_PLAYTYPE_BACK);
						padNum_[i] = j;
						start_[i] = true;
					}
					break;
				}
			}
		}
		else if (idVec_[i] == CON_ID::MOUSE)
		{
			if (idVec_[i ^ 1] == CON_ID::MOUSE)
			{
				return false;
			}
		}
		else
		{
			// ｷｰﾎﾞｰﾄﾞの場合何もしない
		}
		if (input_[i]->GetKeyTrg(INPUT_ID::RROTA))
		{
			PlaySoundMem(lpSoundMng.GetHandle("太鼓"), DX_PLAYTYPE_BACK);
			start_[i] = true;
		}
	}
	// 二人とも準備完了でtrue
	return start_[0] && start_[1];
}
