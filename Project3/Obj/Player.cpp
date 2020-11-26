#include <DxLib.h>
#include <chrono>
#include "Player.h"
#include "../Scene/BaseScene.h"
#include "../Scene/GameScene.h"
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
	if (mode == NetWorkMode::OFFLINE)
	{
		if (id_ / UNIT_ID_BASE == 0)
		{
			Update_ = std::bind(&Player::UpdateDef, this,std::placeholders::_1);
			FuncInit();
		}
		else
		{
			Update_ = std::bind(&Player::UpdateAuto, this, std::placeholders::_1);
		}
	}
	else
	{
		int checkID = mode == NetWorkMode::HOST ? 0 : 1;
		if (id_ / UNIT_ID_BASE == checkID)
		{
			Update_ = std::bind(&Player::UpdateDef, this, std::placeholders::_1);
			FuncInit();
		}
		else
		{	
			if (((id_ / UNIT_ID_BASE) % 2) != checkID)
			{
				Update_ = std::bind(&Player::UpdataNet, this, std::placeholders::_1);
			}
			else
			{
				Update_ = std::bind(&Player::UpdateAuto, this, std::placeholders::_1);
			}
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
	pos_ += speedVec_[dir_];
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
		list.sort([](const shared_Obj& objA, const shared_Obj& objB) 
		{
			const auto& APos = objA->GetPos();
			const auto& BPos = objB->GetPos();
			return (APos.x * APos.x + APos.y * APos.y) > (BPos.x * BPos.x + BPos.y * BPos.y);
		}
		);
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
	lpNetWork.SendMes(MES_TYPE::POS, MesDataList{ data[0],data[1],data[2],data[3] });
	animCnt_++;
	const auto& chip = mapMng_->ChengeChip(pos_);
	if (mapMng_->CheckHitFlame(chip))
	{
		//data[0] = { static_cast<unsigned int>(id_) };
		//lpNetWork.SendMes(MES_TYPE::DETH, MesDataList{ data[0] });
		//dir_ = DIR::DETH;
		//state_ = AnimState::DETH;
		//activ_ = false;
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
			pos_ = Vector2{ static_cast<int>(data[1].uidata),static_cast<int>(data[2].uidata) };
			dir_ = static_cast<DIR>(data[3].uidata);
			test = true;
	 }
	 while (isPickMesList(MES_TYPE::SET_BOMB))
	 {
		 auto mes = PickUpMes(MES_TYPE::SET_BOMB);
		 uinonTimeData time = { std::chrono::system_clock::now() };
		 time.idata[0] = mes.second[5].idata;
		 time.idata[1] = mes.second[6].idata;
		 dynamic_cast<GameScene&>(scene_).SetBomb(Vector2{ mes.second[2].idata,mes.second[3].idata }, mes.second[1].idata, mes.second[0].idata, mes.second[4].idata, false, time.time);
	 }
	 if (!test)
	 {
		 TRACE("データ未受信ID:%d\n", id_);
		 fallCnt_++;
	 }
	animCnt_++;
	state_ = AnimState::WALK;
	if (isPickMesList(MES_TYPE::DETH))
	{
		dir_ = DIR::DETH;
		state_ = AnimState::DETH;
		activ_ = false;
	}
	return true;
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
		anim = static_cast<size_t>((animCnt_ / 15) % 4) * 5;
		dir = static_cast<size_t>(dir_);
		if (((animCnt_ / 15) % 4) == 0)
		{
			alive_ = false;
		}
	}
	DrawGraph(pos_.x, pos_.y - offSetY_, lpImageMng.GetID("player")[anim + state + dir], true);
	_dbgDrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, 0xffffff, false);
}

void Player::StockBomb(int id)
{
	if (!activ_)
	{
		return;
	}
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
	auto CheckHitObj = [&](const Vector2& APos, const Vector2& ASize, const shared_Obj objB,std::shared_ptr<Map> mapMng)
	{
		const auto& BPos = objB->GetPos();

		
		const auto& Bchip = mapMng->ChengeChip(BPos);

		return APos == Bchip;
	};
	
	moveFunc_.emplace_back([&](bool tmp, std::list<shared_Obj> list)
	{
		if (input_->GetKeySty(INPUT_ID::DOWN))
		{
			if (tmp)
			{
				dir_ = DIR::DOWN;
				if (!CheckHitWall(DIR::DOWN))
				{
					auto tmpPos = pos_ + speedVec_[DIR::DOWN];
					if (list.size() != 0)
					{
						auto Achip = mapMng_->ChengeChip(tmpPos);
						Achip.y++;
						const auto& BPos = list.front()->GetPos();
						if (CheckHitObj(Achip, size_, list.front(),mapMng_) && (tmpPos.y + size_.y < BPos.y))
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
					auto tmpPos = pos_ + speedVec_[DIR::LEFT];
					if (list.size() != 0)
					{
						auto Achip = mapMng_->ChengeChip(tmpPos);
						Achip.x--;
						const auto& BPos = list.front()->GetPos();
						const auto& BSize = list.front()->GetSize();
						if (CheckHitObj(Achip, size_, list.front(), mapMng_) && (tmpPos.x < BPos.x + BSize.x))
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
					auto tmpPos = pos_ + speedVec_[DIR::RIGHT];
					if (list.size() != 0)
					{
						auto Achip = mapMng_->ChengeChip(tmpPos);
						Achip.x++;
						const auto& BPos = list.front()->GetPos();
						if (CheckHitObj(Achip, size_, list.front(), mapMng_) && (tmpPos.x + size_.x < BPos.x))
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

					auto tmpPos = pos_ + speedVec_[DIR::UP];
					if (list.size() != 0)
					{
						auto Achip = mapMng_->ChengeChip(tmpPos);
						Achip.y++;
						const auto& BSize = list.front()->GetSize();
						const auto& BPos = list.front()->GetPos();
						if (CheckHitObj(Achip, size_, list.front(), mapMng_) && (tmpPos.y < BPos.y + BSize.y))
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
