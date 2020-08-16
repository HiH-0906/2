#pragma once
#include<utility>
#include<memory>
#include"Puyo/Puyo.h"
#include"PleyErea.h"

using NextPair = std::pair<sharPuyo, sharPuyo>;
using NextList = std::list<NextPair>;

class NextPuyoCtl
{
public:
	/// <summary>
	/// NextPuyo管理用ｸﾗｽ
	/// </summary>
	/// <param name="pos">描画時位置</param>
	/// <param name="haveCount">いくつﾃﾞｰﾀを内部的に保持するか</param>
	/// <param name="drawCount">描画個数 なお描画範囲の関係で2個目の半分までしか見えない</param>
	NextPuyoCtl(Vector2& pos,int haveCount,int drawCount);
	~NextPuyoCtl();
	void Draw(void);
	NextPair PickUp(void);				// NextPuyoListから1ｾｯﾄ取り出して、1ｾｯﾄ補充

	bool Add(int no);					// nextを強制的に指定ｾｯﾄ数補充 PickUpで内部的に呼んでいるので自発的に呼ぶことはないかも

private:
	Vector2 pos_;						// 描画位置
	NextList nextPuyoList_;				// NextPuyoのﾘｽﾄ
	int drawCount_;						// いくつ描画するか
	int screenID_;
};

