#pragma once

#include <map>
#include <string>
#include <vector>
#include "BaseScene.h"
#include "../map/Map.h"
#include "../Input/Input.h"
#include "../common/Vector2.h"
#include "../common/Button.h"
#include "../common/NumPad.h"

#define ResetTime 30000

enum class UPDATE_STATE
{
	SET_NET,				// ���p �z�X�g���Q�X�g��
	SELECT_INIT,
	SELECT_HOST,			// �Q�X�g��p �O��ڑ��҂ɂȂ����ǂ��� 
	READ_HOST,				// �Q�X�g��p �O��ڑ��҂ɂȂ�
	HOST_IP,				// �Q�X�g��p IP���͑ҋ@
	START_INIT,				// ���p ������
};

// ����l�b�g��������ɂȂ��Ă��� ������i�C�l
class LoginScene :
	public BaseScene
{
public:
	LoginScene();
	~LoginScene();
	void Init(void)override final;
	uniqueBase Update(uniqueBase own, const Time& now)override final;
	void DrawOwnScene(void)override final;
private:
	void FuncInit(void);
	void ButtonInit(void);
	void ImageInit(void);

	// �X�e�[�g�p�^�[���ŊǗ���������
	using TitleFuncT = bool(LoginScene::*)();
	std::map<UPDATE_STATE, TitleFuncT> func_;
	// �X�e�[�g�p�^�[���ɂԂ����܂��\��֐�
	bool HostIPInput(void);						// IP�A�h���X�̓���
	bool SelectInit(void);
	bool SetNetWork(void);						// �z�X�g�A�Q�X�g�A�I�t���C���̑I��
	bool StartInit(void);						// ������
	bool SelectHost(void);						// IP�A�h���X����͂��邩�ǂݍ��ނ�
	bool ReadHost(void);						// IP�A�h���X�ǂݍ���

	// �t�@�C���֘A
	bool ReadFile(void);
	bool WritFile(void);
	/// <summary>
	/// IPDATA���e�����Ƃ̐����ɕϊ����đ�O������Vector�Ɋi�[����
	/// 156->number[0]=1,number[1]=5,number[2]=6
	/// </summary>
	/// <param name="ch">�ϊ�����char</param>
	/// <param name="dig">����</param>
	/// <param name="number">���ʊi�[��vector</param>
	void ChengeIPDATAToIntVector(const unsigned char& ch, std::vector<int>& number);
	void IPDraw(const std::vector<int>& ipInt,Vector2& pos, bool comma);

	UPDATE_STATE state_;						// Update�Ǘ��p�ϐ�

	// �����킩��n
	int screenSize_X;
	int screenSize_Y;
	bool sendTmx_;
	bool reset_;
	bool wait_;
	Vector2 pos_;
	IPDATA ipData_;
	std::unique_ptr<Input> input_;
	std::unique_ptr<NumPad> numPad_;
	std::vector<std::unique_ptr<Button>> btn_;

	Time waitTime_;
};

