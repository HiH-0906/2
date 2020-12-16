#include<dxlib.h>
#include <cmath>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;

namespace
{
	int image;
	Vector3 Light = Vector3{ 1,-1,-1 };
	float albedo[3] = { 0.5f , 0.5f, 1.0f };
	// 視線と平面の交点
	Plane plane(Vector3(0, 1, 0), 100);
}

// 反射ベクトルを返す
// in：	入射ベクトル
// normal：法線ベクトル
// 返り値：反射ベクトル
Vector3 ReflectVector(const Vector3& in, const Vector3& normal)
{
	// 反射ベクトルの式
	// R=I-2*(N・I)N
	// をそのままプログラムにする
	// なおオペレーターオーバーロードの関係で実数*ベクトルはできないので注意してね
	// 結果のRを返す関数を作る
	return in - (normal * 2 * (Dot(normal, in)));
}


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

using Color = Vector3;

Color Clamp(const Color& in, const float min = 0.0f, const float max = 1.0f)
{
	return Color(Clamp(in.x, min, max), Clamp(in.y, min, max), Clamp(in.z, min, max));
}

Color GetCheckerdColorFromPosition(const Position3& pos)
{
	Color col1(255, 255, 255);
	Color col2(0, 0, 0);
	auto checker = (static_cast<int>(pos.x / 30) + static_cast<int>(pos.z / 30)) % 2 == 0;
	checker = pos.x < 0 ? !checker : checker;
	checker = pos.z < 0 ? !checker : checker;
	if (checker)
	{
		return col1;
	}
	else
	{
		return col2;
	}
}

UINT32 GetUINT32ColorFromVectorColor(const Color col)
{
	return GetColor(col.x, col.y, col.z);
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const Sphere& sphere) {

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
				auto deiffuseB = Dot(N, -L);
				
				// ライトの反射ベクトルと視線の逆ベクトルの内積を取りpowでn乗する。オーバーフローしないように気を付ける
				auto specular = Dot(ReflectVector(L, N), -ray);

				specular = Clamp(specular);
				specular = pow(specular, 10);
				float deiffuse[3];

				// 反射を考える
				// 反射ベクトルを作る
				auto Rray = ReflectVector(ray, N);
				// 反射ベクトルが地面と当たるかを考える
				auto dot = Dot(Rray, plane.N);
				// まずははんしゃさきが地面なら色を変える
				if (dot < 0.0f)
				{
					// 交点P=eye+ray*t
					// t=w/u (wは平面からの距離　　uは一回当たり平面に近づく大きさ)
					// w=eye・N u=-ray・N
					// t=w/uにしたいけどdがあるので
					// t=(w+d)/u
					// あとはここからPを求めればよい
					// まずｔを求める。多分tは４万くらい
					auto w = Dot(P, plane.N);
					auto u = Dot(-Rray, plane.N);
					auto tmp = (w + plane.d) / u;
					P = P + Rray * tmp;

					for (int i = 0; i < 3; i++)
					{
						deiffuse[i] = (deiffuseB * albedo[i]) + specular;
						deiffuse[i] = Clamp(deiffuse[i]);
					}
					//※塗りつぶしはDrawPixelという関数を使う。
					Color color(255 * deiffuse[0], 255 * deiffuse[1], 255 * deiffuse[2]);
					auto spharCol = GetUINT32ColorFromVectorColor(color);

					Color col = GetCheckerdColorFromPosition(P);
					float reflectivity = 0.5f;
					auto tmpCol = GetUINT32ColorFromVectorColor(col);

					//tmpCol = spharCol * (1.0f - reflectivity) + tmpCol * reflectivity;

					DrawPixel(x, y, tmpCol);
				}
				else
				{
					for (int i = 0; i < 3; i++)
					{
						deiffuse[i] = (deiffuseB * albedo[i]) + specular;
						deiffuse[i] = Clamp(deiffuse[i]);
					}
					//※塗りつぶしはDrawPixelという関数を使う。
					Color color(255 * deiffuse[0], 255 * deiffuse[1], 255 * deiffuse[2]);

					auto R = ReflectVector(L, N);
					DrawPixel(x, y, GetUINT32ColorFromVectorColor(color));
				}
			}
			// 球体に当たらなかった
			else
			{
				// 平面に当たる条件
				// 仮に平面の法線を(0,1,0)
				// 平面に当たる条件は視線と法線ベクトルが90度以上

				auto dot = Dot(ray, plane.N);
				if (dot < 0.0f)	// 地面に当たってる
				{
					// 交点P=eye+ray*t
					// t=w/u (wは平面からの距離　　uは一回当たり平面に近づく大きさ)
					// w=eye・N u=-ray・N
					// t=w/uにしたいけどdがあるので
					// t=(w+d)/u
					// あとはここからPを求めればよい
					// まずｔを求める。多分tは４万くらい
					auto w = Dot(eye, plane.N);
					auto u = Dot(-ray, plane.N);
					auto tmp = (w + plane.d) / u;
					auto P = eye + ray * tmp;


					Color col = GetCheckerdColorFromPosition(P);

					DrawPixel(x, y, GetUINT32ColorFromVectorColor(col));
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
}

int WINAPI WinMain(HINSTANCE , HINSTANCE,LPSTR,int ) {
	ChangeWindowMode(true);
	SetGraphMode(screen_width, screen_height, 32);
	SetMainWindowText(_T("簡易版のレイトレでっせ"));
	DxLib_Init();
	image = LoadSoftImage(L"image/test.png");
	auto eye = Vector3(0, 0, 300);
	auto sphere = Sphere(100, Position3(0, 50, -100));
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();
		if (CheckHitKey(KEY_INPUT_UP))
		{
			sphere.pos.y+=5;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			sphere.pos.y-=5;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			sphere.pos.x-=5;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			sphere.pos.x+=5;
		}
		if (CheckHitKey(KEY_INPUT_Z))
		{
			sphere.pos.z += 5;
		}
		if (CheckHitKey(KEY_INPUT_X))
		{
			sphere.pos.z -= 5;
		}
		RayTracing(eye, sphere);
		ScreenFlip();
	}
	DxLib_End();
}