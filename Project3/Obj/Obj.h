#pragma once
#include <functional>
#include <mutex>
#include "../NetWork/NetWork.h"
#include "../common/Vector2.h"
#include "../State/DIR.h"
#include "../Scene/BaseScene.h"

class Map;

enum class OBJ_TAG
{
	PLAYER,
	BOMB,
	ITEM
};

class Obj
{
public:
	Obj(Vector2 pos, Vector2 size,int id,int speed, std::shared_ptr<Map> mapMng, BaseScene& scene);
	virtual ~Obj() = default;
	virtual bool UpdateDef(const Time& now);
	const Vector2& GetPos(void)const;
	const Vector2& GetSize(void)const;
	const OBJ_TAG& GetTag(void);
	std::function<bool(const Time& now)> Update_;
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
	OBJ_TAG tag_;
	int speed_;
	int offSetY_;
	int animCnt_;
	BaseScene& scene_;

	bool alive_;
	bool activ_;
};

