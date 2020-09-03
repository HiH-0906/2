#include "SoundMng.h"
#include <DxLib.h>

std::unique_ptr<SoundMng, SoundMng::SoundMngDeleter> SoundMng::s_instance(new SoundMng());

int SoundMng::GetHandle(std::string key)
{
	return soundHandle_[key];
}

void SoundMng::SetHandle(std::string key)
{
	soundHandle_.try_emplace(key, LoadSoundMem(("sound/" + key + ".mp3").c_str()));
}

SoundMng::SoundMng()
{
}

SoundMng::~SoundMng()
{
}
