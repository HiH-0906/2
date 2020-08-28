#pragma once
#include <vector>
#include <memory>
#include "BaseScene.h"
#include "../common/Button.h"


class TitleScene :
    public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	virtual unipueBase Update(unipueBase own) override final;
private:
};

