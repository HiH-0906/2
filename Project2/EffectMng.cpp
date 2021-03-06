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
	int handle = PlayEffekseer2DEffect(GetHandle(name));
	SetPosPlayingEffekseer2DEffect(handle, static_cast<float>(pos.x), static_cast<float>(pos.y), 0);
	DrawQueT que = { pos,handle ,0.0,0.0,50,id,DATA_TYPE::EFFECT ,true};
	lpSceneMng.AddDrawList(std::move(que));
}

const int EffectMng::GetHandle(std::string name)
{
	effectHandle_.try_emplace(name, LoadEffekseerEffect(("effect/" + name + ".efk").c_str(),1.0f));

	return effectHandle_[name];
}

EffectMng::EffectMng()
{
}

EffectMng::~EffectMng()
{
}
