#pragma once
#include "MenuScene.h"
class MissedConScene :
	public MenuScene
{
public:
	MissedConScene();
	MissedConScene(unipueBase child, bool draw, bool stop, int screen,int num);
	 ~MissedConScene();
	unipueBase Update(unipueBase own)override final;
private:
	int padNum_;
};

