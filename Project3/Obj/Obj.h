#pragma once
#include <functional>
#include <mutex>
#include "../NetWork/NetWork.h"
#include "../common/Vector2.h"

class Map;

class Obj
{
public:
	Obj(Vector2 pos, Vector2 size,int speed, std::shared_ptr<Map> mapMng);
	virtual ~Obj() = default;
	virtual bool UpdateDef(void);
	std::function<bool(void)> Update_;
	virtual void Draw(void) = 0;
	bool CheckMesList(void);
protected:
	std::vector<RevData> revList_;
	std::shared_ptr<Map> mapMng_;
	bool isPickMesList(MES_TYPE type);
	RevData PickUpMes(MES_TYPE type);
	std::mutex mtx_;
	Vector2 pos_;
	Vector2 size_;
	int speed_;
	int offSetY_;
	int animCnt_;
};

