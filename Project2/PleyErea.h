#pragma once
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo.h"
#include "State/PUYO_ID.h"
#include "playUnit.h"

enum class PUYO_STATE
{
	NON,
	STAY,
	DEATH,
};

enum class STAGE_MODE
{
	DROP,
	PUYON,
	FALL,
	MUNYON,
	ERASE,
};

using PuyoSt = std::pair < PUYO_ID, PUYO_STATE >;
using PuyoUnit = std::shared_ptr<Puyo>;

class PleyErea
{
public:
	PleyErea(Vector2&& size, Vector2&& offset, CON_ID id);
	~PleyErea();
	void UpDate(void);								// 更新用
	const Vector2 offset(void);						// ｵﾌｾｯﾄのget	いるっけ…？
	const int GetScreenID(void)const;				// 描画用ｽｸﾘｰﾝのget
private:
	void InstancePuyo(void);
	void Draw(void);								// 描画
	void DeletePuyo(void);							// 今のぷよを動けなくするのと次のぷよｲﾝｽﾀﾝｽ
	bool SetErasePuyo(Vector2 vec, PUYO_ID id);		// 消せるか判断
	bool CheckMovePuyo(PuyoUnit& puyo);				// 動いていいぷよか判断
	bool CheckMuyonPuyo(void);						// むよーんってしてるﾌﾟﾖがいるかどうか
	bool Init(CON_ID id);							// 初期化用関数
	int screenID_;									// 個別描画用ｽｸﾘｰﾝ
	int puyoScreenID_;								// ぷよ操作場所描画用ｽｸﾘｰﾝ
	int playerID_;									// 自分が何番目か
	const Vector2 stgSize_;							// ぷよぷよﾏｽ目
	const Vector2 size_;							// playEreaの大きさ
	std::map<STAGE_MODE, std::function<void(PleyErea&)>> func_;			// Updateまとめ
	Vector2 offset_;								// 描画時ｵﾌｾｯﾄ
	std::shared_ptr<Input*> input_;					// 入力ｸﾗｽ管理用
	std::vector<PuyoUnit> puyoList_;				// ぷよ管理用ﾘｽﾄ
	std::vector<PuyoUnit*> playErea_;				// 下の奴を二次元配列みたいにｱｸｾｽするとき用
	std::vector<PuyoUnit> playEreaBase_;			// playErea全体のﾃﾞｰﾀ管理用配列
	std::vector<PuyoUnit*> eraseErea_;				// 下の奴を二次元配列みたいにｱｸｾｽするとき用
	std::vector<PuyoUnit> eraseEreaBase_;			// playErea全体の消すとこﾃﾞｰﾀ管理用配列
	int color_;										// ｴﾘｱの色
	int blockSize_;									// 1ﾏｽの大きさ
	static int allStage_;							// 全体でplayEreaがいくつあるかのｶｳﾝﾄ
	STAGE_MODE mode_;								// 現在のﾓｰﾄﾞ
	std::unique_ptr<playUnit> playUnit_;			// 操作系まとめ

	friend class playUnit;
	friend struct FallMode;
	friend struct DropMode;
	friend struct EraseMode;
	friend struct PuyonMode;
	friend struct MunyonMode;
};

