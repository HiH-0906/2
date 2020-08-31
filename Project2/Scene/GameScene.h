#pragma once
#include "BaseScene.h"
#include "../PleyErea.h"

class GameScene:
	public BaseScene
{
public:
	GameScene(std::vector<std::shared_ptr<PleyErea>>&& playErea);
	~GameScene();
	virtual unipueBase Update(unipueBase own) override final;
private:
	std::vector<std::shared_ptr<PleyErea>> playErea_;
	int playPadNum_;
	int startFCnt_;
	int cntDownNum_;
	bool overFlag;
};

