#pragma once

#include <map>
#include <string>
#include <vector>
#include "BaseScene.h"
#include "../map/Map.h"
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

// 現状ネットを試す場になっている 是非もナイネ
class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();
	void Init(void)override final;
	uniqueBase Update(uniqueBase own, NowTime time)override final;
	void DrawOwnScene(void)override final;
private:

	// ステートパターンで管理したい感
	using TitleFuncT = bool(LoginScene::*)();

	std::map<UPDATE_STATE, TitleFuncT> func_;
	// ステートパターンにぶち込まれる予定関数
	bool HostIPInput(void);						// IPアドレスの入力
	bool PlayUpdate(void);						// ゲーム本編 最終的になくなると思う
	bool SetNetWork(void);						// ホスト、ゲスト、オフラインの選択
	bool StartInit(void);						// 初期化
	bool SelectHost(void);						// IPアドレスを入力するか読み込むか
	bool ReadHost(void);						// IPアドレス読み込み

	// ファイル関連
	bool ReadFile(void);
	bool WritFile(void);


	UPDATE_STATE state_;						// Update管理用変数

	// 見りゃわかる系 一部タイトルで持つのこれ…？ってのがあるけどいずれなくなるでしょう現状TitleSceneしかないので是非もナイネ
	int screenSize_X;
	int screenSize_Y;
	int Image;
	Vector2 pos_;
	IPDATA ipData_;
	std::unique_ptr<Input> input_;

	int tetHight_;
};

