#pragma once
#include <memory>
#include <vector>
#include "PlayErea.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		return *S_instance;
	}
	void Run();
	
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
	std::vector<std::unique_ptr<PlayErea>> _playErea;
};

