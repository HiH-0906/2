#include "DxLib.h"
#include "HostState.h"
#include "../_debug/_DebugConOut.h"

HostState::HostState()
{
	// �ڑ���t�J�n���Ă�����0�Ȃ̂ł���Ȋ���
	active_ = PreparationListenNetWork(portNum_) == 0 ? true : false;
}

HostState::~HostState()
{
}
