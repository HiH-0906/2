#include <DxLib.h>
#include "MouseCtl.h"
#include "Myself.h"
#include "TiketMachine.h"
#include "InsertMax.h"
#include "_debug/_DebugConOut.h"

Myself* Myself::s_Instance = nullptr;

void Myself::Draw(void)
{
	auto moneyLine = 0;
	SetFontSize(font_size);
	for (auto cashData:_cash)
	{
		DrawFormatString(money_sizeX + 25, money_sizeY * moneyLine + money_sizeY / 3, 0xffffff, "%2d ��", cashData.second);
		moneyLine++;
	}

	const VecInt moneyType = lpTiketMachine.GetMoneyType();

	// �d�؂��
	DrawLine(0, money_sizeY * (moneyType.size() + 2), screen_sizeX, money_sizeY * (moneyType.size() + 2), 0xffffff, true);
	DrawLine(money_sizeX * 2, 0, money_sizeX * 2, money_sizeY * (moneyType.size() + 2), 0xffffff, true);
	lpTiketMachine.Draw();
}
bool Myself::Run(void)
{
	if (!SysInit())
	{
		return false;
	}
	if (!MyInit())
	{
		return false;
	}
	if (!lpTiketMachine.Init(_mouse))
	{
		return false;
	}

	const VecInt& moneyType = lpTiketMachine.GetMoneyType();

	while (!DxLib::ProcessMessage() && !DxLib::CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_mouse->Update();
		const Vector2 pos = _mouse->GetPos();
		Wallet wallet = { PayType::MAX,0 };
		if (_mouse->GetClickTrg(MOUSE_INPUT_LEFT))
		{			
			if (!lpTiketMachine.paySuccess())
			{
				if (pos.x < money_sizeX)
				{
					if (pos.y < money_sizeY * static_cast<int>(moneyType.size()))				// _moneyType�̗v�f�����������˂Ă���
					{
						wallet.cash = moneyType[pos.y / money_sizeY];
						if (lpMyself._cash[wallet.cash] > 0)
						{
							_cash[wallet.cash]--;
							wallet.payType = PayType::CASH;
						}
					}
					else
					{
						// �����͈̔�+1�̈ʒu�����傤�Ǔd�q�Ȱ
						if (pos.y < money_sizeY * static_cast<int>(moneyType.size() + 1))		// _moneyType�̗v�f�����������˂Ă���
						{
							wallet.payType = PayType::CARD;
						}
					}
				}
				_insert(wallet, lpTiketMachine.GetMonyeData(wallet));
			}
		}
		if (_mouse->GetClickTrg(MOUSE_INPUT_MIDDLE))
		{
			if (pos.x < money_sizeX)
			{
				if (pos.y < money_sizeY * static_cast<int>(moneyType.size()))
				{
					auto type = moneyType[pos.y / money_sizeY];
					TRACE("%d\n", type)
						_cash[type] ++;
				}
			}
		}

		lpTiketMachine.Run();

		DxLib::SetDrawScreen(DX_SCREEN_BACK);
		DxLib::ClsDrawScreen();
		Draw();
		DxLib::ScreenFlip();
	}
	return true;
}

bool Myself::MergeCash(MapInt& change)
{
	for (auto cash : change)
	{
		_cash[cash.first] += cash.second;
		TRACE("�ԋp���ꂽ�����F%9d  %2d��\n", cash.first,cash.second);
	}
	return true;
}

void Myself::SetInsert(Func_T func)
{
	_insert = func;
}

void Myself::ClearInsert(void)
{
	_insert = InsertMax();
}



bool Myself::SysInit(void)
{
	DxLib::SetGraphMode(screen_sizeX, screen_sizeY, 16);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("1916035_���{��P");
	if (DxLib::DxLib_Init() == -1)
	{
		return false;
	}
	DxLib::SetFontSize(font_size);
	return true;
}


bool Myself::MyInit(void)
{
	_mouse = std::make_shared<MouseCtl>();

	if (!_mouse)
	{
		return false;
	}

	// �ێ����錻����ǉ�
	_cash.try_emplace(10, 15);
	_cash.try_emplace(50, 3);
	_cash.try_emplace(100, 2);
	_cash.try_emplace(500, 1);
	_cash.try_emplace(1000, 1);
	_cash.try_emplace(5000, 1);
	_cash.try_emplace(10000, 1);

	_insert = InsertMax();

	return true;
}

Myself::Myself() :screen_sizeX(800), screen_sizeY(600), money_sizeX(100), money_sizeY(50), font_size(18)
{
	TRACE("Myself�̐���\n")
}

Myself::~Myself()
{
	//delete _mouse;
	TRACE("Myself�̔j��\n")
}
