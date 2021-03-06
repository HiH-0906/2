#include <DxLib.h>
#include "MouseCtl.h"
#include "_debug/_DebugConOut.h"

MouseCtl::MouseCtl()
{
	_mouseData = 0;
	_mouseDataOld = 0;
	TRACE("MouseCtrの生成")
}

MouseCtl::~MouseCtl()
{
	TRACE("MouseCtrの破棄")
}

bool MouseCtl::GetClickTrg(char btton)
{
	return ((_mouseData & btton) != 0 && (_mouseDataOld & btton) == 0);
}

bool MouseCtl::GetClicking(char btton)
{
	return ((_mouseData & btton) != 0 && (_mouseDataOld & btton) != 0);
}

const Vector2& MouseCtl::GetPos(void)
{
	return _pos;
}

void MouseCtl::Update(void)
{
	// ﾏｳｽの座標取得
	GetMousePoint(&_pos.x, &_pos.y);
	
	// ﾏｳｽのｸﾘｯｸ状態更新
	_mouseDataOld = _mouseData;
	_mouseData = GetMouseInput();
}


