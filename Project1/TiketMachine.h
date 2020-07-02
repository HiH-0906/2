#pragma once
#include <functional>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "MouseCtl.h"


struct Wallet;
// Œ»İ‚Ìx•¥‚¢Ó°ÄŞ
enum class PayType
{
	CASH,			// Œ»‹à
	CARD,			// IC¶°ÄŞ
	MAX				// –¢İ’è
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

	std::string _btnKey;												// •\¦‚·‚éÎŞÀİ‚Ö‚Ì·°(images)
	std::string _cBtnKey;

	Vector2 _btnPos;													// ŒˆÏ(ó‚¯æ‚èjÎŞÀİ‚Ì•\¦ˆÊ’u
	Vector2 _cBtnPos;													// ·¬İ¾ÙÎŞÀİ‚Ì•\¦ˆÊ’u
	VecInt _moneyType;													// x•¥‚¢è–@
	PayType _payType;													// x•¥‚¢•û–@
	MapInt _cashData;													// Œ»‹à
	MapInt _cashDataChenge;												// ‚¨’Ş‚è
	MapInt _cardData;													// ¶°ÄŞÃŞ°À first‚ª‚Á‚Ä‚¢‚é‚¨‹à second‚ª·‚µˆø‚«Šz
	bool _paySuccess;													// x•¥‚¢Š®—¹

	std::map < std::string, int > _images;								// ‰æ‘œÊİÄŞÙ
	std::map <PayType, std::function<void(void)>> _draw;				// •`‰æ—p×ÑÀŞ®•Û‘¶•Ï”
	using Func_t = bool(TiketMachine::*)();
	std::map <PayType, Func_t> _pay;									// x•¥‚¢ŠÖŒWŠÖ”•Û‘¶•Ï”
	const int comment_offsetY;
	const int pay_btn_sizeX;											// x•¥‚¢ÎŞÀİ‚Ì‰¡»²½Ş
	const int pay_btn_sizeY;											// x•¥‚¢ÎŞÀİ‚Ìc»²½Ş
	const int draw_offsetX;												// •¶š•`‰æµÌ¾¯Ä
	const int draw_offsetY;												// •¶š•`‰æµÌ¾¯Ä
	const int price_cash;												// Œ»‹à‚Ìê‡‚Ì‰¿Ši
	const int price_card;												// “dqÏÈ°‚Ìê‡‚Ì‰¿Ši
	const int screen_sizeX;												// ‰æ–Ê‚Ì‰¡‚Ì‰ğ‘œ“x
	const int screen_sizeY;												// ‰æ–Ê‚Ìc‚Ì‰ğ‘œ“x
	const int money_sizeX;												// ‚¨‹à‚Ì‰¡‚Ì»²½Ş
	const int money_sizeY;												// ‚¨‹à‚Ìc‚Ì»²½Ş
	const int font_size;												// Ì«İÄ‚Ì»²½Ş

	TiketMachine();
	~TiketMachine();
};

