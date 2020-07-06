#include<DxLib.h>
#include"Vector2.h"
#include"Input.h"
#include "SceneMng.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	if (!lpSceneMng.SysInit())
	{
		return - 1;
	}
	while (!ProcessMessage() && !CheckHitKey(KEY_INPUT_ESCAPE))
	{
		lpSceneMng.Run();
	}
	DxLib::DxLib_End();

	return 0;
}