#pragma once
#include <DxLib.h>
#include <memory>

#define lpNetWork NetWork::GetInstance()

class NetWork
{
public:
	static NetWork& GetInstance()
	{
		return *s_instance;
	}
	IPDATA GetIP(void);
private:
	struct NetWorkDeleter
	{
		void operator()(NetWork* net)
		{
			delete net;
		}
	};

	NetWork();
	~NetWork();

	static std::unique_ptr<NetWork, NetWorkDeleter> s_instance;
};

