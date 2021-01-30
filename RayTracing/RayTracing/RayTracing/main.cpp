#include<dxlib.h>
#include <cmath>
#include"Geometry.h"
#include "Primitive.h"
const int screen_width = 640;
const int screen_height = 480;

namespace
{
	int image;
	Vector3 Light = Vector3{ 1,-1,-1 };
	float albedo[3] = { 0.5f , 0.5f, 1.0f };
	// �����ƕ��ʂ̌�_
	Plane plane(Vector3(0, 1, 0), 100);
}

// ���˃x�N�g����Ԃ�
// in�F	���˃x�N�g��
// normal�F�@���x�N�g��
// �Ԃ�l�F���˃x�N�g��
Vector3 ReflectVector(const Vector3& in, const Vector3& normal)
{
	// ���˃x�N�g���̎�
	// R=I-2*(N�EI)N
	// �����̂܂܃v���O�����ɂ���
	// �Ȃ��I�y���[�^�[�I�[�o�[���[�h�̊֌W�Ŏ���*�x�N�g���͂ł��Ȃ��̂Œ��ӂ��Ă�
	// ���ʂ�R��Ԃ��֐������
	return in - (normal * 2 * (Dot(normal, in)));
}

//�q���g�ɂȂ�Ǝv���āA�F�X�Ɗ֐���p�ӂ��Ă���܂���
//�ʂɂ��̊֐����g��Ȃ���΂����Ȃ��킯�ł��A����ɉ���Ȃ���΂����Ȃ��킯�ł�
//����܂���B���C�g���[�V���O���ł��Ă���΍\���܂���B

///���C(����)�Ƌ��̂̓����蔻��
///@oaram eye ���_
///@param ray (���_����X�N���[���s�N�Z���ւ̃x�N�g��)
///@param sphere ��
///@hint ���C�͐��K�����Ƃ����ق����g���₷�����낤
///@param t ���_�����_�܂ł̋���
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Sphere& sp,float& t) {
	//���C�����K���ς݂ł���O��Łc
	//
	//���_���狅�̒��S�ւ̃x�N�g�������܂�
	//
	auto c = sp.pos - eye;
	//���S���王���ւ̓��ς��Ƃ�܂������ˉe���i�x�N�g�����Ƃ��ė��p�j
	auto d = Dot(c, ray);
	//�����x�N�g���ƃx�N�g�����������āA���S����̐����������_�����߂܂�
	auto r = ray * d;
	r = c - r;
	if (r.Magnitude() <= sp.radius)
	{
		// t�����߂���
		// w�����߂�B�ʂ̔��a�ƃx�N�g��V�̑傫�����狁�܂�
		// �~�܂ł̒��S���王���x�N�g���ɗ��Ƃ����ˉe�̒�������w��������t�ɂȂ�܂�
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
	auto checker = (static_cast<int>(pos.x / 50) + static_cast<int>(pos.z / 50)) % 2 == 0;
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

///���C�g���[�V���O
///@param eye ���_���W
///@param sphere ���I�u�W�F�N�g(���̂��������ɂ���)
void RayTracing(const Position3& eye,const Sphere& sphere) {

	for (int y = 0; y < screen_height; ++y) {//�X�N���[���c����
		for (int x = 0; x < screen_width; ++x) {//�X�N���[��������
			//�@���_�ƃX�N���[�����W���王���x�N�g�������
			auto ray = Position3(x - screen_width / 2, screen_height / 2 - y, 0) - eye;
			//�A���K�����Ƃ�
			Ray tmp(eye, ray.Normalized());
			//�BIsHitRayAndObject�֐���True�������甒���h��Ԃ�
			float t;
			Vector3 N;
			if (sphere.IsHit(tmp,t,N))
			{
				//// ��_�����߂�
				auto P = eye + ray * t;
				//// ��_�����������̂Ŗ@���x�N�g�������߂�
				//auto N = P - sphere.pos;
				//N.Normalize();
				auto L = Light.Normalized();
				auto deiffuseB = Dot(N, -L);
				
				// ���C�g�̔��˃x�N�g���Ǝ����̋t�x�N�g���̓��ς����pow��n�悷��B�I�[�o�[�t���[���Ȃ��悤�ɋC��t����
				auto specular = Dot(ReflectVector(L, N), -ray);

				specular = Clamp(specular);
				specular = pow(specular, 10);
				float deiffuse[3];
				for (int i = 0; i < 3; i++)
				{
					deiffuse[i] = (deiffuseB * albedo[i]) + specular;
					deiffuse[i] = Clamp(deiffuse[i]);
				}
				//���h��Ԃ���DrawPixel�Ƃ����֐����g���B
				Color color(255 * deiffuse[0], 255 * deiffuse[1], 255 * deiffuse[2]);

				// ���˂��l����
				// ���˃x�N�g�������
				auto Rray = ReflectVector(ray, N);
				// ���˃x�N�g�����n�ʂƓ����邩���l����
				auto dot = Dot(Rray, plane.N);
				// �܂��͂͂񂵂Ⴓ�����n�ʂȂ�F��ς���
				if (dot < 0.0f)
				{
					// ��_P=eye+ray*t
					// t=w/u (w�͕��ʂ���̋����@�@u�͈�񓖂��蕽�ʂɋ߂Â��傫��)
					// w=eye�EN u=-ray�EN
					// t=w/u�ɂ���������d������̂�
					// t=(w+d)/u
					// ���Ƃ͂�������P�����߂�΂悢
					// �܂��������߂�B����t�͂S�����炢
					auto w = Dot(P, plane.N);
					auto u = Dot(-Rray, plane.N);
					auto tmp = (w + plane.d) / u;
					P = P + Rray * tmp;

					
					//���h��Ԃ���DrawPixel�Ƃ����֐����g���B

					Color col = GetCheckerdColorFromPosition(P);
					float reflectivity = 0.2f;	// ���˗�
					auto tmpCol = (col * reflectivity);

					
					UINT32 lastCol = GetUINT32ColorFromVectorColor(color * (1.0f - reflectivity) + tmpCol);

					DrawPixel(x, y, lastCol);
				}
				else
				{
					DrawPixel(x, y, GetUINT32ColorFromVectorColor(color));
				}
			}
			// ���̂ɓ�����Ȃ�����
			else
			{
				// ���ʂɓ��������
				// ���ɕ��ʂ̖@����(0,1,0)
				// ���ʂɓ���������͎����Ɩ@���x�N�g����90�x�ȏ�

				auto dot = Dot(ray, plane.N);
				if (dot < 0.0f)	// �n�ʂɓ������Ă�
				{
					// ��_P=eye+ray*t
					// t=w/u (w�͕��ʂ���̋����@�@u�͈�񓖂��蕽�ʂɋ߂Â��傫��)
					// w=eye�EN u=-ray�EN
					// t=w/u�ɂ���������d������̂�
					// t=(w+d)/u
					// ���Ƃ͂�������P�����߂�΂悢
					// �܂��������߂�B����t�͂S�����炢
					auto w = Dot(eye, plane.N);
					auto u = Dot(-ray, plane.N);
					auto tmp = (w + plane.d) / u;
					auto P = eye + ray * tmp;

					Color col = GetCheckerdColorFromPosition(P);

					// �e����
					if (IsHitRayAndObject(P, -Light.Normalized(), sphere, t))
					{
						// �Â�����
						col *= 0.5f;
					}

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
	SetMainWindowText(_T("�ȈՔł̃��C�g���ł���"));
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