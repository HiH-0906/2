#pragma once
#include <memory>
#include <vector>
#include "common/Vector2.h"
#include "BaseScene.h"
#include "PleyErea.h"
#include "State/SCREEN_ID.h"

#define PUYO_RAD 16
#define PUYO_SIZE (PUYO_RAD*2)
#define STAGE_X 8
#define STAGE_Y 15

// RENSA_QUE—p
enum class RENSA_DATA
{
	ID,
	RENSA,
	CNT
};


#define lpSceneMng SceneMng::GetInstance()

using DrawQueT = std::tuple<Vector2&, int&, SCREEN_ID>;
using DrawList = std::vector<DrawQueT>;


class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		return *s_instance;
	}
	void Run();
	const Vector2 screenSize(void)const;
	bool SysInit(void);
	void AddDrawList(DrawQueT&& que);
private:
	struct SceneMngDeleter
	{
		void operator()(SceneMng* mng)
		{
			delete mng;
		}
	};

	unipueBase activeScene_;					// ÕÆ°¸Îß²ÝÀ

	std::map<SCREEN_ID, std::vector<std::pair<int, Vector2>>> DrawMap;
	std::map<SCREEN_ID, std::vector<int>> EffectMap;

	DrawList drawList_;
	const int screenX;
	const int screenY;

	void Draw();
	SceneMng();
	~SceneMng();
	static std::unique_ptr<SceneMng,SceneMngDeleter> s_instance;
};

