#pragma once
#include <memory>
#include <vector>
#include "PleyErea.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		return *S_instance;
	}
	void Run();
	const Vector2 screenSize(void)const;
	bool SysInit(void);
private:
	struct SceneMngDeleter
	{
		void operator()(SceneMng* mng)
		{
			delete mng;
		}
	};

	const int screenX;
	const int screenY;
	void Draw();
	SceneMng();
	~SceneMng();
	static std::unique_ptr<SceneMng,SceneMngDeleter> S_instance;
	std::vector<std::unique_ptr<PleyErea>> _playErea;
};

