#pragma once
#include <memory>
#include <vector>
#include <list>
#include <functional>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo/Puyo.h"
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
	GAMEOVER,
	WIN,
};

using PuyoSt = std::pair < PUYO_ID, PUYO_STATE >;
using sharPuyo = std::shared_ptr<Puyo>;

class NextPuyoCtl;

class PleyErea
{
public:
	PleyErea(Vector2&& size, Vector2&& offset,Vector2&& pos, CON_ID id);
	~PleyErea();
	int UpDate(void);								// �X�V�p
	const Vector2& offset(void);						// �̾�Ă�get	��������c�H
	const int& GetScreenID(void)const;				// �`��p��ذ݂�get
	void ozyamaCnt(int cnt);						// ���ז��Ղ�\��
	const int& playerID(void)const;
	void SetWinner(bool winner);					// true�Ŏ���������
	const Vector2& pos(void)const;
	const CON_ID& inputID(void)const;
	const int& padNum(void)const;
	bool PlesePose(void);							// �߰�ޗv���֐�
	std::shared_ptr<Input*> GetInput(void);
	void padNum(int& num);
	void inputID(CON_ID&& id);

	void Draw(void);								// �`��
private:
	void InstancePuyo(void);
	void DrawOzyama(void);							// ���ז��Ղ�\���̕`��
	void DrawGost(void);

	void DeletePuyo(void);							// ���̂Ղ�𓮂��Ȃ�����̂Ǝ��̂Ղ�ݽ�ݽ

	bool SetErasePuyo(Vector2 vec, PUYO_ID id);		// �����邩���f
	bool CheckMovePuyo(sharPuyo& puyo);				// �����Ă����Ղ悩���f
	bool CheckGost(Vector2& pos,std::vector<sharPuyo*> gost);
	bool Init(CON_ID id);							// �������p�֐�

	Vector2 ConvertGrid(Vector2&& pos);				// pos��Grid��

	int screenID_;									// �ʕ`��p��ذ�
	int puyoScreenID_;								// �Ղ摀��ꏊ�`��p��ذ�
	int gostScreen_;								// �ް�ĕ`���
	int noticeOzyamaScrID_;							// ���ז��Ղ�\���p��ذ�
	int playerID_;									// ���������Ԗڂ�
	int padNum_;									// �ǂ��߯�ނ��g����

	const Vector2 stgSize_;							// �Ղ�Ղ�Ͻ��
	const Vector2 size_;							// playErea�̑傫��

	std::map<STAGE_MODE, std::function<int(PleyErea&)>> stageFunc_;			// Update�܂Ƃ�

	Vector2 offset_;								// �`�掞�̾��
	Vector2 pos_;									// �`��ʒu
	float rad_;										// �`�掞�p�x

	std::shared_ptr<Input*>input_;					// ���͸׽�Ǘ��p
	CON_ID inputID_;								// ����׽�I��p

	std::vector<sharPuyo> puyoList_;				// �Ղ�Ǘ��pؽ�

	std::vector<sharPuyo*> playErea_;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<sharPuyo> playEreaBase_;			// playErea�S�̂��ް��Ǘ��p�z��
	std::vector<sharPuyo*> eraseErea_;				// ���̓z��񎟌��z��݂����ɱ�������Ƃ��p
	std::vector<sharPuyo> eraseEreaBase_;			// playErea�S�̂̏����Ƃ��ް��Ǘ��p�z��

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
	std::unique_ptr<NextPuyoCtl> nextPuyo_;			// next�Ǘ��p
	void FallOzyama(void);							// ���ז��Ղ�ݽ�ݽ�p

	friend class playUnit;
	friend struct FallMode;
	friend struct DropMode;
	friend struct EraseMode;
	friend struct PuyonMode;
	friend struct MunyonMode;
	friend struct OzyamaMode;
	friend struct GameOverMode;
	friend struct WinMode;
};

using PlayEreaVec = std::vector<std::shared_ptr<PleyErea>>;