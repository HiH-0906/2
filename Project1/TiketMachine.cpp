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
	/// ﾎﾞﾀﾝの判定
	/// </summary>
	/// <param name="">true:範囲内  falsee:範囲外</param>
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
				TRACE("エラー：未知の支払い方法");
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
		// ｶｰﾄﾞ及び現金が未投入の場合のみ受け付けるのでそれ以外は処理しない
		return false;
	}
	_cardData = lpCardServer.GetCardState();
	return true;
}

void TiketMachine::Draw(void)
{
	//// C++20～
	//if (_draw.contains(_payType))
	//{
	//	_draw[_payType]();
	//}
	//// 先生がよく書く書き方
	//if (_draw.count(_payType))
	//{
	//	_draw[_payType];
	//}

	if (_draw.find(_payType) != _draw.end())
	{
		_draw[_payType]();
	}

	// 切符の値段表示
	DrawString(screen_sizeX / 2 - font_size, money_sizeY / 2, "切符の価格　現金：130円　電子マネー：124円", 0xffffff);
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
	lpMyself.insertClear();
}

void TiketMachine::DrawBtn(void)
{
	SetFontSize(font_size * 2);

	std::string btnName = _paySuccess == false ? "　決　済" : " 受け取り";

	DrawGraph(_btnPos.x, _btnPos.y, _images[_btnKey], true);

	DrawString(_btnPos.x + (font_size / 2), _btnPos.y + (font_size / 2), btnName.c_str(), 0x000000);
}

bool TiketMachine::PayCash(void)
{
	int totalCash = 0;
	auto tmpCashData = _cashData;

	// 投入金額合計
	for (auto cash : tmpCashData)
	{
		totalCash += (cash.first * cash.second);
	}

	TRACE("合計金額：%d\n", totalCash);

	if (totalCash < price_cash)
	{
		// 投入金額不足
		return false;
	}

	int credit = price_cash;

	auto cashCount = [&](std::pair<const int,int>& data) {
		while (data.second > 0)
		{
			credit -= data.first;
			data.second--;
			TRACE("使ったお金：%d\n", data.first);
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
			// お釣処理
			_cashDataChenge = tmpCashData;
			int change = -credit;

	/*		for (auto it = _moneyType.crbegin(); it != _moneyType.crend(); it++)
			{
				while (change >= *it)
				{
					change -= *it;
					_cashDataChenge.try_emplace(*it, 0);
					_cashDataChenge[*it]++;
					TRACE("返したお金：%d\n", *it);
				}
				if (change < 0)
				{
					TRACE("お釣り異常\n");
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
					TRACE("返したお金：%d\n", type);
				}
			}
			);

			if (change < 0)
			{
				TRACE("お釣り異常\n");
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
	TRACE("金入れろ\n")
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
			DrawString(0, comment_offsetY + GetFontSize() / 2, "決済完了\nお釣りを受け取る際は受け取りボタンを押してください。", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "投入金額", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "        　　枚数", 0xffffff);
			for (auto cashData : _cashData)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "%d円", cashData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "　　　　%d枚", cashData.second);
				moneyLine++;
				totalMoney += (cashData.first * cashData.second);
			}
			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), 0xffffff, "合計投入金額　%d円", totalMoney);

			DrawString(draw_offsetX * 2, draw_offsetY, "   釣銭", 0xffffff);
			DrawString(draw_offsetX * 2, draw_offsetY, "　　　　　  枚数", 0xffffff);
			int changeLine = 0;

			for (auto data : _cashDataChenge)
			{
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), 0xffffff, "%5d円", data.first);
				DrawFormatString(draw_offsetX * 2, (draw_offsetY + GetFontSize()) + changeLine * GetFontSize(), 0xffffff, "     %9d枚", data.second);
				changeLine++;
			}
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "左の枠内の現金を選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。", 0xffffff);

			DrawString(draw_offsetX, draw_offsetY, "投入金額", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "        　　枚数", 0xffffff);

			// 現金投入時の表示
			for (auto cashData : _cashData)
			{
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "%d円", cashData.first);
				DrawFormatString(draw_offsetX + GetFontSize(), (draw_offsetY + GetFontSize()) + moneyLine * GetFontSize(), 0xffffff, "　　　　%d枚", cashData.second);
				moneyLine++;
				totalMoney += (cashData.first * cashData.second);
			}
			DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 2) + moneyLine * GetFontSize(), 0xffffff, "合計投入金額　%d円", totalMoney);

			if (totalMoney < price_cash)
			{
				DrawFormatString(draw_offsetX, (draw_offsetY + GetFontSize() * 3) + moneyLine * GetFontSize(), 0xff0000, "金額が足りません");
			}
		}
	}
	);
	_draw.try_emplace(PayType::CARD,
		[&]() {
		DrawGraph(0, 0, _images["act_card"], true);
		if (_paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "決済完了\nICカードを出す際は受け取りボタンを押してください。", 0xffffff);

			DrawString(draw_offsetX, draw_offsetY, "電子マネー", 0xffffff);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), 0xffffff, "残高　%d円", _cardData.first);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize() * 2, 0xffffff, "引去額　%d円", _cardData.second);
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2, "左の枠内のICカードを選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。", 0xffffff);
			DrawString(draw_offsetX, draw_offsetY, "電子マネー", 0xffffff);
			DrawFormatString(draw_offsetX + GetFontSize(), draw_offsetY + GetFontSize(), 0xffffff, "残高　%d円", _cardData.first);
			if (_cardData.first < price_card)
			{
				DrawString(draw_offsetX, draw_offsetY + GetFontSize() * 3, "残高が足りません\n", 0xffffff, true);
			}
		}
	}
	);
	_draw.try_emplace(PayType::MAX,
		[&]() {
		DrawGraph(0, 0, _images["money"], true);
		DrawString(0, comment_offsetY + GetFontSize() / 2, "左の枠内の現金かカードを選択しクリックして入金してください。\n入金が完了したら決済ボタンを押してください。", 0xffffff);
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

	// 画像の読み込み
	_images.try_emplace("money", LoadGraph("image/money.png"));
	_images.try_emplace("act_cash", LoadGraph("image/active_cash.png"));
	_images.try_emplace("act_card", LoadGraph("image/active_card.png"));
	_images.try_emplace("btn", LoadGraph("image/btn.png"));
	_images.try_emplace("btn_push", LoadGraph("image/btn_push.png"));

	// UI配置されているお金を追加
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


TiketMachine::TiketMachine() :comment_offsetY(450), draw_offsetX(200), draw_offsetY(70), price_cash(130), price_card(124), screen_sizeX(800), screen_sizeY(600), money_sizeX(100), money_sizeY(50), font_size(18), pay_btn_sizeX(200), pay_btn_sizeY(50)
{
	Clear();
	TRACE("TiketMachineの生成\n")
}

TiketMachine::~TiketMachine()
{
	TRACE("TiketMachineの破棄\n")
}
