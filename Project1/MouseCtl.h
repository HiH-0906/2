#pragma once
#include "Vector2.h"
class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();
	/// <summary>
	/// �C�ӂ̃}�E�X�{�^�����N���b�N�����u��
	/// </summary>
	/// <param name="">�Ƃ肽���}�E�X�{�^��</param>
	/// <returns>�����u��:true�@����ȊO:false</returns>
	bool GetClickTrg(char btton);					// �د������u�Ԃ̏ꍇ�Atrue
	/// <summary>
	/// �C�ӂ̃}�E�X�{�^�����N���b�N���Ă��邩
	/// </summary>
	/// <param name="">�Ƃ肽���}�E�X�{�^��</param>
	/// <returns>���Ă����:true�@����ȊO:false</returns>
	bool GetClicking(char btton);					// �د����̏ꍇ�Atrue
	const Vector2& GetPos(void);					// �د��ʒu�̎擾
	void Update(void);								// ϳ��̏�ԍX�V
private:
	Vector2 _pos;			// ϳ��̍��W
	char _mouseData;		// ���݂�ϳ��̸د����
	char _mouseDataOld;		// 1�ڰёO��ϳ��̸د����
};

