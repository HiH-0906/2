#pragma once
#include <functional>
#include <mutex>
#include "../NetWork/NetWork.h"
#include "../common/Vector2.h"
#include "../State/DIR.h"

class Map;
class BaseScene;

class Obj
{
public:
	Obj(Vector2 pos, Vector2 size,int id,int speed, std::shared_ptr<Map> mapMng, BaseScene& scene);
	virtual ~Obj() = default;
	virtual bool UpdateDef(void);
	std::function<bool(void)> Update_;
	virtual void Draw(void) = 0;
	bool CheckMesList(void);
	int GetID(void);
	bool Alive(void);
protected:
	int id_;
	std::vector<RevDataP> revList_;
	std::shared_ptr<Map> mapMng_;
	bool isPickMesList(MES_TYPE type);
	RevDataP PickUpMes(MES_TYPE type);
	bool CheckHitWall(DIR dir);
	std::mutex mtx_;
	Vector2 pos_;
	Vector2 size_;
	int speed_;
	int offSetY_;
	int animCnt_;
	BaseScene& scene_;

	bool alive_;
};

