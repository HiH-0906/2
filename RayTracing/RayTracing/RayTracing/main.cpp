#include<dxlib.h>
#include <cmath>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;

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



UINT32 GetUINT32ColorFromVectorColor(const Color col)
{
	return GetColor(col.x, col.y, col.z);
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const Sphere& sphere,int& image) {
	Vector3 Light = Vector3{ 1,-1,-1 };
	float albedo[3] = { 0.5f , 0.5f, 1.0f };

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
				for (int i = 0; i < 3; i++)
				{
					deiffuse[i] = (deiffuseB * albedo[i]) + specular;
					deiffuse[i] = Clamp(deiffuse[i]);
				}

				//※塗りつぶしはDrawPixelという関数を使う。
				int color = 0xff * deiffuse[0];
				color = color << 8;
				color += 0xff * deiffuse[1];
				color = color << 8;
				color += 0xff * deiffuse[2];
				
				auto R = ReflectVector(L, N);

				/*Color difColor(255, 128, 128);
				Color specColor(255, 255, 255);

				difColor *= deiffuseB;
				specColor *= specular;
				difColor *= deiffuseB;
				color = GetUINT32ColorFromVectorColor(Clamp(difColor + specColor, 0.0f, 255.0f));*/
				DrawPixel(x, y, color);
			}
			// 球体に当たらなかった
			else
			{
				// 平面に当たる条件
				// 仮に平面の法線を(0,1,0)
				// 平面に当たる条件は視線と法線ベクトルが90度以上

				// 視線と平面の交点
				Plane plane(Vector3(0, 1, 0), 100);
				auto dot = Dot(ray, plane.N);
				if (dot < 0.0f)	// 地面に当たってる
				{
					int r, g, b, a;
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


					Color col(255, 255, 255);
					col *= Clamp(tmp / 3000.0f);

					GetPixelSoftImage(image, abs(((int)(P.x) % 120)), abs((int)(P.z) % 120), &r, &g, &b, &a);

				
					auto checker = (static_cast<int>(P.x / 120) + static_cast<int>(P.z / 120)) % 2 == 0;
					checker = P.x < 0 ? !checker : checker;
					checker = P.z < 0 ? !checker : checker;
					if (checker)
					{
						DrawPixel(x, y, /*0xffffff*/GetColor(r,g,b));
					}
					else
					{
						DrawPixel(x, y, /*0x000000*/GetColor(r, g, b));
					}
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
	int image = LoadSoftImage(L"image/image.png");
	auto eye = Vector3(0, 0, 300);
	auto sphere = Sphere(100, Position3(0, 0, -100));
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();
		RayTracing(eye, sphere, image);
		ScreenFlip();
	}
	DxLib_End();
}