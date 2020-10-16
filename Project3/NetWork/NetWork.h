#pragma once
#include <memory>
#include <DxLib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()


// �����̃V���O���g���N���X
class NetWork
{
public:
	static NetWork& GetInstance()
	{
		return *s_Instance;
	}

	bool UpDate(void);

	bool SetNetWorkMode(NetWorkMode mode);
	ACTIVE_STATE GetActive(void);
	IPDATA GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// �w�肳�ꂽIP�A�h���X�̃z�X�g�֐ڑ����ɍs��
	NetWorkMode GetMode(void);
	void RecvMes(Vector2& pos);
	void SendMes(MES_DATA data);
	void SendStanby(void);
	void SendStart(void);

	bool GetRevMesType(MES_TYPE type);
	bool GetRevStanby(void);

private:
	bool revState_;

	struct NetWorkDeleter
	{
		void operator()(NetWork* net)
		{
			delete net;
		}
	};
	static std::unique_ptr<NetWork, NetWorkDeleter> s_Instance;

	// �l�b�g���[�N�֌W�{��
	std::unique_ptr<NetWorkState> state_;

	NetWork();
	~NetWork();
};

