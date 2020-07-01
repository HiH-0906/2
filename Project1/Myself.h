#pragma once
#include <mutex>
#include <memory>
#include <map>
#include <vector>
#include "Vector2.h"
#include "TiketMachine.h"

#define lpMyself Myself::GetInstance()

struct Wallet
{
	PayType payType;
	int cash;
};

using Func_T = std::function<void(Wallet wallet)>;


struct InsertCard;
struct InsertMax;
struct InsertCash;

class MouseCtl;

class Myself
{
public:
	

	static Myself& GetInstance()
	{
		static std::once_flag once;
		std::call_once(once, Create);
		return (*s_Instance);
	}
	static void Create()
	{
		if (s_Instance == nullptr)
		{
			s_Instance = new Myself();
		}
	}
	static void Destroy()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	bool Run(void);
	bool MergeCash(MapInt& change);			// お釣り受け取り
	
	void insertClear(void);

private:
	friend struct InsertMax;
	friend struct InsertCard;
	friend struct InsertCash;

	void Draw(void);

	bool SysInit(void);
	bool MyInit(void);

	const int screen_sizeX;					// 画面の横の解像度
	const int screen_sizeY;					// 画面の縦の解像度
	const int money_sizeX;					// お金の横のｻｲｽﾞ
	const int money_sizeY;					// お金の縦のｻｲｽﾞ
	const int font_size;					// ﾌｫﾝﾄのｻｲｽﾞ

	//std::unique_ptr<MouseCtl> _mouse;
	sharedMouse _mouse;

	Func_T _insert;
	
	MapInt _cash;								// 持っているお金　(first:金種、second:枚数)

	Myself();
	~Myself();
	static Myself* s_Instance;
};

