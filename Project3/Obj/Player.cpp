#include <DxLib.h>
#include <chrono>
#include "Player.h"
#include "../Scene/BaseScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/SceneMng.h"
#include "../NetWork/NetWork.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../Input/PadState.h"
#include "../Input/keyState.h"
#include"../map/Map.h"

int Player::fallCnt_ = 0;

Player::Player(Vector2 pos, Vector2 size,Vector2 ImageSize, int speed,int id, std::shared_ptr<Map> mapMng, BaseScene& scene): Obj(pos,size,id,speed,mapMng,scene)
{
	lpImageMng.GetID("player", "Image/bomberman.png", { ImageSize.x,ImageSize.y }, { 5,4 });
	dir_ = DIR::DOWN;
	state_ = AnimState::IDEL;
	tag_ = OBJ_TAG::PLAYER;
	offSetY_ = ImageSize.y / 2;
	length_ = 3;
	chipSize_ = mapMng_->GetChipSize();
	input_ = std::make_unique<keyState>();
	input_->Setting(0, DX_INPUT_PAD1);

	auto mode = lpNetWork.GetMode();
	idScreen_ = MakeScreen(size.x, size.y + 20, true);
	SetDrawScreen(idScreen_);
	auto drawID = id_ / UNIT_ID_BASE + 1;
	if (mode == NetWorkMode::OFFLINE)
	{	
		if (id_ == 0)
		{
			DrawFormatString(14, 1, 0, "%d", drawID);
			DrawFormatString(13, 0, 0xff2222, "%d", drawID);
			Update_ = std::bind(&Player::UpdateDef, this, std::placeholders::_1);
			FuncInit();
		}
		else
		{
			DrawFormatString(14, 1, 0, "%d", drawID);
			DrawFormatString(13, 0, 0xffffff, "%d", drawID);
			Update_ = std::bind(&Player::UpdateAuto, this, std::placeholders::_1);
		}
	}
	else
	{
		if (id_ == lpNetWork.GetID())
		{
			DrawFormatString(14, 1, 0, "%d", drawID);
			DrawFormatString(13, 0, 0xff2222, "%d", drawID);
			Update_ = std::bind(&Player::UpdateDef, this, std::placeholders::_1);
			FuncInit();
		}
		else
		{	
			DrawFormatString(14, 1, 0, "%d", drawID);
			DrawFormatString(13, 0, 0xffffff, "%d", drawID);
			Update_ = std::bind(&Player::UpdataNet, this, std::placeholders::_1);
		}
	}
	revList_.reserve(100);

	speedVec_.try_emplace(DIR::DOWN, Vector2{ 0,speed });
	speedVec_.try_emplace(DIR::UP, Vector2{ 0,-speed });
	speedVec_.try_emplace(DIR::LEFT, Vector2{ -speed,0 });
	speedVec_.try_emplace(DIR::RIGHT, Vector2{ speed,0 });

	for (int bomb = 1; bomb <= 4; bomb++)
	{
		StockBomb(id_ + bomb);
	}
	lpNetWork.SetObjRevData(id_, mtx_, revList_);
}

Player::~Player()
{
}

bool Player::UpdateAuto(const Time& now)
{
	if (!activ_)
	{
		animCnt_++;
		return true;
	}
	std::list<shared_Obj> list = GetHitObjList();
	auto NextDir = [&](DIR dir)
	{
		if (dir == DIR::DOWN)
		{
			return DIR::LEFT;
		}
		if (dir == DIR::LEFT)
		{
			return DIR::UP;
		}
		if (dir == DIR::UP)
		{
			return DIR::RIGHT;
		}
		if (dir == DIR::RIGHT)
		{
			return DIR::DOWN;
		}
		return begin(DIR());
	};
	auto dir = dir_;
	if (((pos_.x % chipSize_.x) == 0) && ((pos_.y % chipSize_.y) == 0))
	{
		while (CheckHitWall(dir))
		{
			dir = NextDir(dir);
		}
	}
	dir_ = dir;
	if (list.size() != 0)
	{
		const auto& pos = list.front()->GetPos();
		if (!CheckHitObj(list.front(), dir_))
		{
			pos_ += speedVec_[dir_];
		}
		else
		{
			dir_ = NextDir(dir_);
		}
	}
	else
	{
		pos_ += speedVec_[dir_];
	}
	data[0] = { static_cast<unsigned int>(id_) };
	data[1] = { static_cast<unsigned int>(pos_.x) };
	data[2] = { static_cast<unsigned int>(pos_.y) };
	data[3] = { static_cast<unsigned int>(dir_) };
	lpNetWork.SendMes(MES_TYPE::POS, MesDataList{ data[0],data[1],data[2],data[3] });
	animCnt_++;
	state_ = AnimState::WALK;
	const auto& chip = mapMng_->ChengeChip(pos_);
	if (mapMng_->CheckHitFlame(chip))
	{
		data[0] = { static_cast<unsigned int>(id_) };
		lpNetWork.SendMes(MES_TYPE::DETH, MesDataList{ data[0] });
		dir_ = DIR::DETH;
		state_ = AnimState::DETH;
		activ_ = false;
		auto game = dynamic_cast<GameScene&>(scene_);
		game.SetDethPlayerID(id_);
	}
	return true;
}

bool Player::UpdateDef(const Time& now)
{
	if (!activ_)
	{
		animCnt_++;
		return true;
	}
	std::list<shared_Obj> list = GetHitObjList();

	if (list.size() != 0)
	{
		
		int i = 0;
		for (auto& tmp : list)
		{
			const auto& pos = tmp->GetPos();
			_dbgDrawFormatString(pos.x, pos.y, 0xff0000, "%d", i);
			i++;
		}
	}

	input_->Update();
	state_ = AnimState::IDEL;
	for (auto movePrg = moveFunc_.begin(); movePrg != moveFunc_.end(); movePrg++)
	{
		if ((*movePrg)(true,list))
		{
			moveFunc_.splice(moveFunc_.begin(), moveFunc_, movePrg);
			moveFunc_.sort([&](moveFunc funcA, moveFunc funcB) {return funcA(false,list) < funcB(false,list); });
			state_ = AnimState::WALK;
			break;
		}
	}

	if (input_->GetKeyTrg(INPUT_ID::LROTA))
	{
		auto id = UseBomb();
		if (id >= 0)
		{
			dynamic_cast<GameScene&>(scene_).SetBomb(pos_, id, id_, length_, true, now);
		}
	}
	data[0] = { static_cast<unsigned int>(id_) };
	data[1] = { static_cast<unsigned int>(pos_.x) };
	data[2] = { static_cast<unsigned int>(pos_.y) };
	data[3] = { static_cast<unsigned int>(dir_) };
	lpNetWork.SendMesAll(MES_TYPE::POS, MesDataList{ data[0],data[1],data[2],data[3] });
	animCnt_++;
	const auto& chip = mapMng_->ChengeChip(pos_);
	if (mapMng_->CheckHitFlame(chip))
	{
		data[0] = { static_cast<unsigned int>(id_) };
		lpNetWork.SendMesAll(MES_TYPE::DETH, MesDataList{ data[0] });
		dir_ = DIR::DETH;
		state_ = AnimState::DETH;
		activ_ = false;
		auto game = dynamic_cast<GameScene&>(scene_);
		game.SetDethPlayerID(id_);
	}
	return true;
}

bool Player::UpdataNet(const Time& now)
{
	if (!activ_)
	{
		animCnt_++;
		return false;
	}
	bool test = false;
	 while (isPickMesList(MES_TYPE::POS))
	 {
		 auto mes = PickUpMes(MES_TYPE::POS);
		 auto& data = mes.second;
		 if (data[3].uidata >= 0 && data[3].uidata < static_cast<unsigned int>(DIR::MAX))
		 {
			 dir_ = static_cast<DIR>(data[3].uidata);
		 }
		 else
		 {
			 TRACE("DIR異常	ID：%d\n", id_);
		 }
		 auto pos = Vector2{ static_cast<int>(data[1].uidata),static_cast<int>(data[2].uidata) };
		 if (pos >= Vector2(0, 0) && pos < lpSceneMng.GetScreenSize())
		 {
			 pos_ = pos;
		 }
		 else
		 {
			 TRACE("画面外pos(%d,%d) ID:%d", pos.x, pos.y, id_);
		 }
		 
		 test = true;
	 }
	 while (isPickMesList(MES_TYPE::SET_BOMB))
	 {
		 auto checkBombPos = [](const Vector2&player, const Vector2& bomb)
		 {
			 for (int x = -1; x < 2; x++)
			 {
				 for (int y = -1; y < 2; y++)
				 {
					 if (Vector2{ player.x + x,player.y + y } == bomb)
					 {
						 return true;
					 }
				 }
			 }
			 return false;
		 };
		 auto mes = PickUpMes(MES_TYPE::SET_BOMB);
		 auto pos = Vector2{ mes.second[2].idata,mes.second[3].idata };
		 unionTimeData time = { std::chrono::system_clock::now() };
		 if (mes.second[0].idata != id_)
		 {
			 TRACE("ボムonwerID異常%d\n", mes.second[0].idata);
			 break;
		 }
		 if (mes.second[1].idata < id_ || mes.second[1].idata > id_ + 4)
		 {
			 TRACE("ボムID異常%d\n", mes.second[0].idata);
			 break;
		 }
		 if (mes.second[4].idata != length_)
		 {
			 TRACE("ボムのlength異常：%d\n", mes.second[4].idata);
			 break;
		 }
		 if (!checkBombPos(mapMng_->ChengeChip(pos_), mapMng_->ChengeChip(pos)))
		 {
			 TRACE("ボムがプレイヤーの周り以外から生成されようとしている	ID：%d\n", id_);
			 //break;
		 }
		 time.idata[0] = mes.second[5].idata;
		 time.idata[1] = mes.second[6].idata;
		 dynamic_cast<GameScene&>(scene_).SetBomb(pos, mes.second[1].idata, mes.second[0].idata, mes.second[4].idata, false, time.time);
	 }
	 if (!test)
	 {
		 //TRACE("データ未受信ID:%d\n", id_);
		 fallCnt_++;
	 }
	animCnt_++;
	state_ = AnimState::WALK;
	if (isPickMesList(MES_TYPE::DETH) || isPickMesList(MES_TYPE::LOST))
	{
		dir_ = DIR::DETH;
		state_ = AnimState::DETH;
		activ_ = false;
		auto game = dynamic_cast<GameScene&>(scene_);
		game.SetDethPlayerID(id_);
	}
	return true;
}

bool Player::CheckHitObj(const shared_Obj& objB,DIR dir)
{
	if ((pos_.x % 32 == 0) && (pos_.y % 32 == 0))
	{
		auto APos = mapMng_->ChengeChip(pos_);
		APos += +speedVec_[dir] / speed_;
		const auto& BPos = objB->GetPos();
		const auto& Bchip = mapMng_->ChengeChip(BPos);

		return (APos == Bchip);
	}
	return false;
}

std::list<shared_Obj> Player::GetHitObjList(void)
{
	std::list<shared_Obj> list;
	for (auto& obj : dynamic_cast<GameScene&>(scene_).GetObjList())
	{
		if (obj->GetTag() != OBJ_TAG::PLAYER)
		{
			list.emplace_back(obj);
		}
	}
	if (list.size() > 1)
	{
		list.sort([&](const shared_Obj& objA, const shared_Obj& objB)
		{
			const auto& APos = objA->GetPos() - pos_;
			const auto& BPos = objB->GetPos() - pos_;
			if (APos.x == 0 && APos.y == 0)
			{
				return false;
			}
			return (APos.x * APos.x + APos.y * APos.y) < (BPos.x * BPos.x + BPos.y * BPos.y);
		}
		);
	}
	return std::move(list);
}

void Player::Draw(void)
{
	size_t anim = 0;
	size_t state = 0;
	size_t dir = 0;
	if (state_ != AnimState::DETH)
	{
		anim = static_cast<size_t>((animCnt_ / 15) % 2) * 5;
		state = (static_cast<size_t>(state_) * 10);
		dir = static_cast<size_t>(dir_);
	}
	else
	{
		if (((animCnt_ / 15) % 4) == 0)
		{
			alive_ = false;
		}
		return;
	}
	DrawGraph(pos_.x, pos_.y - offSetY_ - 20, idScreen_, true);
	DrawGraph(pos_.x, pos_.y - offSetY_, lpImageMng.GetID("player")[anim + state + dir], true);
}

void Player::StockBomb(int id)
{
	bombList_.emplace_back(id);
}

int Player::UseBomb(void)
{
	int id = -1;
	if (bombList_.size() != 0)
	{
		id = *bombList_.begin();
		bombList_.erase(bombList_.begin());
	}
	return id;
}

void Player::FuncInit(void)
{
	moveFunc_.emplace_back([&](bool tmp, std::list<shared_Obj> list)
	{
		if (input_->GetKeySty(INPUT_ID::DOWN))
		{
			if (tmp)
			{
				dir_ = DIR::DOWN;
				if (!CheckHitWall(DIR::DOWN))
				{
					if (list.size() != 0)
					{
						if (CheckHitObj(list.front(),DIR::DOWN))
						{
							return false;
						}
					}
					pos_ += speedVec_[DIR::DOWN];
					if (pos_.x % (size_.x) != 0)
					{
						pos_.x = ((pos_.x + (size_.x / 2)) / chipSize_.x) * chipSize_.x;
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
	moveFunc_.emplace_back([&](bool tmp, std::list<shared_Obj> list)
	{
		if (input_->GetKeySty(INPUT_ID::LEFT))
		{
			if (tmp)
			{
				dir_ = DIR::LEFT;
				if (!CheckHitWall(DIR::LEFT))
				{
					if (list.size() != 0)
					{
						if (CheckHitObj(list.front(), DIR::LEFT))
						{
							return false;
						}
					}
					pos_ += speedVec_[DIR::LEFT];
					if (pos_.y % (32) != 0)
					{
						pos_.y = ((pos_.y + (size_.y / 2)) / chipSize_.y) * chipSize_.y;
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
	moveFunc_.emplace_back([&](bool tmp, std::list<shared_Obj> list)
	{
		if (input_->GetKeySty(INPUT_ID::RIGHT))
		{
			if (tmp)
			{
				dir_ = DIR::RIGHT;
				if (!CheckHitWall(DIR::RIGHT))
				{
					if (list.size() != 0)
					{
						if (CheckHitObj(list.front(), DIR::RIGHT))
						{
							return false;
						}
					}
					pos_ += speedVec_[DIR::RIGHT];
					if (pos_.y % (32) != 0)
					{
						pos_.y = ((pos_.y + (size_.y / 2)) / chipSize_.y) * chipSize_.y;
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
	moveFunc_.emplace_back([&](bool tmp, std::list<shared_Obj> list)
	{
		if (input_->GetKeySty(INPUT_ID::UP))
		{
			if (tmp)
			{
				dir_ = DIR::UP;
				if (!CheckHitWall(DIR::UP))
				{
					if (list.size() != 0)
					{
						if (CheckHitObj(list.front(), DIR::UP))
						{
							return false;
						}
					}
					pos_ += speedVec_[DIR::UP];
					if (pos_.x % (size_.x) != 0)
					{
						pos_.x = ((pos_.x + (size_.x / 2)) / chipSize_.x) * chipSize_.x;
					}
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
}
