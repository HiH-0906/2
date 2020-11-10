#include<dxlib.h>
#include <cmath>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;

//ヒントになると思って、色々と関数を用意しておりますが
//別にこの関数を使わなければいけないわけでも、これに沿わなければいけないわけでも
//ありません。レイトレーシングができていれば構いません。

///レイ(光線)と球体の当たり判定
///@oaram eye 視点
///@param ray (視点からスクリーンピクセルへのベクトル)
///@param sphere 球
///@hint レイは正規化しといたほうが使いやすいだろう
///@param t 視点から交点までの距離
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Sphere& sp,float& t) {
	//レイが正規化済みである前提で…
	//
	//視点から球体中心へのベクトルを作ります
	//
	auto c = sp.pos - eye;
	//中心から視線への内積をとります＝＞射影長（ベクトル長として利用）
	auto d = Dot(c, ray);
	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	auto r = ray * d;
	r = c - r;
	if (r.Magnitude() <= sp.radius)
	{
		// tを求めたい
		// wを求める。玉の半径とベクトルVの大きさから求まる
		// 円までの中心から視線ベクトルに落とした射影の長さからwを引けばtになります
		auto w = sqrt(sp.radius * sp.radius - Dot(r,r));
		t = d - w;
		return true;
	}
	return false;
}

float Clamp(float in, const float min = 0.0f, const float max = 1.0f) {
	return max(min(in, max), min);
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const Sphere& sphere) {
	Vector3 Light = Vector3{ 1,-1,-1 };
	for (int y = 0; y < screen_height; ++y) {//スクリーン縦方向
		for (int x = 0; x < screen_width; ++x) {//スクリーン横方向
			//①視点とスクリーン座標から視線ベクトルを作る
			auto ray = Position3(x - screen_width / 2, screen_height / 2 - y, 0) - eye;
			//②正規化しとく
			ray.Normalize();
			//③IsHitRayAndObject関数がTrueだったら白く塗りつぶす
			float t;
			if (IsHitRayAndObject(eye, ray, sphere, t))
			{
				// 交点を求める
				auto P = eye + ray * t;
				// 交点が分かったので法線ベクトルを求める
				auto N = P - sphere.pos;
				N.Normalize();
				auto L = Light.Normalized();
				auto blink = Dot(N, -L);

				blink = Clamp(blink);

				//※塗りつぶしはDrawPixelという関数を使う。
				int color = 0xff * blink;
				color = color << 8;
				color += 0xff * blink;
				color = color << 8;
				color += 0xff * blink;
				
				DrawPixel(x, y, color);
			}
			else
			{
				if ((x / 40 + y / 40) % 2 == 0)
				{
					DrawPixel(x, y, 0x007700);
				}
			}
		}
	}
}

int WINAPI WinMain(HINSTANCE , HINSTANCE,LPSTR,int ) {
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("簡易版のレイトレでっせ"));
	DxLib_Init();

	auto eye = Vector3(0, 0, 300);
	auto sphere = Sphere(100, Position3(0, 0, -100));
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();
		RayTracing(eye, sphere);
		ScreenFlip();
	}
	DxLib_End();
}