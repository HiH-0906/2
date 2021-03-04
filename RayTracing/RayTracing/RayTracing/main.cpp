#include<dxlib.h>
#include <cmath>
#include <vector>
#include <utility>
#include <functional>
#include"Geometry.h"
#include "Primitive.h"

const int screen_width = 640;
const int screen_height = 480;

using Primitive_t = std::vector<Primitive*>;

namespace
{
	int image;
	Vector3 Light = Vector3{ 1,-1,-1 };
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


float Clamp(float in, const float min = 0.0f, const float max = 1.0f) {
	return max(min(in, max), min);
}

using Color = Vector3;

Color Clamp(const Color& in, const float min = 0.0f, const float max = 1.0f)
{
	return Color(Clamp(in.x, min, max), Clamp(in.y, min, max), Clamp(in.z, min, max));
}

Vector3 GetCheckerdColorFromPosition(const Position3& pos,const Material& mat)
{
	if (mat.pattern != Pattern::checker)
	{
		return mat.baceCol;
	}
	auto checker = (static_cast<int>(pos.x / mat.patternBlockSize) + static_cast<int>(pos.z / mat.patternBlockSize)) % 2 == 0;
	checker = pos.x < 0 ? !checker : checker;
	checker = pos.z < 0 ? !checker : checker;
	if (checker)
	{
		return mat.baceCol;
	}
	else
	{
		return mat.subCol;
	}
}

Vector3 Max(const Vector3& av, const Vector3& bv)
{
	return Vector3{ max(av.x,bv.x),max(av.y,bv.y) ,max(av.z,bv.z) };
}

UINT32 GetUINT32ColorFromVectorColor(const Color col)
{
	return GetColor(static_cast<unsigned int>(col.x), static_cast<unsigned int>(col.y), static_cast<unsigned int>(col.z));
}

bool Trace(const Ray& ray, const Primitive_t& primitives, Vector3& refCol,int limit = 5, Primitive* self = nullptr, bool isShadow = false)
{
	std::pair<float, std::function<Vector3(void)>> depthAndColorFunc = std::make_pair(FLT_MAX, []()->Vector3 {return Black; });
	bool onceHit = false;
	for (auto& prim : primitives)
	{
		if (prim == self)
		{
			continue;
		}
		if (limit < 0)
		{
			return false;
		}
		float t = 0.0f;
		Vector3 norm;
		Vector3 intersection;
		if (prim->IsHit(ray,t,norm, intersection))
		{
			if (isShadow)
			{
				return true;
			}
			if (t < depthAndColorFunc.first)
			{
				depthAndColorFunc.first = t;
				depthAndColorFunc.second = [ray, t, norm, intersection, &primitives, &prim, limit]()
				{
					Vector3 refCol;
					auto& mtr = prim->matrial;
					float diff = Clamp(Dot(norm, -Light));
					auto half = -(Light + ray.vec).Normalized();
					float spec = pow(Clamp(Dot(half, norm)), mtr.specularity);
					auto baseCol = GetCheckerdColorFromPosition(intersection, mtr);
					refCol = (Clamp((baseCol * diff + (mtr.specCol * spec))) * 255);
					if (mtr.reflectivity > 0.0f)
					{
						Ray rray(intersection, ReflectVector(ray.vec, norm));
						Vector3 reflectColor;
						if (Trace(rray, primitives, reflectColor, limit - 1, prim))
						{
							refCol = (refCol * (1.0f - mtr.reflectivity)) + (reflectColor * mtr.reflectivity);
						}
					}
					Vector3 dummyCol;
					Ray shadowRay = Ray(intersection, -Light.Normalized());
					if (Trace(shadowRay, primitives, dummyCol, 0, prim, true))
					{
						refCol *= 0.5f;
					}
					return refCol;
				};
			}
			onceHit = true;
		}
	}
	if (onceHit)
	{
		refCol = depthAndColorFunc.second();
	}
	return onceHit;
}

///レイトレーシング
///@param eye 視点座標
///@param sphere 球オブジェクト(そのうち複数にする)
void RayTracing(const Position3& eye,const Primitive_t& primitives) {

	for (int y = 0; y < screen_height; ++y) {//スクリーン縦方向
		for (int x = 0; x < screen_width; ++x) {//スクリーン横方向
			//①視点とスクリーン座標から視線ベクトルを作る
			auto ray = Position3(x - screen_width / 2, screen_height / 2 - y, 0) - eye;
			//②正規化しとく
			Ray tmpRay(eye, ray.Normalized());
			Vector3 color = {};
			if (Trace(tmpRay, primitives, color))
			{
				UINT32 intCol = GetUINT32ColorFromVectorColor(color);
				DrawPixel(x, y, intCol);
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
	SetMainWindowText(_T("1916035_橋本大輝"));
	DxLib_Init();
	auto eye = Vector3(0, 0, 300);
	// 球体
	auto sphere = Sphere(100.0f, Position3(0, 50, -100));
	sphere.matrial.baceCol = { 0.5f,0.5f,1.0f };
	sphere.matrial.reflectivity = 0.5f;
	auto sphere2 = Sphere(100.0f, Position3(250, 50, -100));
	sphere2.matrial.baceCol = { 1.0f,0.5f,0.5f };
	sphere2.matrial.reflectivity = 0.8f;
	auto sphere3 = Sphere(100.0f, Position3(-250, 50, -100));
	sphere3.matrial.baceCol = { 0.5f,1.0f,0.5f };
	sphere3.matrial.reflectivity = 0.2f;
	// 平面
	Plane plane(Vector3(0, 1, 0), 100);
	plane.matrial.pattern = Pattern::checker;
	plane.matrial.specCol;
	Primitive_t primitives_;
	primitives_.push_back(&sphere);
	primitives_.push_back(&sphere2);
	primitives_.push_back(&sphere3);
	primitives_.push_back(&plane);
	int speed = 10;
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		ClsDrawScreen();
		if (CheckHitKey(KEY_INPUT_UP))
		{
			sphere.pos.y+= speed;
		}
		if (CheckHitKey(KEY_INPUT_DOWN))
		{
			sphere.pos.y-= speed;
		}
		if (CheckHitKey(KEY_INPUT_LEFT))
		{
			sphere.pos.x-= speed;
		}
		if (CheckHitKey(KEY_INPUT_RIGHT))
		{
			sphere.pos.x+= speed;
		}
		if (CheckHitKey(KEY_INPUT_Z))
		{
			sphere.pos.z += speed;
		}
		if (CheckHitKey(KEY_INPUT_X))
		{
			sphere.pos.z -= speed;
		}
		RayTracing(eye, primitives_);
		ScreenFlip();
	}
	DxLib_End();
}