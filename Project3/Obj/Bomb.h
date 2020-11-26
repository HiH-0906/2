#pragma once
#include "Obj.h"
class Bomb :
	public Obj
{
public:
	Bomb(Vector2 pos, Vector2 size, int length,int id,int oid, std::shared_ptr<Map> mapMng, BaseScene& scene, std::chrono::system_clock::time_point start);
	~Bomb();
	bool UpdateDef(const Time& now)override;
	void Draw(void)override;
private:
	bool explosion_;
	int owner_;
	int length_;

	Time start_;
	__int64 elapsedTime_;
	int explosionTime_;
	int defTime_;
	__int64 offset_;
};

