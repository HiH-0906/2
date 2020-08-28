#pragma once
#include "../BaseScene.h"
#include "../../Input/Input.h"

class MenuScene :
	public BaseScene
{
public:
	MenuScene();
	MenuScene(unipueBase child, bool draw, bool stop,int screen, std::weak_ptr<Input*> input);
	virtual ~MenuScene();
	virtual unipueBase Update(unipueBase own)override;
protected:
	unipueBase childScene_;
	bool draw_;
	bool stop_;
	int screenImage_;

	std::weak_ptr<Input*> input_;
};

