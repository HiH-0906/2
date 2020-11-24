#include "Bomb.h"
#include "../common/ImageMng.h"
#include "../Scene/GameScene.h"

Bomb::Bomb(Vector2 pos, Vector2 size, int length,int id, int oid, std::shared_ptr<Map> mapMng, BaseScene& scene, std::chrono::system_clock::time_point start)
	: Obj(pos, size,id, 0, mapMng, scene),owner_(oid)
{
	lpImageMng.GetID("Bomb", "Image/bomb.png", size_, { 2,7 });
	start_ = start;
	length_ = length;
}

Bomb::~Bomb()
{
}

bool Bomb::UpdateDef(void)
{
	if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_).count() >= 3000)
	{
		auto player = dynamic_cast<GameScene&>(scene_).GetPlayer(owner_);
		dynamic_cast<Player&>(*player).StockBomb(id_);
		auto chip = mapMng_->ChengeChip(pos_);
		mapMng_->GeneratoFlame(chip, 3);
		alive_ = false;
	}
	
	
	return true;
}

void Bomb::Draw(void)
{
	DrawGraph(pos_.x, pos_.y , lpImageMng.GetID("Bomb")[0], true);
}
