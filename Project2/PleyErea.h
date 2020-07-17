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
	int screenID_;									// �ʕ`��p��ذ�
	int puyoScreenID_;								// �Ղ摀��ꏊ�`��p��ذ�
	int playerID_;									// ���������Ԗڂ�
	const Vector2 stgSize_;							// �Ղ�Ղ�Ͻ��
	const Vector2 size_;							// playErea�̑傫��
	Vector2 offset_;								// �`�掞�̾��
	std::shared_ptr<Input*> input_;					// ���͸׽�Ǘ��p
	std::list<std::shared_ptr<Puyo>> puyoList_;		// �Ղ�Ǘ��pؽ�
	std::vector<PUYO_ID*> playErea_;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<PUYO_ID> playEreaBase_;				// playErea�S�̂��ް��Ǘ��p�z��
	int color_;										// �ر�̐F
	int blockSize_;									// 1Ͻ�̑傫��
	int aliveCnt_;									// �ڒn�㐶���Ă鎞�ԊǗ��p
	int downCnt_;
	static int allStage_;							// �S�̂�playErea���������邩�̶���
};

