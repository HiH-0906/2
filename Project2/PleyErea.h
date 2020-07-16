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
	int _screenID;									// 個別描画用ｽｸﾘｰﾝ
	int _playerID;									// 自分が何番目か
	const Vector2 _stgSize;							// ぷよぷよﾏｽ目
	const Vector2 _size;							// playEreaの大きさ
	Vector2 _offset;								// 描画時ｵﾌｾｯﾄ
	std::shared_ptr<Input*> _input;					// 入力ｸﾗｽ管理用
	std::list<std::shared_ptr<Puyo>> _puyoList;		// ぷよ管理用ﾘｽﾄ
	std::vector<PUYO_ID*> _playErea;				// 下の奴を二次元配列みたいにｱｸｾｽするとき用
	std::vector<PUYO_ID> _playEreaBase;				// playErea全体のﾃﾞｰﾀ管理用配列
	int _color;										// ｴﾘｱの色
	int _blockSize;									// 1ﾏｽの大きさ
	int aliveCnt_;									// 接地後生きてる時間管理用
	static int _allStage;							// 全体でplayEreaがいくつあるかのｶｳﾝﾄ
};

