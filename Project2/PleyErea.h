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
	OZYAMA,
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
	void ozyamaCnt(int cnt);						// ���ז��Ղ�\��
	const int playerID(void)const;
private:
	void InstancePuyo(void);
	void Draw(void);								// �`��
	void DrawOzyama(void);							// ���ז��Ղ�\���̕`��
	void DeletePuyo(void);							// ���̂Ղ�𓮂��Ȃ�����̂Ǝ��̂Ղ�ݽ�ݽ
	bool SetErasePuyo(Vector2 vec, PUYO_ID id);		// �����邩���f
	bool CheckMovePuyo(PuyoUnit& puyo);				// �����Ă����Ղ悩���f
	bool Init(CON_ID id);							// �������p�֐�
	Vector2 ConvertGrid(Vector2&& pos);				// pos��Grid��
	int screenID_;									// �ʕ`��p��ذ�
	int puyoScreenID_;								// �Ղ摀��ꏊ�`��p��ذ�
	int NoticeOzyamaScrID;							// ���ז��Ղ�\���p��ذ�
	int playerID_;									// ���������Ԗڂ�
	const Vector2 stgSize_;							// �Ղ�Ղ�Ͻ��
	const Vector2 size_;							// playErea�̑傫��
	std::map<STAGE_MODE, std::function<bool(PleyErea&)>> stageFunc_;			// Update�܂Ƃ�
	Vector2 offset_;								// �`�掞�̾��
	std::shared_ptr<Input*> input_;					// ���͸׽�Ǘ��p
	std::array<PuyoUnit, 2> nextPuyo_;				// ���~���Ă���Ղ�
	std::vector<PuyoUnit> puyoList_;				// �Ղ�Ǘ��pؽ�
	std::vector<PuyoUnit*> playErea_;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<PuyoUnit> playEreaBase_;			// playErea�S�̂��ް��Ǘ��p�z��
	std::vector<PuyoUnit*> eraseErea_;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<PuyoUnit> eraseEreaBase_;			// playErea�S�̂̏����Ƃ��ް��Ǘ��p�z��
	int color_;										// �ر�̐F
	int blockSize_;									// 1Ͻ�̑傫��
	int rensaNum_;									// ���ݘA����
	int rensaMax_;									// ���̎����ł̍ő�A����
	int ozyamaCnt_;									// ���ז��Ղ旎����
	int ozyamaFallMax_;								// ���ז��Ղ擯�������ő吔
	int eraseCnt_;									// ����Erase�ł����Ղ��������
	static int allStage_;							// �S�̂�playErea���������邩�̶���
	STAGE_MODE mode_;								// ���݂�Ӱ��
	std::unique_ptr<playUnit> playUnit_;			// ����n�܂Ƃ�
	void FallOzyama(void);							// ���ז��Ղ�ݽ�ݽ�p

	friend class playUnit;
	friend struct FallMode;
	friend struct DropMode;
	friend struct EraseMode;
	friend struct PuyonMode;
	friend struct MunyonMode;
	friend struct OzyamaMode;
};

