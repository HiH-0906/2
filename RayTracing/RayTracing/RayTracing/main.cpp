#include<dxlib.h>
#include <cmath>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;

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

///���C�g���[�V���O
///@param eye ���_���W
///@param sphere ���I�u�W�F�N�g(���̂��������ɂ���)
void RayTracing(const Position3& eye,const Sphere& sphere) {
	Vector3 Light = Vector3{ 1,-1,-1 };
	for (int y = 0; y < screen_height; ++y) {//�X�N���[���c����
		for (int x = 0; x < screen_width; ++x) {//�X�N���[��������
			//�@���_�ƃX�N���[�����W���王���x�N�g�������
			auto ray = Position3(x - screen_width / 2, screen_height / 2 - y, 0) - eye;
			//�A���K�����Ƃ�
			ray.Normalize();
			//�BIsHitRayAndObject�֐���True�������甒���h��Ԃ�
			float t;
			if (IsHitRayAndObject(eye, ray, sphere, t))
			{
				// ��_�����߂�
				auto P = eye + ray * t;
				// ��_�����������̂Ŗ@���x�N�g�������߂�
				auto N = P - sphere.pos;
				N.Normalize();
				auto L = Light.Normalized();
				auto blink = Dot(N, -L);

				blink = Clamp(blink);

				//���h��Ԃ���DrawPixel�Ƃ����֐����g���B
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
	SetMainWindowText(_T("�ȈՔł̃��C�g���ł���"));
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