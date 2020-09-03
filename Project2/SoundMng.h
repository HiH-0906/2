#pragma once
#include <memory>
#include <map>
#include <string>

#define lpSoundMng SoundMng::GetInstance()

class SoundMng
{
public:
	static SoundMng& GetInstance(void)
	{
		return  *s_instance;
	}
	int GetHandle(std::string key);
	void SetHandle(std::string key);
private:
	std::map<std::string, int> soundHandle_;
	SoundMng();
	~SoundMng();
	struct SoundMngDeleter
	{
		void operator ()(SoundMng* mng)
		{
			delete mng;
		}
	};
	static std::unique_ptr<SoundMng, SoundMngDeleter> s_instance;
};

