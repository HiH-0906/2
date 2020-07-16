#pragma once
#include <memory>
#include <vector>
#include "PleyErea.h"

#define PUYO_RAD 16
#define PUYO_SIZE (PUYO_RAD*2)
#define STAGE_X 8
#define STAGE_Y 15

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

