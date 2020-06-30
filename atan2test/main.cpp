#include <DxLib.h> 
#include <cmath>
#include "Geometry.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText(L"‹éŒ`‚Ì“–‚½‚è”»’è");
	if (DxLib::DxLib_Init())
	{
		return -1;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	int image;
	image = LoadGraph(L"image/test.png");
	int speed = 4;
	Position2 catPos = { 320,240 };
	Position2 target = {};
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	while (!DxLib::ProcessMessage()&&!CheckHitKey(KEY_INPUT_ESCAPE))
	{
		DxLib::ClearDrawScreen();
		
		DxLib::GetMousePoint(&target.x, &target.y);

		//DrawCircle(catPos.x, catPos.y, 100, 0xffffff);

		/*auto vCat = target - center;

		center.x += static_cast<int>(speed * cos(atan2(target.y - center.y, target.x - center.x)));
		center.y += static_cast<int>(speed * sin(atan2(target.y - center.y, target.x - center.x)));

		auto tmp = target - center;

		auto angle = atan2f(tmp.y, tmp.x );*/

		//DxLib::DrawRotaGraph(center.x, center.y, 1.0f, angle, image, true);
		DxLib::ScreenFlip();
	}
	return 0;
}