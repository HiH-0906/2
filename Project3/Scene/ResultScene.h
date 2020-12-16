#pragma once
#include <list>
#include "BaseScene.h"

class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();

	uniqueBase Update(uniqueBase own, const Time& now)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
private:
	std::list<int> data_;
};

