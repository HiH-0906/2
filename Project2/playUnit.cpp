#include "playUnit.h"
#include "PleyErea.h"

playUnit::playUnit(PleyErea& erea):playErea_(erea)
{
}

playUnit::~playUnit()
{
}

bool playUnit::Update(void)
{
	if (playErea_.mode_ != STAGE_MODE::DROP)
	{
		return false;
	}
	if ((*playErea_.input_)->GetKeySty(INPUT_ID::DOWN))
	{
		playErea_.puyoList_.front()->SetSoftDrop();
	}
	for (auto id : INPUT_ID())
	{
		if ((*playErea_.input_)->GetKeyTrg(id))
		{
			playErea_.puyoList_.front()->Move(id);
		}
	}
	return true;
}
