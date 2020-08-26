#include <time.h>
#include <DxLib.h>
#include "SceneMng.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "SelectScene.h"
#include "../ImageMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../State/CON_ID.h"
#include "../EffectMng.h"


std::unique_ptr<SceneMng, SceneMng::SceneMngDeleter> SceneMng::s_instance(new SceneMng());

void SceneMng::Run()
{
	_dbgSetup(screenX, screenY, 16);

	activeScene_ = std::make_unique<SelectScene>();

	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		activeScene_ = (*activeScene_).Update(std::move(activeScene_));
		lpEffectMng.Update();
		Draw();
		fCnt_++;
	}
	DxLib::DxLib_End();
}

const Vector2 SceneMng::screenSize(void) const
{
	return Vector2{ screenX,screenY };
}

void SceneMng::Draw()
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();
	DATA_TYPE type;

	std::sort(drawList_.begin(), drawList_.end(), [](DrawQueT& que1, DrawQueT& que2)
	{
		return std::tie(std::get<static_cast<int>(DRAW_QUE::ID)>(que1), std::get<static_cast<int>(DRAW_QUE::ZODER)>(que1))
			<
			std::tie(std::get<static_cast<int>(DRAW_QUE::ID)>(que2), std::get<static_cast<int>(DRAW_QUE::ZODER)>(que2));
	});

	SCREEN_ID id,nextId;
	id = SCREEN_ID::BG;

	for (auto tmpid : SCREEN_ID())
	{
		SetDrawScreen(drawScreen_[tmpid]);
		ClsDrawScreen();
	}
	SetDrawScreen(drawScreen_[SCREEN_ID::BG]);
	for (auto& que:drawList_)
	{
		type = std::get<static_cast<int>(DRAW_QUE::TYPE)>(que);
		nextId = std::get<static_cast<int>(DRAW_QUE::ID)>(que);
		if (id != nextId)
		{
			SetDrawScreen(drawScreen_[nextId]);
			ClsDrawScreen();
			id = nextId;
		}

		(this->*drawFunc_[type])(std::move(que));
	}
	auto itr = std::remove_if(drawList_.begin(), drawList_.end(), [](DrawQueT& que)
	{
		return std::get<static_cast<int>(DRAW_QUE::FLAG)>(que);
	});
	drawList_.erase(drawList_.begin(), itr);

	SetDrawScreen(DX_SCREEN_BACK);
	ClsDrawScreen();

	for (auto tmpid : SCREEN_ID())
	{
		DrawGraph(0, 0, drawScreen_[tmpid], true);
	}

	_dbgAddDraw();
	ScreenFlip();
}

void SceneMng::EffectDraw(DrawQueT&& que)
{
	int handle = std::get<static_cast<int>(DRAW_QUE::HANDLE)>(que);
	if (IsEffekseer2DEffectPlaying(handle) != 0)
	{
		DrawQueT tmpque = DrawQueT{ Vector2{}, 0, 0.0, 0.0, 0, SCREEN_ID::MAX, DATA_TYPE::EFFECT, false };
		que = tmpque;
		return;
	}
	DrawEffekseer2D_Begin();
	DrawEffekseer2D_Draw(handle);
	DrawEffekseer2D_End();
}

void SceneMng::ImageDraw(DrawQueT&& que)
{
	int handle;
	double rate,rad;
	Vector2 pos;

	std::tie(pos, handle, rate, rad, std::ignore, std::ignore, std::ignore,std::ignore) = que;

	DrawRotaGraph(pos.x, pos.y, rate, rad, handle, true);
	DrawQueT tmpque = DrawQueT{ Vector2{}, 0, 0.0, 0.0, 0, SCREEN_ID::MAX, DATA_TYPE::IMG, false };
	que = tmpque;
}

const int& SceneMng::fCnt(void) const
{
	return fCnt_;
}

bool SceneMng::SysInit(void)
{
	ChangeWindowMode(true);
	SetWindowText("1916035_ã¥ñ{ëÂãP");
	SetGraphMode(screenX, screenY, 16);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	lpEffectMng.Init({ screenX ,screenY });
	drawFunc_.try_emplace(DATA_TYPE::IMG, &SceneMng::ImageDraw);
	drawFunc_.try_emplace(DATA_TYPE::EFFECT, &SceneMng::EffectDraw);
	drawScreen_.try_emplace(SCREEN_ID::BG, MakeScreen(screenX, screenY,true));
	drawScreen_.try_emplace(SCREEN_ID::PLAY, MakeScreen(screenX, screenY, true));
	drawScreen_.try_emplace(SCREEN_ID::FRONT, MakeScreen(screenX, screenY, true));
	fCnt_ = 0;
	playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 0,0 }, CON_ID::KEY));
	playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 512, 0 }, CON_ID::KEY));
	lpImageMng.GetID("BG", "image/èÙ.png");
	return true;
}

void SceneMng::AddDrawList(DrawQueT&& que)
{
	drawList_.emplace_back(que);
}

PlayEreaVec& SceneMng::playErea(void)
{
	return playErea_;
}

const int SceneMng::playNum(void)
{
	return playErea_.size();
}

SceneMng::SceneMng():screenX(1024),screenY(768)
{
	SysInit();
}

SceneMng::~SceneMng()
{
}
