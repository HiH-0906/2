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
	auto itr = std::remove_if(effectList_.begin(), effectList_.end(), [&](EffectPair data) {return !IsEffekseer2DEffectPlaying(data.first); });
	effectList_.erase(itr, effectList_.end());
}

void EffectMng::Draw(void)
{
	DrawEffekseer2D();
}

const EffectList EffectMng::GeteffectList(void) const
{
	return effectList_;
}

bool EffectMng::StopAll(void)
{
	for (auto handle: effectList_)
	{
		StopEffekseer2DEffect(handle.first);
	}
	return true;
}

void EffectMng::SetEffect(std::string name, Vector2 pos, SCREEN_ID id)
{
	EffectPair pair = { PlayEffekseer2DEffect(GetHandle(name)),id };
	effectList_.emplace_front(pair);
	SetPosPlayingEffekseer2DEffect(effectList_.front().first,static_cast<float>(pos.x), static_cast<float>(pos.y), 0);
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
