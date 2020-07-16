#pragma once
#include <memory>
#include <vector>
#include <list>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo.h"
#include "State/PUYO_ID.h"

enum class PUYO_STATE
{
	NON,
	STAY,
	DEATH,
};

using PuyoSt = std::pair < PUYO_ID, PUYO_STATE >;

class PleyErea
{
public:
	PleyErea(Vector2&& size, Vector2&& offset, CON_ID id);
	~PleyErea();
	void UpDate(void);								// �X�V�p
	const Vector2 offset(void);						// �̾�Ă�get	��������c�H
	const int GetScreenID(void)const;				// �`��p��ذ݂�get
private:
	void Draw(void);								// �`��
	void NextPuyo(void);							// ���̂Ղ�𓮂��Ȃ�����̂Ǝ��̂Ղ�ݽ�ݽ
	bool CheckMovePuyo(void);						// �����Ă����Ղ悩���f
	bool Init(CON_ID id);							// �������p�֐�
	int _screenID;									// �ʕ`��p��ذ�
	int _playerID;									// ���������Ԗڂ�
	const Vector2 _stgSize;							// �Ղ�Ղ�Ͻ��
	const Vector2 _size;							// playErea�̑傫��
	Vector2 _offset;								// �`�掞�̾��
	std::shared_ptr<Input*> _input;					// ���͸׽�Ǘ��p
	std::list<std::shared_ptr<Puyo>> _puyoList;		// �Ղ�Ǘ��pؽ�
	std::vector<PUYO_ID*> _playErea;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<PUYO_ID> _playEreaBase;				// playErea�S�̂��ް��Ǘ��p�z��
	int _color;										// �ر�̐F
	int _blockSize;									// 1Ͻ�̑傫��
	int aliveCnt_;									// �ڒn�㐶���Ă鎞�ԊǗ��p
	static int _allStage;							// �S�̂�playErea���������邩�̶���
};

