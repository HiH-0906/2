#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <chrono>
#include <thread>
#include <mutex>
#include "../common/Vector2.h"

#define INIT_COUNT_TIME 15000

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
	COUNT_DOWN_ROOM,
	ID,
	STANBY_HOST,
	STANBY_GUEST,
	COUNT_DOWN_GAME,
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

struct HANDLE_DATA
{
	int handle;
	unsigned int id;
	int state;
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
// �\���������IdSTATE-1�ؒf0�j���[�g����1����
using HandleList = std::list<HANDLE_DATA>;

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

	void SetCountDownRoomTime(std::chrono::system_clock::time_point time);
	void SetCountDownGameTime(std::chrono::system_clock::time_point time);
	void SetPlayerID(int id, unsigned int max);
	ACTIVE_STATE GetActive(void);
	const HandleList& GetNetHandle(void)const;
	bool GetCountStart(void);
	bool GetGameStart(void);
	const std::chrono::system_clock::time_point& GetCountDownRoomTime(void)const;
	const std::chrono::system_clock::time_point& GetCountDownGameTime(void)const;
	virtual bool ConnectHost(IPDATA hostIP);						// �z�X�g��I�t���C���̎��͕K��false
	bool SetActive(ACTIVE_STATE state);
	const int& GetID(void)const;
	const int& GetMax(void)const;
private:
	std::mutex downMtx_;
	std::mutex gameMtx_;
	virtual bool CheckNetState(void) { return false; };
protected:
	void CloseNetWork(void);
	const int portNum_ = 8086;										// �ڑ����|�[�g�ԍ� �����͐搶�̂��C�ɓ���̔ԍ�
	HandleList netHandleList_;
	ACTIVE_STATE active_;											// �ڑ��J�n���Ă��邩�ǂ���
	std::chrono::system_clock::time_point countDownRoomTime_;
	std::chrono::system_clock::time_point countDownGameTime_;
	bool countDown_;
	bool gameStart_;
	int playerID_;
	int playerMax_;
};

