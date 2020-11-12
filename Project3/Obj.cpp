#include "Obj.h"

Obj::Obj(Vector2 pos, Vector2 size, int speed) :pos_(pos), size_(size), speed_(speed)
{
	Update_ = std::bind(&Obj::UpdateDef, this);
	offSetY_ = size_.y / 2;
	animCnt_ = 0;
}

bool Obj::UpdateDef(void)
{
	return false;
}
