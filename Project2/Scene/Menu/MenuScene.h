#pragma once
#include "../BaseScene.h"

class MenuScene :
	public BaseScene
{
public:
	MenuScene();
	MenuScene(unipueBase child, bool draw, bool stop,int screen);
	virtual ~MenuScene();
	virtual unipueBase Update(unipueBase own)override;
protected:
	unipueBase childScene_;
	bool draw_;
	bool stop_;
	int screenImage_;
};

