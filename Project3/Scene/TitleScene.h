#pragma once

#include "BaseScene.h"
#include "../Input/PadState.h"
#include "../common/Vector2.h"

class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	void Init(void)override final;
	unipueBase Update(unipueBase own)override final;
	void Draw(void)override final;
private:
	int screenSize_X;
	int screenSize_Y;
	int Image;
	Vector2 pos_;
	int speed_;
	float rad_;
	std::unique_ptr<Input> input_;
};

