#pragma once
#include <functional>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include "MouseCtl.h"


struct Wallet;
// ���݂̎x����Ӱ��
enum class PayType
{
	CASH,			// ����
	CARD,			// IC����
	MAX				// ���ݒ�
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

	std::string _btnKey;												// �\���������݂ւ̷�(images)
	std::string _cBtnKey;

	Vector2 _btnPos;													// ����(�󂯎��j���݂̕\���ʒu
	Vector2 _cBtnPos;													// ��ݾ����݂̕\���ʒu
	VecInt _moneyType;													// �x������@
	PayType _payType;													// �x�������@
	MapInt _cashData;													// ����
	MapInt _cashDataChenge;												// ���ނ�
	MapInt _cardData;													// �����ް� first�������Ă��邨�� second�����������z
	bool _paySuccess;													// �x��������

	std::map < std::string, int > _images;								// �摜�����
	std::map <PayType, std::function<void(void)>> _draw;				// �`��p���ގ��ۑ��ϐ�
	using Func_t = bool(TiketMachine::*)();
	std::map <PayType, Func_t> _pay;									// �x�����֌W�֐��ۑ��ϐ�
	const int comment_offsetY;
	const int pay_btn_sizeX;											// �x�������݂̉�����
	const int pay_btn_sizeY;											// �x�������݂̏c����
	const int draw_offsetX;												// �����`��̾��
	const int draw_offsetY;												// �����`��̾��
	const int price_cash;												// �����̏ꍇ�̉��i
	const int price_card;												// �d�q�Ȱ�̏ꍇ�̉��i
	const int screen_sizeX;												// ��ʂ̉��̉𑜓x
	const int screen_sizeY;												// ��ʂ̏c�̉𑜓x
	const int money_sizeX;												// �����̉��̻���
	const int money_sizeY;												// �����̏c�̻���
	const int font_size;												// ̫�Ă̻���

	TiketMachine();
	~TiketMachine();
};

