#pragma once
#include <memory>
#include <vector>
#include <list>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo.h"
#include "State/PUYO_ID.h"

enum class PUYO_STATE
{
	NON,
	STAY,
	DEATH,
};

using PuyoSt = std::pair < PUYO_ID, PUYO_STATE >;

class PleyErea
{
public:
	PleyErea(Vector2&& size, Vector2&& offset, CON_ID id);
	~PleyErea();
	void UpDate(void);								// 更新用
	const Vector2 offset(void);						// ｵﾌｾｯﾄのget	いるっけ…？
	const int GetScreenID(void)const;				// 描画用ｽｸﾘｰﾝのget
private:
	void Draw(void);								// 描画
	void NextPuyo(void);							// 今のぷよを動けなくするのと次のぷよｲﾝｽﾀﾝｽ
	bool CheckMovePuyo(void);						// 動いていいぷよか判断
	bool Init(CON_ID id);							// 初期化用関数
	int screenID_;									// 個別描画用ｽｸﾘｰﾝ
	int puyoScreenID_;								// ぷよ操作場所描画用ｽｸﾘｰﾝ
	int playerID_;									// 自分が何番目か
	const Vector2 stgSize_;							// ぷよぷよﾏｽ目
	const Vector2 size_;							// playEreaの大きさ
	Vector2 offset_;								// 描画時ｵﾌｾｯﾄ
	std::shared_ptr<Input*> input_;					// 入力ｸﾗｽ管理用
	std::list<std::shared_ptr<Puyo>> puyoList_;		// ぷよ管理用ﾘｽﾄ
	std::vector<PUYO_ID*> playErea_;				// 下の奴を二次元配列みたいにｱｸｾｽするとき用
	std::vector<PUYO_ID> playEreaBase_;				// playErea全体のﾃﾞｰﾀ管理用配列
	int color_;										// ｴﾘｱの色
	int blockSize_;									// 1ﾏｽの大きさ
	int aliveCnt_;									// 接地後生きてる時間管理用
	int downCnt_;
	static int allStage_;							// 全体でplayEreaがいくつあるかのｶｳﾝﾄ
};

