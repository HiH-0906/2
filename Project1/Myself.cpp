#include <DxLib.h>
#include "MouseCtl.h"
#include "Myself.h"
#include "TiketMachine.h"
#include "_debug/_DebugConOut.h"

Myself* Myself::s_Instance = nullptr;

void Myself::Draw(void)
{
	auto moneyLine = 0;
	SetFontSize(font_size);
	for (auto cashData:_cash)
	{
		DrawFormatString(money_sizeX + 25, money_sizeY * moneyLine + money_sizeY / 3, 0xffffff, "%2d 枚", cashData.second);
		moneyLine++;
	}

	const VecInt moneyType = lpTiketMachine.GetMoneyType();

	// 仕切り線
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
		if (_mouse->GetClickTrg(MOUSE_INPUT_LEFT))
		{
			if (pos.x < money_sizeX )
			{
				if (pos.y < money_sizeY * static_cast<int>(moneyType.size()))				// _moneyTypeの要素数ﾁｪｯｸも兼ねている
				{
					auto type = moneyType[pos.y / money_sizeY];
					if (_cash[type] > 0)
					{
						if (lpTiketMachine.InsertCash(type))
						{
							TRACE("%d\n", type);
							_cash[type] --;
						}
					}
				}
				else
				{
					// 現金の範囲+1の位置がちょうど電子ﾏﾈｰ
					if (pos.y < money_sizeY * static_cast<int>(moneyType.size() + 1))		// _moneyTypeの要素数ﾁｪｯｸも兼ねている
					{
						lpTiketMachine.InsertCard();
						// 返り値を使って音つけるとかええんちゃう
					}
				}
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
		TRACE("返却されたお金：%9d  %2d枚\n", cash.first,cash.second);
	}
	return true;
}

bool Myself::SysInit(void)
{
	DxLib::SetGraphMode(screen_sizeX, screen_sizeY, 16);
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("1916035_橋本大輝");
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

	// 保持する現金を追加
	_cash.try_emplace(10, 15);
	_cash.try_emplace(50, 3);
	_cash.try_emplace(100, 2);
	_cash.try_emplace(500, 1);
	_cash.try_emplace(1000, 1);
	_cash.try_emplace(5000, 1);
	_cash.try_emplace(10000, 1);

	return true;
}

Myself::Myself() :screen_sizeX(800), screen_sizeY(600), money_sizeX(100), money_sizeY(50), font_size(18)
{
	TRACE("Myselfの生成\n")
}

Myself::~Myself()
{
	//delete _mouse;
	TRACE("Myselfの破棄\n")
}
