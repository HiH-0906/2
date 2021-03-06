#pragma once
#include <functional>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "MouseCtl.h"


struct Wallet;
// 現在の支払いﾓｰﾄﾞ
enum class PayType
{
	CASH,			// 現金
	CARD,			// ICｶｰﾄﾞ
	MAX				// 未設定
};

enum class CardType
{
	BALANCE,
	CHANGE,
	MAX
};

using PairInt = std::pair<int, int>;
using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using sharedMouse = std::shared_ptr<MouseCtl>;


#define CardKey  0
#define lpTiketMachine  TiketMachine::GetInstance()



class TiketMachine
{
public:
	static TiketMachine& GetInstance()
	{
		static TiketMachine s_Instance;
		return (s_Instance);
	}
	void Run();
	void Draw(void);
	void payType(PayType paytype);
	bool Init(sharedMouse mouse);
	bool paySuccess(void);
	MapInt& GetMonyeData(Wallet wallet);
	VecInt& GetMoneyType(void);
private:
	void CancelButton(void);
	void Clear(void);
	void DrawBtn(void);
	bool PayCash(void);
	bool PayCard(void);
	bool PayMax(void);
	bool InitDraw(void);
	bool InitPay(void);

	sharedMouse _mouse;

	std::string _btnKey;												// 表示するﾎﾞﾀﾝへのｷｰ(images)
	std::string _cBtnKey;

	Vector2 _btnPos;													// 決済(受け取り）ﾎﾞﾀﾝの表示位置
	Vector2 _cBtnPos;													// ｷｬﾝｾﾙﾎﾞﾀﾝの表示位置
	VecInt _moneyType;													// 支払い手法
	PayType _payType;													// 支払い方法
	MapInt _cashData;													// 現金
	MapInt _cashDataChenge;												// お釣り
	MapInt _cardData;													// ｶｰﾄﾞﾃﾞｰﾀ firstが持っているお金 secondが差し引き額
	bool _paySuccess;													// 支払い完了

	std::map < std::string, int > _images;								// 画像ﾊﾝﾄﾞﾙ
	std::map <PayType, std::function<void(void)>> _draw;				// 描画用ﾗﾑﾀﾞ式保存変数
	using Func_t = bool(TiketMachine::*)();
	std::map <PayType, Func_t> _pay;									// 支払い関係関数保存変数
	const int comment_offsetY;
	const int pay_btn_sizeX;											// 支払いﾎﾞﾀﾝの横ｻｲｽﾞ
	const int pay_btn_sizeY;											// 支払いﾎﾞﾀﾝの縦ｻｲｽﾞ
	const int draw_offsetX;												// 文字描画ｵﾌｾｯﾄ
	const int draw_offsetY;												// 文字描画ｵﾌｾｯﾄ
	const int price_cash;												// 現金の場合の価格
	const int price_card;												// 電子ﾏﾈｰの場合の価格
	const int screen_sizeX;												// 画面の横の解像度
	const int screen_sizeY;												// 画面の縦の解像度
	const int money_sizeX;												// お金の横のｻｲｽﾞ
	const int money_sizeY;												// お金の縦のｻｲｽﾞ
	const int font_size;												// ﾌｫﾝﾄのｻｲｽﾞ

	TiketMachine();
	~TiketMachine();
};

