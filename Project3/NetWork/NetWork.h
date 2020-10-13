#pragma once
#include <DxLib.h>

#define lpNetWork NetWork::GetInstance()

class NetWork
{
public:
	static NetWork& GetInstance()
	{
		static NetWork s_Instance;
		return (s_Instance);
	}
	IPDATA GetIP(void);
private:
	NetWork();
	~NetWork();
};

