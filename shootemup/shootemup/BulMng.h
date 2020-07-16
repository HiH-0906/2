#pragma once
#include <memory>
#include <type_traits>
#include<tuple>
#include<vector>
#include"Geometry.h"
#include"bullet.h"

enum class BULLET_MOVE
{
	Normal, 					// í èÌ
	ThreeWay, 					// 3Way
	Diffusion,					// ägéU
	Scattered,					// ÇŒÇÁÇ‹Ç´
	Spiral,						// âQ
	randBullet,					// ëSëÃÇŒÇÁÇ‹Ç´
	MAX
};

inline BULLET_MOVE operator++(BULLET_MOVE& bul)
{
	return bul = static_cast<BULLET_MOVE>(std::underlying_type<BULLET_MOVE>::type(bul) + 1);
}

enum class BUL_QUE
{
	TYPE,					// éÌï 
	E_POS,					// ¥»–∞à íu
	P_POS					// Ãﬂ⁄≤‘∞à íu
};

using shardBul = std::shared_ptr<bullet>;
using Func_B = std::function<void(Position2&, Position2&, std::vector<shardBul>&)>;
using BUL_QUE_T = std::tuple<BULLET_MOVE, Position2&, Position2&>;
using Pair_B = std::pair<Func_B, int>;

#define lpBulMng BulMng::GetInstance()

class BulMng
{
public:
	static BulMng &GetInstance()
	{
		return *_instance;
	}
	void AddBulQue(BULLET_MOVE type, Position2& enemy, Position2& player);
	void RunBulQue(void);
	void UpDate(void);
	void Draw(bool debug,int& image);
	int GetBulCnt(BULLET_MOVE type);
	bool HitCheck(Position2& player,float rad);
private:
	struct BulMngDeleter
	{
		void operator()(BulMng* bulMng)const
		{
			delete bulMng;
		}
	};
	BulMng();
	~BulMng();
	static std::unique_ptr<BulMng,BulMngDeleter> _instance;
	std::vector<BUL_QUE_T> _queList;
	std::vector<shardBul> _bulletList;
	std::map<BULLET_MOVE, Pair_B> _init;
};

