#pragma once
#include "Input.h"

// ���ް�ޓ��͸׽
class keyState :
	public Input
{
public:
	CON_ID GetID(void)override final;						// �����p�����Ȃ�
	void Update(void)override final;
private:
	static std::map<int, keyData> _keyCon;					// ����̨�ލ��̂Ƃ���Œ�
};

