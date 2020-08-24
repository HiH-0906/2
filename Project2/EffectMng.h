#pragma once
#include <memory>
#include <map>
#include <string>
#include <EffekseerForDXLib.h>
#include "Scene/SceneMng.h"
#include "common/Vector2.h"

#define lpEffectMng EffectMng::GetInstance()
using EffectPair = std::pair<int, SCREEN_ID>;
using EffectList = std::list<EffectPair>;

class EffectMng
{
public:
	static EffectMng& GetInstance(void)
	{
		return  *s_instance;
	}
	bool Init(Vector2&& size);
	void Update(void);
	void Draw(void);
	const EffectList GeteffectList(void)const;
	bool StopAll(void);
	void SetEffect(std::string name, Vector2 pos,SCREEN_ID id);
private:
	const int GetHandle(std::string name);
	std::map<std::string, int> effectHandle_;
	EffectList effectList_;
	EffectMng();
	~EffectMng();
	struct EffectMngDeleter
	{
		void operator ()(EffectMng* mng)
		{
			delete mng;
		}
	};
	static std::unique_ptr<EffectMng, EffectMngDeleter> s_instance;
};

