#pragma once

#include <map>
#include <string>
#include <vector>
#include <chrono>
#include "BaseScene.h"
#include "../map/Map.h"
#include "../Input/PadState.h"
#include "../common/Vector2.h"

enum class UPDATE_STATE
{
	SET_NET,				// ���p �z�X�g���Q�X�g��
	SELECT_HOST,			// �Q�X�g��p �O��ڑ��҂ɂȂ����ǂ��� 
	READ_HOST,				// �Q�X�g��p �O��ڑ��҂ɂȂ�
	HOST_IP,				// �Q�X�g��p IP���͑ҋ@
	START_INIT,				// ���p ������
	PLAY,
};

// ����l�b�g��������ɂȂ��Ă��� ������i�C�l
class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	void Init(void)override final;
	unipueBase Update(unipueBase own)override final;
	void Draw(void)override final;
private:

	// �X�e�[�g�p�^�[���ŊǗ���������
	using TitleFuncT = bool(TitleScene::*)();

	std::map<UPDATE_STATE, TitleFuncT> func_;
	// �X�e�[�g�p�^�[���ɂԂ����܂��\��֐�
	bool HostIPInput(void);						// IP�A�h���X�̓���
	bool PlayUpdate(void);						// �Q�[���{�� �ŏI�I�ɂȂ��Ȃ�Ǝv��
	bool SetNetWork(void);						// �z�X�g�A�Q�X�g�A�I�t���C���̑I��
	bool StartInit(void);						// ������
	bool SelectHost(void);						// IP�A�h���X����͂��邩�ǂݍ��ނ�
	bool ReadHost(void);						// IP�A�h���X�ǂݍ���

	// �t�@�C���֘A
	bool ReadFile(void);
	bool WritFile(void);

	bool SendNetWorkMes(std::string filename);

	std::unique_ptr<Map> mapMng_;				// Map�֌W��̎����Ă�N���X

	UPDATE_STATE state_;						// Update�Ǘ��p�ϐ� �^�C�g���V�[���ł͂Ȃ��Ȃ�\��

	// �����킩��n �ꕔ�^�C�g���Ŏ��̂���c�H���Ă̂����邯�ǂ�����Ȃ��Ȃ�ł��傤����TitleScene�����Ȃ��̂Ő�����i�C�l
	int screenSize_X;
	int screenSize_Y;
	int Image;
	Vector2 pos_;
	int speed_;
	float rad_;
	IPDATA ipData_;
	std::unique_ptr<Input> input_;

	std::chrono::system_clock::time_point start_;
	std::chrono::system_clock::time_point end_;
};

