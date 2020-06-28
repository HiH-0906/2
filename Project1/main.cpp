#include <DxLib.h>
#include "main.h"
#include "Myself.h"
#include "TiketMachine.h"


#define SCREEN_SIZE_X (800)
#define SCREEN_SIZE_Y (600)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Myself::GetInstance().Run();
	Myself::Destroy();
	return 0;
}

