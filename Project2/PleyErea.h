#pragma once
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo.h"
#include "State/PUYO_ID.h"
#include "playUnit.h"

enum class PUYO_STATE
{
	NON,
	STAY,
	DEATH,
};

enum class STAGE_MODE
{
	DROP,
	PUYON,
	FALL,
	MUNYON,
	ERASE,
};

using PuyoSt = std::pair < PUYO_ID, PUYO_STATE >;
using PuyoUnit = std::shared_ptr<Puyo>;

class PleyErea
{
public:
	PleyErea(Vector2&& size, Vector2&& offset, CON_ID id);
	~PleyErea();
	void UpDate(void);								// �X�V�p
	const Vector2 offset(void);						// �̾�Ă�get	��������c�H
	const int GetScreenID(void)const;				// �`��p��ذ݂�get
private:
	void InstancePuyo(void);
	void Draw(void);								// �`��
	void DeletePuyo(void);							// ���̂Ղ�𓮂��Ȃ�����̂Ǝ��̂Ղ�ݽ�ݽ
	bool SetErasePuyo(Vector2 vec, PUYO_ID id);		// �����邩���f
	bool CheckMovePuyo(PuyoUnit& puyo);				// �����Ă����Ղ悩���f
	bool CheckMuyonPuyo(void);						// �ނ�[����Ă��Ă���ւ����邩�ǂ���
	bool Init(CON_ID id);							// �������p�֐�
	int screenID_;									// �ʕ`��p��ذ�
	int puyoScreenID_;								// �Ղ摀��ꏊ�`��p��ذ�
	int playerID_;									// ���������Ԗڂ�
	const Vector2 stgSize_;							// �Ղ�Ղ�Ͻ��
	const Vector2 size_;							// playErea�̑傫��
	std::map<STAGE_MODE, std::function<void(PleyErea&)>> func_;			// Update�܂Ƃ�
	Vector2 offset_;								// �`�掞�̾��
	std::shared_ptr<Input*> input_;					// ���͸׽�Ǘ��p
	std::vector<PuyoUnit> puyoList_;				// �Ղ�Ǘ��pؽ�
	std::vector<PuyoUnit*> playErea_;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<PuyoUnit> playEreaBase_;			// playErea�S�̂��ް��Ǘ��p�z��
	std::vector<PuyoUnit*> eraseErea_;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<PuyoUnit> eraseEreaBase_;			// playErea�S�̂̏����Ƃ��ް��Ǘ��p�z��
	int color_;										// �ر�̐F
	int blockSize_;									// 1Ͻ�̑傫��
	static int allStage_;							// �S�̂�playErea���������邩�̶���
	STAGE_MODE mode_;								// ���݂�Ӱ��
	std::unique_ptr<playUnit> playUnit_;			// ����n�܂Ƃ�

	friend class playUnit;
	friend struct FallMode;
	friend struct DropMode;
	friend struct EraseMode;
	friend struct PuyonMode;
	friend struct MunyonMode;
};

