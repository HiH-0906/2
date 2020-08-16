#include "EffectMng.h"
#include <algorithm>
std::unique_ptr<EffectMng, EffectMng::EffectMngDeleter> EffectMng::s_instance(new EffectMng());


bool EffectMng::Init(Vector2&& size)
{
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting(size.x, size.y);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);

	return true;
}

void EffectMng::Update(void)
{
	UpdateEffekseer2D();
	auto itr = std::remove_if(playList_.begin(), playList_.end(), [&](int data) {return !IsEffekseer2DEffectPlaying(data); });
	playList_.erase(itr, playList_.end());
}

void EffectMng::Draw(void)
{
	DrawEffekseer2D();
}

bool EffectMng::StopAll(void)
{
	for (auto handle:playList_)
	{
		StopEffekseer2DEffect(handle);
	}
	return true;
}

void EffectMng::SetEffect(std::string name, Vector2 pos)
{
	playList_.emplace_front(PlayEffekseer2DEffect(GetHandle(name)));
	SetPosPlayingEffekseer2DEffect(playList_.front(),static_cast<float>(pos.x), static_cast<float>(pos.y), 0);
}

const int EffectMng::GetHandle(std::string name)
{
	effectHandle_.try_emplace(name, LoadEffekseerEffect(("effect/" + name + ".efk").c_str(),8.0f));

	return effectHandle_[name];
}

EffectMng::EffectMng()
{
}

EffectMng::~EffectMng()
{
}
