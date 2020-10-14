#pragma once
#include <DxLib.h>

// �l�b�g��Ԕ��f�p
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GEST,
	MAX
};

// �l�b�g�ڑ����W���[���̊�� OFFLINE���͂��ꂪ�C���X�^���X�����
class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void)
	{
		return NetWorkMode::OFFLINE;
	}

	bool GetActive(void);
	virtual bool ConnectHost(IPDATA hostIP);						// �z�X�g��I�t���C���̎��͕K��false
private:

protected:
	const unsigned int portNum_ = 8086;								// �ڑ����|�[�g�ԍ� �����͐搶�̂��C�ɓ���̔ԍ�
	int netHandle_;
	bool active_;													// �ڑ��J�n���Ă��邩�ǂ���
};

