#include "DxLib.h"
#include "HostState.h"
#include "../_debug/_DebugConOut.h"

HostState::HostState()
{
	// Ú‘±ó•tŠJn‚µ‚Ä‚¢‚½‚ç0‚È‚Ì‚Å‚±‚ñ‚ÈŠ´‚¶
	active_ = PreparationListenNetWork(portNum_) == 0 ? true : false;
}

HostState::~HostState()
{
}
