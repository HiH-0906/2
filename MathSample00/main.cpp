#include <DxLib.h> 
#include <cmath>
#include "Geometry.h"

/// <summary>
/// 矩形と矩形の当たり判定
/// </summary>
/// <param name="rcA">矩形A</param>
/// <param name="rcB">矩形B</param>
/// <returns>
/// rcAとrcBが重なっている:true
/// rcAとrcBが重なっていない:false
/// </returns>
bool isHitAABB(const Rect& rcA, const Rect& rcB)
{
	Vector2 biff = rcA.Center() - rcB.Center();
	return !((abs(biff.x) > (rcA.width + rcB.width)/2) || (abs(biff.y) > (rcA.height + rcB.height)/2));
	//return !(min(rcA.Right(), rcB.Right()) < max(rcA.Left(), rcB.Left()) ||
	//		min(rcA.Bottom(), rcB.Bottom()) < max(rcA.Top(), rcB.Top()));
}

bool isHitCircles(const Circle& cA, const Circle& cB)
{
	float xbiff = cA.center.x - cB.center.x;
	float ybiff = cA.center.y - cB.center.y;
	return hypot(xbiff, ybiff) <= cA.r + cB.r;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	DxLib::ChangeWindowMode(true);
	DxLib::SetWindowText("矩形の当たり判定");
	if (DxLib::DxLib_Init())
	{
		return -1;
	}
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	Rect rcA = { 0,0,100,100 };
	Rect rcB = { 300,300,80,80 };

	Circle cA = { {100.0f,100.0f},50.0f };
	Circle cB = { {300.0f,300.0f},50.0f };

	Vector2 img = { 300.0f,250.0f };

	int image;
	image = LoadGraph("image/test.png");

	int speed = 4;

	while (!DxLib::ProcessMessage())
	{
		DxLib::ClearDrawScreen();
		char keystate[256];
		DxLib::GetHitKeyStateAll(keystate);
		int tmpx = 0;
		int tmpy = 0;
		if (keystate[KEY_INPUT_UP])
		{
			rcA.top -= speed;
			cA.center.y -= speed;
			tmpy -= speed;
		}
		if (keystate[KEY_INPUT_DOWN])
		{
			rcA.top += speed;
			cA.center.y += speed;
			tmpy += speed;
		}
		if (keystate[KEY_INPUT_LEFT])
		{
			rcA.left -= speed;
			cA.center.x -= speed;
			tmpx -= speed;
		}
		if (keystate[KEY_INPUT_RIGHT])
		{
			rcA.left += speed;
			cA.center.x += speed;
			tmpx += speed;
		}
		if (keystate[KEY_INPUT_W])
		{
			rcB.top -= speed;
			cB.center.y -= speed;
		}
		if (keystate[KEY_INPUT_S])
		{
			rcB.top += speed;
			cB.center.y += speed;
		}
		if (keystate[KEY_INPUT_A])
		{
			rcB.left -= speed;
			cB.center.x -= speed;
		}
		if (keystate[KEY_INPUT_D])
		{
			rcB.left += speed;
			cB.center.x += speed;
		}

		unsigned int color = 0xffffff;

		/*if (isHitAABB(rcA,rcB))
		{
			color = 0xc9171e;
			if (0 < tmpx)
			{
				rcA.left -= rcA.Right() - rcB.Left();
			}
			else if(0 > tmpx)
			{
				rcA.left += rcB.Right() - rcA.Left();
			}
			if (0 < tmpy)
			{
				rcA.top -= rcA.Bottom() - rcB.Top();
			}
			else if (0 > tmpy)
			{
				rcA.top += rcB.Bottom() - rcA.Top();
			}
		}*/
		if (isHitCircles(cA, cB))
		{
			color = 0xc9171e;
		}
		// 円A
		DxLib::DrawCircle(cA.center.x, cA.center.y, cA.r, color);

		// 円B
		DxLib::DrawCircle(cB.center.x, cB.center.y, cB.r, color);

		int mousex, mousey =  0;

		DxLib::GetMousePoint(&mousex, &mousey);

		img.x += speed * cos(atan2(static_cast<float>(mousey - img.y), static_cast<float>(mousex - img.x)));
		img.y += speed * sin(atan2(static_cast<float>(mousey - img.y), static_cast<float>(mousex - img.x)));

		auto angle = atan2(static_cast<float>(mousey - img.y), static_cast<float>(mousex - img.x));

		//DxLib::DrawRotaGraph(img.x, img.y,1.0f, angle, image, true);

		//// 矩形A,B
		//DxLib::DrawBox(rcA.Left(), rcA.Top(), rcA.Right(), rcA.Bottom(), color, true);
		//DxLib::DrawBox(rcB.Left(), rcB.Top(), rcB.Right(), rcB.Bottom(), color, true);

		DxLib::ScreenFlip();
	}

	return 0;
}