#include<DxLib.h>
#include<cmath>
#include<Vector>
#include"Geometry.h"
#include "BulMng.h"
#include"bullet.h" 


///当たり判定関数
///@param posA Aの座標
///@param radiusA Aの半径
///@param posB Bの座標
///@param radiusB Bの半径
bool IsHit(const Position2& posA, float radiusA, const Position2& posB,  float radiusB) {
	//当たり判定を実装してください
	auto tmp = posA - posB;
	return hypot(tmp.x, tmp.y) <= radiusA + radiusB;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	ChangeWindowMode(true);
	SetMainWindowText("1916035_橋本大輝");
	if (DxLib_Init() != 0) {
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);

	//背景用
	int bgH[4];
	LoadDivGraph("img/bganim.png", 4, 4, 1, 256, 192, bgH);

	int skyH = LoadGraph("img/sky.png");
	int sky2H = LoadGraph("img/sky2.png");

	int playerH[10];
	LoadDivGraph("img/player.png", 10, 5, 2, 16, 24,playerH );

	int enemyH[2];
	LoadDivGraph("img/enemy.png", 2, 2, 1, 32, 32, enemyH);

	//自機の半径
	float playerRadius = 10.0f;
	

	Position2 enemypos(320,25);//敵座標
	Position2 playerpos(320, 400);//自機座標

	unsigned int frame = 0;//フレーム管理用

	char keystate[256];
	bool isDebugMode = false;
	int skyy = 0;
	int skyy2 = 0;
	int bgidx = 0;

	int _screen = MakeScreen(640, 480, true);
	int _shakeCount = 0;
	Vector2 _screenPos = { 0,0 };

	BULLET_MOVE _moveType = BULLET_MOVE::Normal;

	while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		SetDrawScreen(_screen);
		ClearDrawScreen();

		GetHitKeyStateAll(keystate);

		isDebugMode = keystate[KEY_INPUT_P];

		//背景
		DrawExtendGraph(0, 0, 640, 480, bgH[bgidx / 8], false);
		bgidx = (bgidx + 1) % 32;

		//SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		skyy = (skyy + 1) % 480;
		skyy2 = (skyy2 + 2) % 480;
		DrawExtendGraph(0, skyy, 640, skyy + 480, skyH, true);
		DrawExtendGraph(0, skyy - 480, 640, skyy, skyH, true);
		DrawExtendGraph(0, skyy2, 640, skyy2 + 480, sky2H, true);
		DrawExtendGraph(0, skyy2 - 480, 640, skyy2, sky2H, true);


		//プレイヤー
		if (keystate[KEY_INPUT_RIGHT]) {
			playerpos.x = min(640,playerpos.x+4);
		}
		else if (keystate[KEY_INPUT_LEFT]) {
			playerpos.x = max(0,playerpos.x-4);
		}
		if (keystate[KEY_INPUT_UP]) {
			playerpos.y = max(0,playerpos.y-4);
		}else if (keystate[KEY_INPUT_DOWN]) {
			playerpos.y = min(480,playerpos.y+4);
		}

		int pidx = (frame/4 % 2)*5+3;
		DrawRotaGraph(playerpos.x, playerpos.y, 2.0f, 0.0f, playerH[pidx], true);
		if (isDebugMode) {
			//自機の本体(当たり判定)
			DrawCircle(playerpos.x, playerpos.y, playerRadius, 0xffaaaa, false, 3);
		}

		//弾発射
		if (frame % lpBulMng.GetBulCnt(_moveType) == 0) {
			lpBulMng.AddBulQue(_moveType, enemypos, playerpos);
		}
		lpBulMng.UpDate();
		if (lpBulMng.HitCheck(playerpos, playerRadius))
		{
			_shakeCount = 20;
		}
		lpBulMng.Draw(isDebugMode);
		//敵の表示
		enemypos.x = abs((int)((frame+320) % 1280) - 640);
		int eidx = (frame / 4 % 2);
		DrawRotaGraph(enemypos.x, enemypos.y, 2.0f, 0.0f, enemyH[eidx],true);

		if (isDebugMode) {
			//敵の本体(当たり判定)
			DrawCircle(enemypos.x, enemypos.y, 5, 0xffffff, false, 3);
		}

		if (frame % 300 == 299)
		{
			++_moveType;
			if (_moveType >= BULLET_MOVE::MAX)
			{
				_moveType = BULLET_MOVE::Normal;
			}
		}

		if (_shakeCount)
		{
			SetDrawBlendMode(DX_BLENDMODE_ADD,128);
			DrawBox(0, 0, 640, 480, 0xff0000,true);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND,255);
			_shakeCount--;
			_screenPos = { static_cast<float>(rand() % 20 - 10),static_cast<float>(rand() % 20 - 10) };
			if (!_shakeCount)
			{
				// ずれたままにならないように補正
				_screenPos = { 0,0 };
			}
		}

		++frame;
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		DrawGraph(_screenPos.x,_screenPos.y,_screen,true);
		ScreenFlip();
	}

	DxLib::DxLib_End();

	return 0;
}