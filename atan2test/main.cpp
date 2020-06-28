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
	int speed = 0;
	Position2 center = { 320,240 };
	Position2 target;

	while (!DxLib::ProcessMessage())
	{
		DxLib::ClearDrawScreen();
		
		DxLib::GetMousePoint(&target.x, &target.y);

		center.x += speed * cos(atan2(target.y - center.y, target.x - center.x));
		center.y += speed * sin(atan2(target.y - center.y, target.x - center.x));

		auto angle = atan2f(target.y - center.y, target.x - center.x);
		DxLib::DrawRotaGraph(center.x, center.y, 1.0f, angle, image, true);
		DxLib::ScreenFlip();
	}
	return 0;
}