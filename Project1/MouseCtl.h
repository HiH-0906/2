#pragma once
#include "Vector2.h"
class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();
	/// <summary>
	/// 任意のマウスボタンをクリックした瞬間
	/// </summary>
	/// <param name="">とりたいマウスボタン</param>
	/// <returns>した瞬間:true　それ以外:false</returns>
	bool GetClickTrg(char btton);					// ｸﾘｯｸした瞬間の場合、true
	/// <summary>
	/// 任意のマウスボタンをクリックしているか
	/// </summary>
	/// <param name="">とりたいマウスボタン</param>
	/// <returns>している間:true　それ以外:false</returns>
	bool GetClicking(char btton);					// ｸﾘｯｸ中の場合、true
	const Vector2& GetPos(void);					// ｸﾘｯｸ位置の取得
	void Update(void);								// ﾏｳｽの状態更新
private:
	Vector2 _pos;			// ﾏｳｽの座標
	char _mouseData;		// 現在のﾏｳｽのｸﾘｯｸ状態
	char _mouseDataOld;		// 1ﾌﾚｰﾑ前のﾏｳｽのｸﾘｯｸ状態
};

