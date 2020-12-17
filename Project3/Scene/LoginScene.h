#pragma once

#include <map>
#include <string>
#include <vector>
#include "BaseScene.h"
#include "../map/Map.h"
#include "../Input/Input.h"
#include "../common/Vector2.h"
#include "../common/Button.h"
#include "../common/NumPad.h"

#define ResetTime 30000

enum class UPDATE_STATE
{
	SET_NET,				// 共用 ホストかゲストか
	HOST_INPUT_INIT,
	SELECT_HOST,			// ゲスト専用 前回接続者につなぐかどうか 
	READ_HOST,				// ゲスト専用 前回接続者につなぐ
	HOST_IP,				// ゲスト専用 IP入力待機
	START_INIT,				// 共用 初期化
};

// 現状ネットを試す場になっている 是非もナイネ
class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();
	void Init(void)override final;
	uniqueBase Update(uniqueBase own, const Time& now)override final;
	void DrawOwnScene(void)override final;
private:
	void FuncInit(void);
	void ButtonInit(void);
	void ImageInit(void);

	// ステートパターンで管理したい感
	using TitleFuncT = bool(LoginScene::*)();
	std::map<UPDATE_STATE, TitleFuncT> func_;
	// ステートパターンにぶち込まれる予定関数
	bool HostIPInput(void);						// IPアドレスの入力
	bool HostInputInit(void);
	bool SetNetWork(void);						// ホスト、ゲスト、オフラインの選択
	bool StartInit(void);						// 初期化
	bool ReadHost(void);						// IPアドレス読み込み

	// ファイル関連
	bool ReadFile(void);
	bool WritFile(void);
	/// <summary>
	/// IPDATAを各桁ごとの数字に変換して第三引数のVectorに格納する
	/// 156->number[0]=1,number[1]=5,number[2]=6
	/// </summary>
	/// <param name="ch">変換元のchar</param>
	/// <param name="dig">桁数</param>
	/// <param name="number">結果格納先vector</param>
	void ChengeIPDATAToIntVector(const unsigned char& ch, std::vector<int>& number);
	void IPDraw(const std::vector<int>& ipInt,Vector2& pos,const double& rate, bool comma);

	UPDATE_STATE state_;						// Update管理用変数

	// 見りゃわかる系
	int screenSize_X;
	int screenSize_Y;
	bool sendTmx_;
	bool reset_;
	bool wait_;
	Vector2 pos_;
	IPDATA ipData_;
	std::unique_ptr<Input> input_;
	std::unique_ptr<NumPad> numPad_;
	std::vector<std::unique_ptr<Button>> btn_;

	Time waitTime_;
};

