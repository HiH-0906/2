#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <chrono>
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

enum class MES_TYPE :unsigned char
{
	NON = 100,
	COUNT_DOWN,
	ID,
	STANBY,
	GAME_START,
	START_TIME,
	TMX_SIZE,
	TMX_DATA,
	POS,
	SET_BOMB,
	DETH,
	LOST,
	MAX
};


struct MES_H
{
	MES_TYPE type;
	unsigned char next;
	unsigned short sendID;
	unsigned int length;
};

struct TMX_SIZE
{
	unsigned int size;
};

union mes_H
{
	MES_H head;
	unsigned int ihead[2];
};

union sendData
{
	unsigned int uidata;
	int idata;
	unsigned char cdata[4];
};

union unionTimeData
{
	std::chrono::system_clock::time_point time;
	unsigned int idata[2];
};

using HandleList = std::list<std::pair<int, unsigned int>>;

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

	void SetCountTime(std::chrono::system_clock::time_point time);
	void SetPlayerID(int id, unsigned int max);
	ACTIVE_STATE GetActive(void);
	HandleList GetNetHandle(void);
	virtual bool ConnectHost(IPDATA hostIP);						// �z�X�g��I�t���C���̎��͕K��false
	bool SetActive(ACTIVE_STATE state);
private:
	virtual bool CheckNetState(void) { return false; };
protected:
	void CloseNetWork(void);
	const int portNum_ = 8086;										// �ڑ����|�[�g�ԍ� �����͐搶�̂��C�ɓ���̔ԍ�
	HandleList netHandleList_;
	ACTIVE_STATE active_;											// �ڑ��J�n���Ă��邩�ǂ���
	std::chrono::system_clock::time_point countTime_;
	bool countStart_;
	int playerID_;
	int playerMax_;
};

