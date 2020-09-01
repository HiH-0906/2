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
	void Update(void)override final;
	void Setting(const int& p_id, const int& pad_id)override final;
	void Draw(Vector2&& pos);
private:
	bool GetMouseTrg(int id);
	int cnt_;
	int rad_;
	Vector2 _pos;							// ϳ�Pos
	Vector2 _buff;							// �ǂꂾ����������
	int _mouseData;							// �د�ų
	int _mouseDataOld;						// �د������
	Vector2 _stanPos;						// ��ʒu �����l�^��
	int mouseScreen_;
};