#include "SelectScene.h"
#include "SceneMng.h"
#include "GameScene.h"
#include "../ImageMng.h"
#include "../Input/keyState.h"
#include "../Input/PadState.h"
#include "../Input/MouseState.h"
#include "../_debug/_DebugConOut.h"

SelectScene::SelectScene(std::vector<std::shared_ptr<PleyErea>>&& playErea)
{
	playErea_ = playErea;
	for (int i = 0; i < playErea_.size(); i++)
	{
		idVec_.emplace_back(CON_ID::KEY);
		start_.emplace_back(false);
	}
	lpImageMng.GetID("きー", "image/key.png");
	lpImageMng.GetID("ぱっど", "image/pad.png");
	lpImageMng.GetID("まうす", "image/mouse.png");
	imageKey_.try_emplace(CON_ID::KEY, "きー");
	imageKey_.try_emplace(CON_ID::PAD, "ぱっど");
	imageKey_.try_emplace(CON_ID::MOUSE, "まうす");
	FadeInit(false);
}

SelectScene::~SelectScene()
{
}

unipueBase SelectScene::Update(unipueBase own)
{
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	if (!FadeUpdate())
	{
		if (Setting())
		{
			// 操作方法通知
			playErea_[0]->inputID(std::move(idVec_[0]));
			playErea_[1]->inputID(std::move(idVec_[1]));
			return std::make_unique<GameScene>(std::move(playErea_));
		}
	}
	// 現在選択中操作方法確認用
	lpSceneMng.AddDrawList({ {128,128},IMAGE_ID(imageKey_[idVec_[0]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ {630,128},IMAGE_ID(imageKey_[idVec_[1]])[0],1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	return std::move(own);
}

bool SelectScene::Setting(void)
{
	// ﾌﾟﾚｲﾔｰ人数分回す
	for (int i = 0; i < playErea_.size(); i++)
	{
		// そのﾌﾟﾚｲﾔｰが決定状態なら更新しない
		if (start_[i])
		{
			continue;
		};
		// Inputのｱｯﾌﾟﾃﾞｰﾄ更新
		for (auto id : CON_ID())
		{
			(*playErea_[i]->GetInput()[id])->Update();
			// 変えやすいようにいったんintに
			int tmpid = static_cast<int>(idVec_[i]);
			// 祖須佐方法選択はｷｰﾎﾞｰﾄﾞで
			if ((*playErea_[i]->GetInput()[id])->GetKeyTrg(INPUT_ID::LEFT))
			{
				--tmpid;
				if (tmpid < 0)
				{
					tmpid = 2;
				}
				TRACE("PL%d:%d\n", i + 1, tmpid);
			}
			if ((*playErea_[i]->GetInput()[id])->GetKeyTrg(INPUT_ID::RIGHT))
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
				// 接続されていない場合の待機
				if (lpSceneMng.fCnt() / 30 % 2)
				{
					ReSetupJoypad();
				}
				for (int j = 1; j <= GetJoypadNum(); j++)
				{
					// 二人ともﾊﾟｯﾄﾞの場合先に押した人が1Pになっちゃう
					if ((GetJoypadInputState(j) & PAD_INPUT_1))
					{
						// ほかの人と被ってないか 現状だと二人ﾌﾟﾚｲ想定
						if (playErea_[i ^ 1]->padNum() != j)
						{
							playErea_[i]->padNum(j);
							start_[i] = true;
						}
						break;
					}
				}
			}
			else if(idVec_[i] == CON_ID::MOUSE)
			{
				if (idVec_[i ^ 1] == CON_ID::MOUSE)
				{
					return false;
				}
				// 選ばれているｺﾝﾄﾛｰﾗｰのﾎﾞﾀﾝで決定
				if ((*playErea_[i]->GetInput()[id])->GetKeyTrg(INPUT_ID::LROTA))
				{
					start_[i] = true;
				}
			}
		}
	}
	// 二人とも準備完了でtrue
	return start_[0] && start_[1];
}
