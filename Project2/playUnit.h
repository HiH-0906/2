#pragma once
#include <functional>
#include <map>
#include "common/Vector2.h"
#include "State/INPUT_ID.h"
class PleyErea;

class playUnit
{
public:
	playUnit(PleyErea& erea);
	~playUnit();
	bool Update(void);
	const int targetID(void)const;								// 今自分が捜操作しているぷよ表示に使うため
private:
	PleyErea& playErea_;										// PlayUnit持ち主への参照
	std::map<INPUT_ID,std::function<void(void)>> keyFunc_;		// ｷｰ入力時の関数まとめ
	void Init(void);											// 初期化
	void RotaPuyo(Vector2 puyo1, Vector2 puyo2, bool rotaRight);// ぷよ回転用
	int targetID_;												// 今操作しているぷよ判別用
};

