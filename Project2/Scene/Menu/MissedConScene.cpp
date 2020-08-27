#include <DxLib.h>
#include "../SceneMng.h"
#include "MissedConScene.h"
#include "../../_debug/_DebugConOut.h"

MissedConScene::MissedConScene()
{
	childScene_ = nullptr;
	draw_ = false;
	stop_ = false;
	screenImage_ = 0;
	padNum_ = 0;

}

MissedConScene::MissedConScene(unipueBase child, bool draw, bool stop, int screen, int num)
{
	childScene_ = std::move(child);
	draw_ = draw;
	stop_ = stop;
	screenImage_ = screen;
	padNum_ = num;
}

MissedConScene::~MissedConScene()
{
}

unipueBase MissedConScene::Update(unipueBase own)
{
	TRACE("MissedConScene\n");
	if (stop_)
	{
		childScene_ = childScene_->Update(std::move(childScene_));
	}
	if (draw_)
	{
		lpSceneMng.DrawPanel(lpSceneMng.screenSize() / 2, lpSceneMng.screenSize(), SCREEN_ID::FRONT, 200, 0x000000, 5);
		lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, screenImage_,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	}
	ReSetupJoypad();
	if (GetJoypadNum() == padNum_)
	{
		for (int i = 1; i <= padNum_; i++)
		{
			if (GetJoypadInputState(i) & PAD_INPUT_8)
			{
				return std::move(childScene_);
			}
		}
	}
	return std::move(own);
}
