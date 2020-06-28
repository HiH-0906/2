#pragma once
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "MouseCtl.h"
#include "CardServer.h"

using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using sharedMouse = std::shared_ptr<MouseCtl>;

#define lpTiketMachine  TiketMachine::GetInstance()

// 現在の支払いﾓｰﾄﾞ
enum class PayType
{
	CASH,			// 現金
	CARD,			// ICｶｰﾄﾞ
	CHARGE,			// ｶｰﾄﾞにﾁｬｰｼﾞ
	MAX				// 未設定
};


class TiketMachine
{
public:
	static TiketMachine& GetInstance()
	{
		static TiketMachine s_Instance;
		return (s_Instance);
	}
	void Run(void);
	bool InsertCash(int cash);		// 入金受付処理
	bool InsertCard(void);			// 電子ﾏﾈｰｶｰﾄﾞの処理
	bool InsertCharge(int cash);
	void Draw(void);
	VecInt& GetMoneyType(void);
	bool Init(sharedMouse mouse);
private:
	void Clear(void);
	void DrawBtn(void);
	bool PayCash(void);
	bool PayCard(void);

	sharedMouse _mouse;

	std::string _btnKey;					// 表示するﾎﾞﾀﾝへのｷｰ(images)

	Vector2 _btnPos;						// 決済(受け取り）ﾎﾞﾀﾝの表示位置
	VecInt _moneyType;						// 支払い手法
	PayType _payType;						// 支払い方法
	MapInt _cashData;						// 現金
	MapInt _cashDataChenge;					// お釣り
	PairInt _cardData;						// ｶｰﾄﾞﾃﾞｰﾀ firstが持っているお金 secondが差し引き額
	bool _paySuccess;						// 支払い完了

	std::map < std::string, int > _images;	// 画像ﾊﾝﾄﾞﾙ
	
	const int comment_offsetY;
	const int pay_btn_sizeX;				// 支払いﾎﾞﾀﾝの横ｻｲｽﾞ
	const int pay_btn_sizeY;				// 支払いﾎﾞﾀﾝの縦ｻｲｽﾞ
	const int draw_offsetX;					// 文字描画ｵﾌｾｯﾄ
	const int draw_offsetY;					// 文字描画ｵﾌｾｯﾄ
	const int price_cash;					// 現金の場合の価格
	const int price_card;					// 電子ﾏﾈｰの場合の価格
	const int screen_sizeX;					// 画面の横の解像度
	const int screen_sizeY;					// 画面の縦の解像度
	const int money_sizeX;					// お金の横のｻｲｽﾞ
	const int money_sizeY;					// お金の縦のｻｲｽﾞ
	const int font_size;					// ﾌｫﾝﾄのｻｲｽﾞ

	TiketMachine();
	~TiketMachine();
};

