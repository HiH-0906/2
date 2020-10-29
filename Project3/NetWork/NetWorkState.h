#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include "../common/Vector2.h"

// �l�b�g��Ԕ��f�p
enum class NetWorkMode
{
	NON,
	OFFLINE,
	HOST,
	GEST,
	MAX
};

enum class ACTIVE_STATE
{
	NON,
	WAIT,
	INIT,
	STANBY,
	PLAY,
	OFFLINE
};

enum class MES_TYPE:unsigned char
{
	STANBY,
	GAME_START,
	TMX_SIZE,
	TMX_DATA,
	POS
};


struct MES_DATA
{
	MES_TYPE type;
	unsigned short id;
	unsigned char cdata;
	int data[2];
};

union sendData
{
	unsigned char cdata[8];
	unsigned int idata[2];
	long long ldata;
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

	ACTIVE_STATE GetActive(void);
	int GetNetHandle(void);
	virtual bool ConnectHost(IPDATA hostIP);						// �z�X�g��I�t���C���̎��͕K��false
	bool SetActive(ACTIVE_STATE state);
private:
	virtual bool CheckNetState(void) { return false; };
protected:
	void CloseNetWork(void);
	const int portNum_ = 8086;										// �ڑ����|�[�g�ԍ� �����͐搶�̂��C�ɓ���̔ԍ�
	int netHandle_;
	ACTIVE_STATE active_;											// �ڑ��J�n���Ă��邩�ǂ���
};

