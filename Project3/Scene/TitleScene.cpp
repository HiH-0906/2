#include <DxLib.h>
#include "TitleScene.h"
#include "../NetWork/NetWork.h"
#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	screenSize_X = 0;
	screenSize_Y = 0;
	auto ip = lpNetWork.GetIP();
	TRACE("Ž©•ª‚ÌIPƒAƒhƒŒƒX‚Í%d.%d.%d.%d‚Å‚·\n", ip.d1, ip.d2, ip.d3, ip.d4);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(void)
{
	GetDrawScreenSize(&screenSize_X, &screenSize_Y);
}

unipueBase TitleScene::Update(unipueBase own)
{
	return own;
}

void TitleScene::Draw(void)
{
}


