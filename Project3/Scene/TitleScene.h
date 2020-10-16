#pragma once

#include "BaseScene.h"
#include "../Input/PadState.h"
#include "../common/Vector2.h"

enum class UPDATE_STATE
{
	SET_NET,
	HOST_IP,
	START_INIT,
	PLAY,
};

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

	// ステートパターンで管理したい感
	using TitleFuncT = bool(TitleScene::*)();

	std::map<UPDATE_STATE, TitleFuncT> func_;
	// ステートパターンにぶち込まれる予定関数
	bool HostIPInput(void);
	bool PlayUpdate(void);
	bool SetNetWork(void);
	bool StartInit(void);

	UPDATE_STATE state_;
	int screenSize_X;
	int screenSize_Y;
	int Image;
	Vector2 pos_;
	int speed_;
	float rad_;
	std::unique_ptr<Input> input_;
};

