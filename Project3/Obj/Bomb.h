#pragma once
#include "Obj.h"
class Bomb :
	public Obj
{
public:
	Bomb(Vector2 pos, Vector2 size,int id,int oid, std::shared_ptr<Map> mapMng, BaseScene& scene, std::chrono::system_clock::time_point start);
	~Bomb();
	bool UpdateDef(void)override;
	void Draw(void)override;
private:
	int owner_;
	std::chrono::system_clock::time_point start_;
};

