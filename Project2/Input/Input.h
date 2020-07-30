#pragma once
#include<utility>
#include<array>
#include<map>
#include<DxLib.h>
#include"../State/CON_ID.h"
#include"../State/INPUT_ID.h"


using InputData = std::map<INPUT_ID, std::pair<bool, bool>>;

using keyPair = std::pair<INPUT_ID, int>;
using keyData = std::array<keyPair,static_cast<size_t>(INPUT_ID::MAX)>;

class Input
{
public:
	Input();
	virtual void Update(const int& p_id) = 0;					// Update継承元
	virtual CON_ID GetID(void) = 0;								// ｺﾝﾄﾛｰﾗｰ種別取得用
	virtual void Setting(void);									// 初期化後個別ｾﾃｨﾝｸﾞ必要な場合のみ継承先に記述
	bool GetKeyTrg(INPUT_ID key);								// 指定IDのﾄﾘｶﾞｰ判定
	bool GetKeySty(INPUT_ID key);								// 指定IDのおしっぱ判定
private:
	void Init(void);											// 初期化
protected:
	InputData _input;											// 押下ﾃﾞｰﾀ格納先
};

