#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include "../common/Vector2.h"

class BaseScene;											// ｸﾗｽのﾌﾟﾛﾄﾀｲﾌﾟ宣言

using unipueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unipueBase Update(unipueBase own) = 0;			// どのｼｰﾝにもあるので純粋仮想
private:
	int fadeScrID_;
	int fadeCount_;
	std::array<Vector2,2> fadePos_;
	const int fadeSpeed_;
	int fadeScreen_;
	std::string fadeType_;
protected:
	int startFCnt_;
	void FadeInit(bool close);
	int FadeUpdate(void);
};

