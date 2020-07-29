#pragma once
#include <functional>
#include <map>
#include "common/Vector2.h"
#include "State/INPUT_ID.h"
class PleyErea;

class playUnit
{
public:
	playUnit(PleyErea& erea);
	~playUnit();
	bool Update(void);
	const int targetID(void)const;								// ���������{���삵�Ă���Ղ�\���Ɏg������
private:
	PleyErea& playErea_;										// PlayUnit������ւ̎Q��
	std::map<INPUT_ID,std::function<void(void)>> keyFunc_;		// �����͎��̊֐��܂Ƃ�
	void Init(void);											// ������
	void RotaPuyo(Vector2 puyo1, Vector2 puyo2, bool rotaRight);// �Ղ��]�p
	int targetID_;												// �����삵�Ă���Ղ攻�ʗp
};

