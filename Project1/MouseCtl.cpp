#include <DxLib.h>
#include "MouseCtl.h"
#include "_debug/_DebugConOut.h"

MouseCtl::MouseCtl()
{
	_mouseData = 0;
	_mouseDataOld = 0;
	TRACE("MouseCtr‚Ì¶¬")
}

MouseCtl::~MouseCtl()
{
	TRACE("MouseCtr‚Ì”jŠü")
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
	// Ï³½‚ÌÀ•Wæ“¾
	GetMousePoint(&_pos.x, &_pos.y);
	
	// Ï³½‚Ì¸Ø¯¸ó‘ÔXV
	_mouseDataOld = _mouseData;
	_mouseData = GetMouseInput();
}


