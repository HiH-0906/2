#pragma once
#include <DxLib.h>
#include <functional>
#include "../common/Vector2.h"

// �l�b�g��Ԕ��f�p
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GEST,
	MAX
};

enum class NET_FUNC
{
	CHOICE,
	GUEST_IP,
	HOST_IP,
	ACTIVE,
	CLOSE,
};

struct POS_DATA
{
	int x, y;
};

// �l�b�g�ڑ����W���[���̊�� OFFLINE���͂��ꂪ�C���X�^���X�����
class NetWorkState
{
public:
	NetWorkState();
	bool Update(void);
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void)
	{
		return NetWorkMode::OFFLINE;
	}

	virtual void SendMes(POS_DATA data);
	virtual void RecvMes(Vector2& pos);
	bool GetActive(void);
	virtual bool ConnectHost(IPDATA hostIP);						// �z�X�g��I�t���C���̎��͕K��false
private:
	virtual bool CheckNetState(void) { return false; };
protected:
	void CloseNetWork(void);
	const int portNum_ = 8086;										// �ڑ����|�[�g�ԍ� �����͐搶�̂��C�ɓ���̔ԍ�
	int netHandle_;
	bool active_;													// �ڑ��J�n���Ă��邩�ǂ���
};

