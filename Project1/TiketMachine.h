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

// ���݂̎x����Ӱ��
enum class PayType
{
	CASH,			// ����
	CARD,			// IC����
	CHARGE,			// ���ނ������
	MAX				// ���ݒ�
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
	bool InsertCash(int cash);		// ������t����
	bool InsertCard(void);			// �d�q�Ȱ���ނ̏���
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

	std::string _btnKey;					// �\���������݂ւ̷�(images)

	Vector2 _btnPos;						// ����(�󂯎��j���݂̕\���ʒu
	VecInt _moneyType;						// �x������@
	PayType _payType;						// �x�������@
	MapInt _cashData;						// ����
	MapInt _cashDataChenge;					// ���ނ�
	PairInt _cardData;						// �����ް� first�������Ă��邨�� second�����������z
	bool _paySuccess;						// �x��������

	std::map < std::string, int > _images;	// �摜�����
	
	const int comment_offsetY;
	const int pay_btn_sizeX;				// �x�������݂̉�����
	const int pay_btn_sizeY;				// �x�������݂̏c����
	const int draw_offsetX;					// �����`��̾��
	const int draw_offsetY;					// �����`��̾��
	const int price_cash;					// �����̏ꍇ�̉��i
	const int price_card;					// �d�q�Ȱ�̏ꍇ�̉��i
	const int screen_sizeX;					// ��ʂ̉��̉𑜓x
	const int screen_sizeY;					// ��ʂ̏c�̉𑜓x
	const int money_sizeX;					// �����̉��̻���
	const int money_sizeY;					// �����̏c�̻���
	const int font_size;					// ̫�Ă̻���

	TiketMachine();
	~TiketMachine();
};

