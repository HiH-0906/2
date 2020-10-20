#pragma once

#include "BaseScene.h"
#include <map>
#include <string>
#include <vector>
#include "../Input/PadState.h"
#include "../common/Vector2.h"

enum class UPDATE_STATE
{
	SET_NET,				// 共用 ホストかゲストか
	SELECT_HOST,			// ゲスト専用 前回接続者につなぐかどうか 
	READ_HOST,				// ゲスト専用 前回接続者につなぐ
	HOST_IP,				// ゲスト専用 IP入力待機
	START_INIT,				// 共用 初期化
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
	bool SelectHost(void);
	bool ReadHost(void);

	// ファイル関連
	bool ReadFile(void);
	bool WritFile(void);

	std::map<std::string, std::vector<int>> test_;

	int bg;
	int image_[12];


	UPDATE_STATE state_;
	int screenSize_X;
	int screenSize_Y;
	int Image;
	Vector2 pos_;
	int speed_;
	float rad_;
	IPDATA ipData_;
	std::unique_ptr<Input> input_;
};

