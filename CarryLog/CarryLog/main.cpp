#include<DxLib.h>
#include<algorithm>
#include<cmath>
#include<memory>
#include<vector>
#include<time.h>
#include"Geometry.h"
#include"Rock.h"


using namespace std;

void DrawWood(const Capsule& cap, int handle) {


	auto v = cap.posB - cap.posA;
	auto angle = atan2(v.y, v.x) + DX_PI_F / 2.0f;
	auto w = cos(angle)*cap.radius;
	auto h = sin(angle)*cap.radius;

	DrawModiGraph(
		cap.posA.x - w, cap.posA.y - h,
		cap.posB.x - w, cap.posB.y - h,
		cap.posB.x + w, cap.posB.y + h,
		cap.posA.x + w, cap.posA.y + h, handle,true);
}

///とある点を中心に回転して、その回転後の座標を返します。
///@param center 回転中心点
///@param angle 回転角度
///@param pos 元の座標
///@return 変換後の座標
Matrix RotatePosition(const Position2& center, float angle) {
	//①中心を原点に平行移動して
	//②原点中心に回転して
	//③中心を元の座標へ戻す

	Matrix mat = MultipleMat(TranslateMat(center.x, center.y),
		MultipleMat(RotateMat(angle),
			TranslateMat(-center.x, -center.y)));
	return mat;
	//これを書き換えて、特定の点を中心に回転を行うようにしてください。
}

float Clamp(float value, float minVal = 0.0f, float maxVal = 1.0f) {
	//今は値をそのまま返していますが、クランプ(最小値最大値の範囲を超えたら最小値もしくは最大値にする)した
	//値を返してください。
	return min(maxVal, max(minVal, value));
}

//カプセルと円が当たったか？
bool IsHit(const Capsule& cap, const Circle& cc) {
	auto vp = cc.pos - cap.posA;
	auto v = cap.posB - cap.posA;
	auto vv = v * (Clamp(Dot(v, vp) / Dot(v, v)));
	auto d = (vp - vv).Magnitude();
	return d <= cap.radius + cc.radius;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(true);
	SetGraphMode(512, 800, 32);
	DxLib_Init();
	SetWindowText("1916035_橋本大輝");
	SetDrawScreen(DX_SCREEN_BACK);


	using ShareRock = std::shared_ptr<Rock>;
	
	std::vector<ShareRock> rockList_;

	int sw, sh;//スクリーン幅、高さ
	GetDrawScreenSize(&sw, &sh);

	auto woodH = LoadGraph("img/wood.png");
	int wdW, wdH;
	GetGraphSize(woodH, &wdW, &wdH);
	wdW = 200;

	auto cascadeH = LoadGraph("img/cascade_chip.png");
	auto chipH = LoadGraph("img/atlas0.png");
	auto rockH = LoadGraph("img/rock.png");
	auto clearH = LoadGraph("img/clear.png");
	auto spaceH = LoadGraph("img/space.png");
	int explosionH[12];
	LoadDivGraph("img/explosion.png", 12, 12, 1, 128, 128, explosionH);

	Capsule cap(20,Position2((sw-wdW)/2,sh-100),Position2((sw - wdW) / 2+wdW,sh-100));

	char keystate[256];
	
	float angle = 0.0f;

	int frame = 0;

	bool isLeft = false;
	bool alive = true;

	Position2 bPos;
	int bcnt = 0;
	bool bFlag = false;

	bool clear = false;

	int fcnt = 0;

	int rockCnt = 60;
	srand((unsigned int)time(NULL));
	while (ProcessMessage() == 0 && !CheckHitKey(KEY_INPUT_ESCAPE)) {
		ClearDrawScreen();
		GetHitKeyStateAll(keystate);

		DrawBox(0, 0, sw, sh, 0xaaffff, true);

		int mx = 0, my = 0;
		if (!clear)
		{
			if (keystate[KEY_INPUT_LEFT]) {
				isLeft = true;
			}
			else if (keystate[KEY_INPUT_RIGHT]) {
				isLeft = false;
			}

			if (isLeft) {
				mx = cap.posA.x;
				my = cap.posA.y;
			}
			else {
				mx = cap.posB.x;
				my = cap.posB.y;
			}
			for (auto rock : rockList_)
			{
				rock->UpDate();
			}
			if (alive)
			{
				if (keystate[KEY_INPUT_Z]) {

					angle = -0.05f;
				}
				else if (keystate[KEY_INPUT_X]) {

					angle = 0.05f;
				}
				else {
					angle = 0.0f;
				}


				if (frame % (rockCnt + 60) == 0 && rockList_.size() < 3)
				{
					rockList_.emplace_back(make_shared<Rock>(Circle{ 24,Position2{static_cast<float>(32 + rand() % 400),36.0f} }, Vector2{ 0,3 }));
					rockCnt = rand() % 120;
				}
				for (auto rock : rockList_)
				{
					//当たり判定を完成させて当たったときの反応を書いてください
					if (IsHit(cap, rock->data())) {
						//反応をここに書いてください。
						bPos = rock->pos();
						rock->alive(false);
						bFlag = true;
						alive = false;
					}
				}

				//カプセル回転
				Matrix rotMat = RotatePosition(Position2(mx, my), angle);
				cap.posA = MultipleVec(rotMat, cap.posA);
				cap.posB = MultipleVec(rotMat, cap.posB);
				auto v = cap.posB - cap.posA;
				if (v.x <= 10)
				{
					alive = false;
				}
				if (cap.posA.x < 32 || cap.posB.x > 480 || (cap.posA.y > 800 && cap.posB.y > 800))
				{
					alive = false;
				}
			}
			else
			{
				cap.posA.y += 10.0f;
				cap.posB.y += 10.0f;

				auto v = cap.posB - cap.posA;
				auto angle = atan2(v.y, v.x);
				Matrix rotMat = RotatePosition(Position2(cap.posA.x + (cos(angle) * 100), cap.posA.y + (sin(angle) * 100)), 0.3f);
				cap.posA = MultipleVec(rotMat, cap.posA);
				cap.posB = MultipleVec(rotMat, cap.posB);
				if (cap.posA.y > 1100 && cap.posB.y > 1100)
				{
					cap.posA = Position2((sw - wdW) / 2, sh - 100);
					cap.posB = Position2((sw - wdW) / 2 + wdW, sh - 100);
					alive = true;
				}
			}
			if (cap.posA.y < 30 && cap.posB.y < 30)
			{
				clear = true;
			}
		}
		//背景の描画
		//滝
		int chipIdx = (frame/4) % 3;
		constexpr int dest_chip_size = 32;
		int destY = 48;
		while (destY < sh) {
			int destX = 0;
			while (destX < sw) {
				DrawRectExtendGraph(
					destX, destY, destX + dest_chip_size, destY + dest_chip_size,
					chipIdx * 16, 0,
					16, 16,
					cascadeH, false);
				destX += dest_chip_size;
			}
			destY += dest_chip_size;
		}

		DrawRectGraph(0, 0, 96, 0, 32, 32, chipH, true);
		DrawRectGraph(sw - 32, 0, 32, 0, 32, 32, chipH, true);
		DrawRectGraph(0, 32, 96, 32, 32, 32, chipH, true);
		DrawRectGraph(sw-32, 32, 32, 32, 32, 32, chipH, true);
		
		destY = 64;
		while (destY < sh) {
			DrawRectGraph(0, destY, 96, 64, 32, 32, chipH, true);
			DrawRectGraph(sw - 32, destY, 32, 64, 32, 32, chipH, true);
			destY += dest_chip_size;
		}
		DrawWood(cap, woodH);
		if (!clear)
		{
			for (auto rock : rockList_)
			{
				DrawRotaGraph(rock->data().pos.x, rock->data().pos.y, rock->rate(), rock->angle(), rockH, true);
			}

			DrawCircle(mx, my, 30, 0xff0000, false, 3);
			if (bFlag)
			{
				DrawRotaGraph(bPos.x, bPos.y, 1.0, 0, explosionH[bcnt / 3], true);
				bcnt++;
				if (bcnt / 3 >= 12)
				{
					bFlag = false;
					bcnt = 0;
				}
			}
		}
		else
		{
			DrawGraph(20, 200, clearH, true);
			if (fcnt / 30 % 2)
			{
				DrawGraph(0, 400, spaceH, true);
			}
			fcnt++;
			if(CheckHitKey(KEY_INPUT_SPACE))
			{
				clear = false;
				cap.posA = Position2((sw - wdW) / 2, sh - 100);
				cap.posB = Position2((sw - wdW) / 2 + wdW, sh - 100);
				rockList_.clear();
				fcnt = 0;
			}
		}
		++frame;
		auto itr = std::remove_if(rockList_.begin(), rockList_.end(), [](ShareRock rock) {return !rock->alive(); });
		rockList_.erase(itr, rockList_.end());
		ScreenFlip();
	}

	DxLib_End();


}