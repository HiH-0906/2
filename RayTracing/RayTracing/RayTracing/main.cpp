#include<dxlib.h>
#include"Geometry.h"
const int screen_width = 640;
const int screen_height = 480;

//�q���g�ɂȂ�Ǝv���āA�F�X�Ɗ֐���p�ӂ��Ă���܂���
//�ʂɂ��̊֐����g��Ȃ���΂����Ȃ��킯�ł��A����ɉ���Ȃ���΂����Ȃ��킯�ł�
//����܂���B���C�g���[�V���O���ł��Ă���΍\���܂���B

///���C(����)�Ƌ��̂̓����蔻��
///@param ray (���_����X�N���[���s�N�Z���ւ̃x�N�g��)
///@param sphere ��
///@hint ���C�͐��K�����Ƃ����ق����g���₷�����낤
bool IsHitRayAndObject(const Position3& eye,const Vector3& ray,const Sphere& sp) {
	//���C�����K���ς݂ł���O��Łc
	//
	//���_���狅�̒��S�ւ̃x�N�g�������܂�
	//
	auto c = sp.pos - eye;
	//���S���王���ւ̓��ς��Ƃ�܂������ˉe���i�x�N�g�����Ƃ��ė��p�j
	auto v = Dot(c, ray);
	//�����x�N�g���ƃx�N�g�����������āA���S����̐����������_�����߂܂�
	auto r = ray * v;
	r = c - r;
	if (r.Magnitude() <= sp.radius)
	{
		return true;
	}
	return false;
}

///���C�g���[�V���O
///@param eye ���_���W
///@param sphere ���I�u�W�F�N�g(���̂��������ɂ���)
void RayTracing(const Position3& eye,const Sphere& sphere) {
	for (int y = 0; y < screen_height; ++y) {//�X�N���[���c����
		for (int x = 0; x < screen_width; ++x) {//�X�N���[��������
			//�@���_�ƃX�N���[�����W���王���x�N�g�������
			auto ray = Position3(x - screen_width / 2, -y + screen_height / 2, 0) - eye;
			//�A���K�����Ƃ�
			ray.Normalize();
			//�BIsHitRayAndObject�֐���True�������甒���h��Ԃ�
			if (IsHitRayAndObject(eye, ray, sphere))
			{
				//���h��Ԃ���DrawPixel�Ƃ����֐����g���B
				DrawPixel(x, y, 0xff0000);
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
	RayTracing(eye, sphere);

	WaitKey();
	DxLib_End();
}