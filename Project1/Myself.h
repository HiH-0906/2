#pragma once
#include <mutex>
#include <memory>
#include <map>
#include <vector>
#include "Vector2.h"
#include "TiketMachine.h"

#define lpMyself Myself::GetInstance()

class MouseCtl;

class Myself
{
public:
	static Myself& GetInstance()
	{
		static std::once_flag once;
		std::call_once(once, Create);
		return (*s_Instance);
	}
	static void Create()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new Myself();
		}
	}
	static void Destroy()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	bool Run(void);
	bool MergeCash(MapInt& change);			// ���ނ�󂯎��

private:
	void Draw(void);

	bool SysInit(void);
	bool MyInit(void);

	const int screen_sizeX;					// ��ʂ̉��̉𑜓x
	const int screen_sizeY;					// ��ʂ̏c�̉𑜓x
	const int money_sizeX;					// �����̉��̻���
	const int money_sizeY;					// �����̏c�̻���
	const int font_size;					// ̫�Ă̻���

	//std::unique_ptr<MouseCtl> _mouse;
	sharedMouse _mouse;

	MapInt _cash;								// �����Ă��邨���@(first:����Asecond:����)

	Myself();
	~Myself();
	static Myself* s_Instance;
};

