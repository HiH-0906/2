#include<DxLib.h>
#include"Vector2.h"
#include"Input.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	ChangeWindowMode(true);
	SetWindowText("1916035_橋本大輝");

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	Input* _input;
	int r = 25;
	int puyoDir = r * 2;
	Vector2 puyo = { puyoDir * 2 + r,0 };
	Vector2 ScreenSize(puyoDir * 20, puyoDir * 13);
	SetGraphMode(ScreenSize.x, ScreenSize.y, 16);
	_input = new Input();
	_input->Init();
	SetDrawScreen(DX_SCREEN_BACK);
	while (!ProcessMessage()&&!CheckHitKey(KEY_INPUT_ESCAPE))
	{
		_input->Update();
		ClsDrawScreen();
		puyo.y += static_cast<int>((puyoDir * 1.5) / 60);

		if (_input->GetKeyTrg(INPUT_ID::LEFT) && puyo.x - r > puyoDir * 2)
		{
			puyo.x -= puyoDir;
		}
		if (_input->GetKeyTrg(INPUT_ID::RIGHT) && puyo.x + r < puyoDir * 8)
		{
			puyo.x += puyoDir;
		}
		if (puyo.y + r >= puyoDir * 13)
		{
			puyo.y = 0;
		}

		// ﾌｨｰﾙﾄﾞ確認用
		DrawBox(puyoDir * 2, 0, puyoDir * 8, puyoDir * 13, 0x888888, true);
		DrawBox(puyoDir * 12, 0, puyoDir * 18, puyoDir * 13, 0x888888, true);
		// ﾈｸｽﾄﾌﾟﾖ確認用
		DrawBox(puyoDir * 8 + r, puyoDir, puyoDir * 9 + r, puyoDir * 3, 0x8888ff, true);
		DrawBox(puyoDir * 10 + r, puyoDir, puyoDir * 11 + r, puyoDir * 3, 0x8888ff, true);
		// ﾌﾟﾖ確認用
		DrawCircle(puyo.x, puyo.y, r, 0xff8888, true);
		DrawCircle(puyo.x , puyo.y - puyoDir, r, 0xff8888, true);

		ScreenFlip();
	}
	delete _input;
	DxLib_End();

	return 0;
}