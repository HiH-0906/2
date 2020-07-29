#pragma once
#include<array>
#include<utility>
#include "Input.h"
#include "../common/Vector2.h"

// ϳ����͸׽ ��ʂ̒���(�����l)����ǂꂾ�����������Ŕ��ʂ킩�肸�炢
class MouseState
	:public Input
{
public:
	MouseState();
	~MouseState();
	CON_ID GetID(void)override final;
	void Update(const int& p_id)override final;
	void Setting(void)override final;
private:
	bool GetMouseTrg(int id);
	Vector2 _pos;							// ϳ�Pos
	Vector2 _buff;							// �ǂꂾ����������
	int _mouseData;							// �د�ų
	int _mouseDataOld;						// �د������
	Vector2 _stanPos;						// ��ʒu �����l�^��
};