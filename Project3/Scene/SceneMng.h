#pragma once
#include <memory>
#include <map>
#include <vector>
#include <functional>
#include "../common/Vector2.h"
#include "BaseScene.h"
#include "../State/SCREEN_ID.h"

#define lpSceneMng SceneMng::GetInstance()


class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		return *s_instance_;
	}
	void Run();

	Vector2 GetScreenSize(void);

	bool SysInit(void);

private:
	struct SceneMngDeleter
	{
		void operator()(SceneMng* mng)
		{
			delete mng;
		}
	};

	uniqueBase activeScene_;					// �ư��߲��

	std::map<SCREEN_ID, std::vector<std::pair<int, Vector2>>> DrawMap_;

	const int screenX;
	const int screenY;

	Time now_;

	void Draw();
	SceneMng();
	~SceneMng();
	static std::unique_ptr<SceneMng,SceneMngDeleter> s_instance_;
};

