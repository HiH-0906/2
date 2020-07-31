#pragma once
#include "common/Vector2.h"
#include "Puyo.h"

class OzyamaPuyo :
	public Puyo
{
public:
	OzyamaPuyo();
	OzyamaPuyo(Vector2&& pos, PUYO_ID id);
	~OzyamaPuyo();
	void SetMuyonCnt(void)override;
	bool SetPuyonCnt(void)override;
	void SetMunyonBit(DirPermit dirpermit)override;
private:
};

