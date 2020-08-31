#pragma once
#include <vector>
#include <memory>
#include "BaseScene.h"
#include "../PleyErea.h"
#include "../common/Button.h"


class TitleScene :
    public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	virtual unipueBase Update(unipueBase own) override final;
private:
	std::vector<sharedButton> button_;
	int cursorNum_;
	static bool close_;
	PlayEreaVec playErea_;
};

