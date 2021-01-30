#include <iostream>
#include <algorithm>
#include <DxLib.h>
#include "GameScene.h"
#include "CrossOver.h"
#include "CheckeredBlock.h"
#include "ResultScene.h"
#include "SceneMng.h"
#include "../Obj/Player.h"
#include "../Obj/Bomb.h"
#include "../NetWork/NetWork.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

uniqueBase GameScene::Update(uniqueBase own, const Time& now)
{
	
	int check = INT_MAX;
	if (lpNetWork.GetMode() != NetWorkMode::OFFLINE)
	{
		check = std::count_if(objList_.begin(), objList_.end(), [](shared_Obj objA) {return objA->Alive(); });
	}
	else
	{
		if (!objList_.front()->Alive())
		{
			check = 1;
		}
		else
		{
			check = std::count_if(objList_.begin(), objList_.end(), [](shared_Obj objA) {return objA->Alive(); });
		}
	}

	if (mapMng_->GetFlameEnd() && check <= 1)
	{
		lpNetWork.SetPlayNow(false);
		if (CheckGameEnd_[lpNetWork.GetMode()]())
		{
			return std::make_unique<CheckeredBlock>(std::move(own), std::make_unique<ResultScene>());
		}
		else
		{
			return std::move(own);
		}
	}

	objList_.sort([](shared_Obj objA, shared_Obj objB)
	{
		return objA->CheckMesList() > objB->CheckMesList();
	}
	);
	
	if (state_ == GameState::PLAY)
	{
		for (auto& pl : objList_)
		{
			pl->Update_(now);
		}
	}
	auto delItr = std::remove_if(objList_.begin(), objList_.end(), [](shared_Obj& obj) {return !obj->Alive() && obj->GetTag() != OBJ_TAG::PLAYER; });
	objList_.erase(delItr, objList_.end());
	DrawOwnScene();
	DrawFps(now);
	mapMng_->Update(now);

	return own;
}

void GameScene::DrawOwnScene(void)
{
	const auto& size = mapMng_->GetMapSize();
	const auto& data = mapMng_->GetFlameData();
	const auto& chipSize = mapMng_->GetChipSize();
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();
	for (int i = 0; i < static_cast<int>(MapLayer::CHAR); i++)
	{
		DrawGraph(0, 0, mapMng_->GetDarwMap(static_cast<MapLayer>(i)), true);
	}
	mapMng_->DrawFlame();
	for (auto& pl : objList_)
	{
		pl->Draw();
	}
	cntDownFunc_[state_]();
}

void GameScene::Init(void)
{
	if (lpNetWork.GetActive() == ACTIVE_STATE::OFFLINE)
	{
		state_ = GameState::PLAY;
	}
	else
	{
		state_ = GameState::STY;
	}
	mapMng_ = std::make_shared<Map>();
	const auto& size = lpSceneMng.GetScreenSize();
	drawScreen_ = MakeScreen(size.x, size.y, true);
	const auto& mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
	{

		mapMng_->LoadMap("mapData/map.tmx");
		const auto& cLayer = mapMng_->GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		int max = lpNetWork.GetMax() + 1;
		const auto& chip = mapMng_->GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,32 }, Vector2{ 32,51 }, 2, id, mapMng_, *this));
				id += UNIT_ID_BASE;
			}
			cnt++;

			if ((id / UNIT_ID_BASE) == max)
			{
				break;
			}
		}
	}
	else
	{
		mapMng_->LoadMap("Capture/test.tmx");
		drawScreen_ = MakeScreen(size.x, size.y, true);
		const auto& cLayer = mapMng_->GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		int max = lpNetWork.GetMax();
		const auto& chip = mapMng_->GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,32 }, Vector2{ 32,51 }, 2, id, mapMng_, *this));
				id += UNIT_ID_BASE;
			}
			cnt++;

			if ((id / UNIT_ID_BASE) == max)
			{
				break;
			}
		}
	}
	lpImageMng.GetID("fire", "Image/fire.png", Vector2{ 32,32 }, Vector2{ 3,4 });
	initFunc();
	DrawOwnScene();
}

GameScene::GameScene()
{
	TRACE("ゲームシーン\n");
	Init();
}

GameScene::~GameScene()
{
	objList_.clear();
}

const GameState& GameScene::GetGameState(void) const
{
	return state_;
}

void GameScene::SetDethPlayerID(const int& id)
{
	dethPlayerID_.push_front(id);
}

shared_Obj GameScene::GetPlayer(int id)
{
	shared_Obj tmp = nullptr;
	for (auto& obj : objList_)
	{
		if (obj->GetID() == id)
		{
			tmp = obj;
			break;
		}
	}
	return tmp;
}

std::list<shared_Obj> GameScene::GetObjList(void)
{
	return objList_;
}

void GameScene::SetBomb(Vector2 pos, int& id, int& oid,  int length ,bool send, std::chrono::system_clock::time_point start)
{
	unionTimeData time = { start };
	auto chip = mapMng_->ChengeChip(pos);
	const auto& chipSize = mapMng_->GetChipSize();
	chip = chip * chipSize;
	if (send)
	{
		sendData data[7];
		data[0].uidata = oid;
		data[1].uidata = id;
		data[2].uidata = chip.x;
		data[3].uidata = chip.y;
		data[4].uidata = 3;
		data[5].uidata = time.idata[0];
		data[6].uidata = time.idata[1];
		
		lpNetWork.SendMesAll(MES_TYPE::SET_BOMB, { data[0],data[1],data[2],data[3],data[4],data[5],data[6] });
	}
	objList_.emplace_back(std::make_unique<Bomb>(chip, Vector2{ 32,32 }, length, id, oid, mapMng_, *this, start));
}

void GameScene::initFunc(void)
{
	cntDownFunc_.try_emplace(GameState::STY, [&]()
	{
		DrawFormatString(300, 500, 0xffffff, "待機中");
		if (lpNetWork.GetStartGame())
		{
			state_ = GameState::COUNT;
		}
	});
	cntDownFunc_.try_emplace(GameState::COUNT, [&]()
	{
		const auto& count = lpNetWork.GetCountDownGameTime();
		const auto& now = std::chrono::system_clock::now();
		auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(now - count).count();

		cnt = abs(START_CNT - cnt);
		DrawFormatString(300, 500, 0xffffff, "開始まであと：%d秒", cnt / 1000);
		if (cnt / 1000 <= 0)
		{
			lpNetWork.SetPlayNow(true);
			state_ = GameState::PLAY;
		}
	});
	cntDownFunc_.try_emplace(GameState::PLAY, [&]()
	{


	});

	CheckGameEnd_.try_emplace(NetWorkMode::NON, [&]()
	{
		// ここに入ってくるのは切断時なのでとりあえず表示用のデータ作成
		// 今までの奴は破棄 全部ハイフン
		dethPlayerID_.clear();
		for (int i = 0; i < 5; i++)
		{
			dethPlayerID_.push_back(-1);
		}
		lpNetWork.SendResult(dethPlayerID_);
		return true;
	});
	CheckGameEnd_.try_emplace(NetWorkMode::GEST, [&]()
	{
		if (lpNetWork.GetResult().size() == 0)
		{
			TRACE("MESマダー\n");
			return false;
		}
		return true;
	});
	CheckGameEnd_.try_emplace(NetWorkMode::HOST, [&]()
	{
		for (const auto& obj : objList_)
		{
			if (obj->Alive())
			{
				dethPlayerID_.push_front(obj->GetID());
				break;
			}
		}
		lpNetWork.SendResult(dethPlayerID_);
		return true;
	});
	CheckGameEnd_.try_emplace(NetWorkMode::OFFLINE, [&]()
	{
		// 自分がAUTOより先に死んだ場合用の生きてるやつ追加 IDが一番大きいやつが1位になる模様
		for (const auto& obj : objList_)
		{
			if (obj->Alive())
			{
				dethPlayerID_.push_front(obj->GetID());
			}
		}
		lpNetWork.SendResult(dethPlayerID_);
		return true;
	});

	CheckGameEnd_.try_emplace(NetWorkMode::MAX, [&]()
	{
		return true;
	});
}
