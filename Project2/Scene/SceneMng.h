#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "../common/Vector2.h"
#include "BaseScene.h"
#include "../PleyErea.h"
#include "../State/SCREEN_ID.h"

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


enum class DATA_TYPE
{
	IMG,
	EFFECT
};

enum class DRAW_QUE
{
	POS,
	HANDLE,
	RATE,
	RAD,
	ZODER,
	ID,
	TYPE,
	FLAG
};

#define lpSceneMng SceneMng::GetInstance()

using DrawQueT = std::tuple<Vector2, int, double, double, int, SCREEN_ID, DATA_TYPE,bool>;
using DrawList = std::vector<DrawQueT>;

using PlayEreaVec = std::vector<std::shared_ptr<PleyErea>>;


class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		return *s_instance;
	}
	void Run();
	const Vector2 screenSize(void)const;

	const int& fCnt(void)const;
	bool SysInit(void);

	void AddDrawList(DrawQueT&& que);

	PlayEreaVec& playErea(void);
	const int playNum(void);

private:
	struct SceneMngDeleter
	{
		void operator()(SceneMng* mng)
		{
			delete mng;
		}
	};

	unipueBase activeScene_;					// ÕÆ°¸Îß²ÝÀ

	std::map<SCREEN_ID, std::vector<std::pair<int, Vector2>>> DrawMap_;
	std::map<SCREEN_ID, std::vector<int>> EffectMap_;

	PlayEreaVec playErea_;

	DrawList drawList_;
	const int screenX;
	const int screenY;

	std::map<DATA_TYPE, void(SceneMng::*)(DrawQueT&&)> drawFunc_;

	std::map<SCREEN_ID, int> drawScreen_;

	void EffectDraw(DrawQueT&& que);
	void ImageDraw(DrawQueT&& que);

	int fCnt_;

	void Draw();
	SceneMng();
	~SceneMng();
	static std::unique_ptr<SceneMng,SceneMngDeleter> s_instance;
};

