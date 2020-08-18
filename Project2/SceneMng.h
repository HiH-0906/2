#pragma once
#include <memory>
#include <vector>
#include "BaseScene.h"
#include "PleyErea.h"

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


class SceneMng
{
public:
	static SceneMng& GetInstance()
	{
		return *s_instance;
	}
	void Run();
	void AddRensaQue(RENSA_QUE&& que);
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

	unipueBase _activeScene;					// ÕÆ°¸Îß²ÝÀ

	const int screenX;
	const int screenY;
	void Draw();
	void RunRensaQue(void);
	std::vector<RENSA_QUE> rensaQue_;
	SceneMng();
	~SceneMng();
	static std::unique_ptr<SceneMng,SceneMngDeleter> s_instance;
	//std::vector<std::unique_ptr<PleyErea>> playErea_;
};

