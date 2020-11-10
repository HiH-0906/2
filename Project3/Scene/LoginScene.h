#pragma once

#include <map>
#include <string>
#include <vector>
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
class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();
	void Init(void)override final;
	uniqueBase Update(uniqueBase own)override final;
	void DrawOwnScene(void)override final;
private:

	// �X�e�[�g�p�^�[���ŊǗ���������
	using TitleFuncT = bool(LoginScene::*)();

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


	UPDATE_STATE state_;						// Update�Ǘ��p�ϐ�

	// �����킩��n �ꕔ�^�C�g���Ŏ��̂���c�H���Ă̂����邯�ǂ�����Ȃ��Ȃ�ł��傤����TitleScene�����Ȃ��̂Ő�����i�C�l
	int screenSize_X;
	int screenSize_Y;
	int Image;
	Vector2 pos_;
	IPDATA ipData_;
	std::unique_ptr<Input> input_;
};

