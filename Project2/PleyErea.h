#pragma once
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo/Puyo.h"
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
	OZYAMA,
	GAMEOVER,
	WIN,
};

using PuyoSt = std::pair < PUYO_ID, PUYO_STATE >;
using sharPuyo = std::shared_ptr<Puyo>;

class NextPuyoCtl;

class PleyErea
{
public:
	PleyErea(Vector2&& size, Vector2&& offset,Vector2&& pos, CON_ID id);
	~PleyErea();
	int UpDate(void);								// 更新用
	const Vector2 offset(void);						// ｵﾌｾｯﾄのget	いるっけ…？
	const int GetScreenID(void)const;				// 描画用ｽｸﾘｰﾝのget
	void ozyamaCnt(int cnt);						// お邪魔ぷよ予約
	const int playerID(void)const;
	void SetWinner(bool winner);					// trueで自分が勝者
	const Vector2 pos(void)const;
private:
	void InstancePuyo(void);
	void Draw(void);								// 描画
	void DrawOzyama(void);							// お邪魔ぷよ予告の描画
	void DeletePuyo(void);							// 今のぷよを動けなくするのと次のぷよｲﾝｽﾀﾝｽ

	bool SetErasePuyo(Vector2 vec, PUYO_ID id);		// 消せるか判断
	bool CheckMovePuyo(sharPuyo& puyo);				// 動いていいぷよか判断
	bool Init(CON_ID id);							// 初期化用関数

	Vector2 ConvertGrid(Vector2&& pos);				// posをGridに

	int screenID_;									// 個別描画用ｽｸﾘｰﾝ
	int puyoScreenID_;								// ぷよ操作場所描画用ｽｸﾘｰﾝ
	int NoticeOzyamaScrID;							// お邪魔ぷよ予告用ｽｸﾘｰﾝ
	int playerID_;									// 自分が何番目か

	const Vector2 stgSize_;							// ぷよぷよﾏｽ目
	const Vector2 size_;							// playEreaの大きさ

	std::map<STAGE_MODE, std::function<int(PleyErea&)>> stageFunc_;			// Updateまとめ

	Vector2 offset_;								// 描画時ｵﾌｾｯﾄ
	Vector2 pos_;									// 描画位置

	std::map<CON_ID,std::shared_ptr<Input*>> input_;					// 入力ｸﾗｽ管理用
	CON_ID inputID_;									// 操作ｸﾗｽ選択用

	std::vector<sharPuyo> puyoList_;				// ぷよ管理用ﾘｽﾄ
	std::vector<sharPuyo*> playErea_;				// 下の奴を二次元配列みたいにｱｸｾｽするとき用
	std::vector<sharPuyo> playEreaBase_;			// playErea全体のﾃﾞｰﾀ管理用配列
	std::vector<sharPuyo*> eraseErea_;				// 下の奴を二次元配列みたいにｱｸｾｽするとき用
	std::vector<sharPuyo> eraseEreaBase_;			// playErea全体の消すとこﾃﾞｰﾀ管理用配列

	int color_;										// ｴﾘｱの色
	int blockSize_;									// 1ﾏｽの大きさ
	int rensaNum_;									// 現在連鎖数
	int rensaMax_;									// 今の試合での最大連鎖数
	int ozyamaCnt_;									// お邪魔ぷよ落下数
	int ozyamaFallMax_;								// お邪魔ぷよ同時落下最大数
	int eraseCnt_;									// 今のEraseでいくつぷよ消したか
	static int allStage_;							// 全体でplayEreaがいくつあるかのｶｳﾝﾄ

	STAGE_MODE mode_;								// 現在のﾓｰﾄﾞ
	std::unique_ptr<playUnit> playUnit_;			// 操作系まとめ
	std::unique_ptr<NextPuyoCtl> nextPuyo_;			// next管理用
	void FallOzyama(void);							// お邪魔ぷよｲﾝｽﾀﾝｽ用

	friend class playUnit;
	friend struct FallMode;
	friend struct DropMode;
	friend struct EraseMode;
	friend struct PuyonMode;
	friend struct MunyonMode;
	friend struct OzyamaMode;
	friend struct GameOverMode;
	friend struct WinMode;
};

