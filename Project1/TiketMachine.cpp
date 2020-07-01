#include <DxLib.h>
#include <algorithm>
#include "TiketMachine.h"
#include "Myself.h"
#include "InsertMax.h"
#include "_debug/_DebugConOut.h"


void TiketMachine::Run(void)
{
	Vector2 pos = _mouse->GetPos();
	/// <summary>
	/// ���݂̔���
	/// </summary>
	/// <param name="">true:�͈͓�  falsee:�͈͊O</param>
	auto checkBtn = [&]() {
		return ((pos.x >= _btnPos.x) && (pos.x < _btnPos.x + pay_btn_sizeX)) &&
				((pos.y >= _btnPos.y) && (pos.y < _btnPos.y + pay_btn_sizeY));
	};


	if (_mouse->GetClicking(MOUSE_INPUT_LEFT) && checkBtn())
	{
		_btnKey = "btn_push";
	}
	else
	{
		_btnKey = "btn";
	}

	if (_mouse->GetClickTrg(MOUSE_INPUT_LEFT) && checkBtn())
	{
		if (_paySuccess)
		{
			switch (_payType)
			{
			case PayType::CASH:
				if (lpMyself.MergeCash(_cashDataChenge))
				{
					Clear();
				}
				break;
			case PayType::CARD:
				if (lpCardServer.MergeCard(_cardData))
				{
					Clear();
				}
				break;
			case PayType::MAX:
				break;
			default:
				TRACE("�G���[�F���m�̎x�������@");
				_payType = PayType::MAX;
				break;
			}
		}
		else
		{
			if (_pay.find(_payType) != _pay.end())
			{
				(this->*_pay[_payType])();
			}
		}
	}
}

bool TiketMachine::InsertCash(int cash)
{
	if (_payType == PayType::MAX)
	{
		_payType = PayType::CASH;
	}

	if (_payType != PayType::CASH)
	{
		return false;
	}
	


	return true;
}

bool TiketMachine::InsertCard(void)
{
	if (_payType == PayType::MAX)
	{
		_payType = PayType::CARD;
	}
	else
	{
		// ���ދy�ь������������̏ꍇ�̂ݎ󂯕t����̂ł���ȊO�͏������Ȃ�
		return false;
	}
	_cardData = lpCardServer.GetCardState();
	return true;
}

void TiketMachine::Draw(void)
{
	//// C++20�`
	//if (_draw.contains(_payType))
	//{
	//	_draw[_payType]();
	//}
	//// �搶���悭����������
	//if (_draw.count(_payType))
	//{
	//	_draw[_payType];
	//}

	if (_draw.find(_payType) != _draw.end())
	{
		_draw[_payType]();
	}

	// �ؕ��̒l�i�\��
	DrawString(screen_sizeX / 2 - font_size, money_sizeY / 2, "�ؕ��̉��i�@�����F130�~�@�d�q�}�l�[�F124�~", 0xffffff);
	DrawBtn();
}

void TiketMachine::payType(PayType paytype)
{
	_payType = paytype;
}

VecInt& TiketMachine::GetMoneyType(void)
{
	return _moneyType;
}

void TiketMachine::Clear(void)
{
	_btnKey = "btn";
	_paySuccess = false;
	_payType = PayType::MAX;
	_cashData.clear();
	_cashDataChenge.clear();
	_cardData = { 0,0 };
	lpMyself.Insert(InsertMax());
}

void TiketMachine::DrawBtn(void)
{
	SetFontSize(font_size * 2);

	std::string btnName = _paySuccess == false ? "�@���@��" : " �󂯎��";

	DrawGraph(_btnPos.x, _btnPos.y, _images[_btnKey], true);

	DrawString(_btnPos.x + (font_size / 2), _btnPos.y + (font_size / 2), btnName.c_str(), 0x000000);
}

bool TiketMachine::PayCash(void)
{
	int totalCash = 0;
	auto tmpCashData = _cashData;

	// �������z���v
	for (auto cash : tmpCashData)
	{
		totalCash += (cash.first * cash.second);
	}

	TRACE("���v���z�F%d\n", totalCash);

	if (totalCash < price_cash)
	{
		// �������z�s��
		return false;
	}

	int credit = price_cash;

	auto cashCount = [&](std::pair<const int,int>& data) {
		while (data.second > 0)
		{
			credit -= data.first;
			data.second--;
			TRACE("�g���������F%d\n", data.first);
			if (credit <= 0)
			{
				break;
			}
		}
	};

	for (auto &data : tmpCashData)
	{
		cashCount(data);
		if (credit <= 0)
		{
			_paySuccess = true;
			// ���ޏ���
			_cashDataChenge = tmpCashData;
			int change = -credit;

	/*		for (auto it = _moneyType.crbegin(); it != _moneyType.crend(); it++)
			{
				while (change >= *it)
				{
					change -= *it;
					_cashDataChenge.try_emplace(*it, 0);
					_cashDataChenge[*it]++;
					TRACE("�Ԃ��������F%d\n", *it);
				}
				if (change < 0)
				{
					TRACE("���ނ�ُ�\n");
					return false;
				}
				if (change == 0)
				{
					return true;
				}
			}*/

			std::for_each(_moneyType.rbegin(), _moneyType.rend(), [&](int type)
			{
				while (change >= type)
				{
					_cashDataChenge.try_emplace(type, 0);
					_cashDataChenge[type]++;
					change -= type;
					TRACE("�Ԃ��������F%d\n", type);
				}
			}
			);

			if (change < 0)
			{
				TRACE("���ނ�ُ�\n");
				return false;
			}
			if (change == 0)
			{
				return true;
			}

			break;
		}
	}
	_paySuccess = false;
	return false;
}

bool TiketMachine::PayCard(void)
{
	if (lpCardServer.PayMent(price_card))
	{
		_paySuccess = true;
		_cardData = lpCardServer.GetCardState();
		return true;
	}
	return false;
}

bool TiketMachine::PayMax(void)
{
	TRACE("�������\n")
	return false;
}

bool TiketMachine::InitDraw(void)
{
	_draw.try_emplace(PayType::CASH,
		[&]() {
		int moneyLine = 0;
		int totalMoney = 0;
		DrawGraph(0, 0, _images["act_cash"], true);
		if (_paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���ϊ���\n���ނ���󂯎��ۂ͎󂯎��{�^���������Ă��������B", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "�������z", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "        �@�@����", 0xffffff);
			for (auto cashData : _cashData)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "%d�~", cashData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "�@�@�@�@%d��", cashData.second);
				moneyLine++;
				totalMoney += (cashData.first * cashData.second);
			}
			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), 0xffffff, "���v�������z�@%d�~", totalMoney);

			DrawString(draw_offsetX * 2, draw_offsetY, "   �ޑK", 0xffffff);
			DrawString(draw_offsetX * 2, draw_offsetY, "�@�@�@�@�@  ����", 0xffffff);
			int changeLine = 0;

			for (auto data : _cashDataChenge)
			{
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), 0xffffff, "%5d�~", data.first);
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), 0xffffff, "     %9d��", data.second);
				changeLine++;
			}
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���̘g���̌�����I�����N���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B", 0xffffff);

			DrawString(draw_offsetX, draw_offsetY, "�������z", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "        �@�@����", 0xffffff);

			// �����������̕\��
			for (auto cashData : _cashData)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "%d�~", cashData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "�@�@�@�@%d��", cashData.second);
				moneyLine++;
				totalMoney += (cashData.first * cashData.second);
			}
			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), 0xffffff, "���v�������z�@%d�~", totalMoney);

			if (totalMoney < price_cash)
			{
				DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 3) + moneyLine * GetFontSize(), 0xff0000, "���z������܂���");
			}
		}
	}
	);
	_draw.try_emplace(PayType::CARD,
		[&]() {
		DrawGraph(0, 0, _images["act_card"], true);
		if (_paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���ϊ���\nIC�J�[�h���o���ۂ͎󂯎��{�^���������Ă��������B", 0xffffff);

			DrawString(draw_offsetX, draw_offsetY, "�d�q�}�l�[", 0xffffff);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), 0xffffff, "�c���@%d�~", _cardData.first);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize() * 2, 0xffffff, "�����z�@%d�~", _cardData.second);
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "���̘g����IC�J�[�h��I�����N���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "�d�q�}�l�[", 0xffffff);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), 0xffffff, "�c���@%d�~", _cardData.first);
			if (_cardData.first < price_card)
			{
				DrawString(draw_offsetX, draw_offsetY + GetFontSize() * 3, "�c��������܂���\n", 0xffffff, true);
			}
		}
	}
	);
	_draw.try_emplace(PayType::MAX,
		[&]() {
		DrawGraph(0, 0, _images["money"], true);
		DrawString(0, comment_offsetY + GetFontSize() / 2, "���̘g���̌������J�[�h��I�����N���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B", 0xffffff);
	}
	);

	return true;
}

bool TiketMachine::InitPay(void)
{ 
	
	_pay.try_emplace(PayType::CASH, &TiketMachine::PayCash);
	_pay.try_emplace(PayType::CARD, &TiketMachine::PayCard);
	_pay.try_emplace(PayType::MAX, &TiketMachine::PayMax);
	return true;
}

bool TiketMachine::Init(sharedMouse mouse)
{
	_mouse = mouse;

	// �摜�̓ǂݍ���
	_images.try_emplace("money", LoadGraph("image/money.png"));
	_images.try_emplace("act_cash", LoadGraph("image/active_cash.png"));
	_images.try_emplace("act_card", LoadGraph("image/active_card.png"));
	_images.try_emplace("btn", LoadGraph("image/btn.png"));
	_images.try_emplace("btn_push", LoadGraph("image/btn_push.png"));

	// UI�z�u����Ă��邨����ǉ�
	if (_moneyType.size() == 0)
	{
		_moneyType.emplace_back(10);
		_moneyType.emplace_back(50);
		_moneyType.emplace_back(100);
		_moneyType.emplace_back(500);
		_moneyType.emplace_back(1000);
		_moneyType.emplace_back(5000);
		_moneyType.emplace_back(10000);
	}

	_btnPos = Vector2((screen_sizeX - money_sizeX * 2) - pay_btn_sizeX, static_cast<int>(money_sizeY * (_moneyType.size())));

	InitDraw();
	InitPay();

	return true;
}

MapInt& TiketMachine::cashData()
{
	return _cashData;
}

void TiketMachine::cardData(const PairInt& pInt)
{
	_cardData = pInt;
}

TiketMachine::TiketMachine() :comment_offsetY(450), draw_offsetX(200), draw_offsetY(70), price_cash(130), price_card(124), screen_sizeX(800), screen_sizeY(600), money_sizeX(100), money_sizeY(50), font_size(18), pay_btn_sizeX(200), pay_btn_sizeY(50)
{
	Clear();
	TRACE("TiketMachine�̐���\n")
}

TiketMachine::~TiketMachine()
{
	TRACE("TiketMachine�̔j��\n")
}
