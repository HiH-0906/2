#include "MenuScene.h"
#include "../SceneMng.h"

MenuScene::MenuScene()
{
	childScene_ = nullptr;
	draw_ = false;
	stop_ = false;
	screenImage_ = 0;
}

MenuScene::MenuScene(unipueBase child, bool draw, bool stop, int screen, std::weak_ptr<Input*> input)
{
	childScene_ = std::move(child);
	draw_ = draw;
	stop_ = stop;
	screenImage_ = screen;
	input_ = input;
}

MenuScene::~MenuScene()
{
}

unipueBase MenuScene::Update(unipueBase own)
{
	(*input_.lock())->Update();
	if (stop_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}
	if (draw_)
	{
		lpSceneMng.DrawPanel(SCREEN_ID::FRONT, 200,0x000000,5);
		lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, screenImage_,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	}
	if ((*input_.lock())->GetKeyTrg(INPUT_ID::POSE))
	{
		return std::move(childScene_);
	}
	return std::move(own);
}
