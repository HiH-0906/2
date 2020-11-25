#include "Bomb.h"
#include "../common/ImageMng.h"
#include "../Scene/GameScene.h"

Bomb::Bomb(Vector2 pos, Vector2 size, int length,int id, int oid, std::shared_ptr<Map> mapMng, BaseScene& scene, std::chrono::system_clock::time_point start)
	: Obj(pos, size,id, 0, mapMng, scene),owner_(oid)
{
	lpImageMng.GetID("Bomb", "Image/bomb.png", size_, { 2,7 });
	start_ = start;
	explosion_ = false;
	elapsedTime_ = 0;
	explosionTime_ = 1000;
	defTime_ = 500;
	offset_ = 0;
	length_ = length;
}

Bomb::~Bomb()
{
}

bool Bomb::UpdateDef(const Time& now)
{
	elapsedTime_ = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count();
	const auto& chip = mapMng_->ChengeChip(pos_);
	if (mapMng_->CheckHitFlame(chip)&& !explosion_)
	{
		auto player = dynamic_cast<GameScene&>(scene_).GetPlayer(owner_);
		dynamic_cast<Player&>(*player).StockBomb(id_);
		auto chip = mapMng_->ChengeChip(pos_);
		mapMng_->GeneratoFlame(chip, length_, now);
		explosion_ = true;
	}
	if (elapsedTime_ >= 3000 && !explosion_)
	{
		auto player = dynamic_cast<GameScene&>(scene_).GetPlayer(owner_);
		dynamic_cast<Player&>(*player).StockBomb(id_);
		auto chip = mapMng_->ChengeChip(pos_);
		mapMng_->GeneratoFlame(chip, length_, now);
		explosion_ = true;
	}
	else if (elapsedTime_ >= 3000 + ((1000 / 6) * 7) || explosion_)
	{
		alive_ = false;
	}
	
	
	return true;
}

void Bomb::Draw(void)
{
	if (!explosion_)
	{
		if (explosionTime_ <= elapsedTime_)
		{
			if (offset_ == 0)
			{
				offset_ = 2;
			}
			else
			{
				offset_ = 0;
				defTime_ -= 130;
			}
			
			explosionTime_ += defTime_;
		}
	}
	else
	{
		offset_ = ((elapsedTime_ - 3000) / (1000 / 6)) * 2 + 1;
	}
	DrawGraph(pos_.x, pos_.y, lpImageMng.GetID("Bomb")[offset_], true);
}
