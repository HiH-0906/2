#include "OzyamaPuyo.h"
#include "SceneMng.h"

OzyamaPuyo::OzyamaPuyo()
{
}

OzyamaPuyo::OzyamaPuyo(Vector2&& pos, PUYO_ID id)
{
	playPuyo_ = false;
	pos_ = pos;
	rad_ = PUYO_RAD;
	activ_ = true;
	id_ = PUYO_ID::OZAYMA;
	puyonCnt_ = 0;
	puyonMax_ = 12;
	muyonCnt_ = 0;
	muyonMax_ = 12;
	downCnt_ = 0;
	downNum_ = 15;
	dirpermit_.per = 0;
	oldDirpermit_.per = 0;
	munyonmit_.per = 0;
	vec_ = {
			{INPUT_ID::LEFT,Vector2{-rad_ * 2,0}},
			{INPUT_ID::RIGHT,Vector2{rad_ * 2,0}},
			{INPUT_ID::UP,Vector2{0,-rad_ * 2}},
			{INPUT_ID::DOWN,Vector2{0,rad_ / 2}}
	};
}

OzyamaPuyo::~OzyamaPuyo()
{
}

void OzyamaPuyo::SetMuyonCnt(void)
{
}

bool OzyamaPuyo::SetPuyonCnt(void)
{
	// ‰½‚à‚µ‚È‚¢
	return false;
}

void OzyamaPuyo::SetMunyonBit(DirPermit dirpermit)
{
	// ‰½‚à‚µ‚È‚¢
}
