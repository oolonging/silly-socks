#include <iostream>
#include "SplashPage.hpp"
#include "Mainmenu.hpp"
#include "Level1.hpp"
#include "Level2.hpp"
#include "GameStateManager.hpp"

int current = 0, previous = 0, next = 0;

FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

void GSM_Initialize(int startingState)
{
	current = previous = next = startingState;
}

void GSM_Update()
{

	switch (current)
	{
	case GS_SPLASH_SCREEN:
		fpLoad = Splash_Load;
		fpInitialize = Splash_Initialize;
		fpUpdate = Splash_Update;
		fpDraw = Splash_Draw;
		fpFree = Splash_Free;
		fpUnload = Splash_Unload;
		break;

	case GS_MAIN_MENU:
		fpLoad = Mainmenu_Load;
		fpInitialize = Mainmenu_Initialize;
		fpUpdate = Mainmenu_Update;
		fpDraw = Mainmenu_Draw;
		fpFree = Mainmenu_Free;
		fpUnload = Mainmenu_Unload;
		break;

	case GS_LEVEL1:
		fpLoad = Level1_Load;
		fpInitialize = Level1_Initialize;
		fpUpdate = Level1_Update;
		fpDraw = Level1_Draw;
		fpFree = Level1_Free;
		fpUnload = Level1_Unload;
		break;
		
	case GS_LEVEL2:
		fpLoad = Level2_Load;
		fpInitialize = Level2_Initialize;
		fpUpdate = Level2_Update;
		fpDraw = Level2_Draw;
		fpFree = Level2_Free;
		fpUnload = Level2_Unload;
		break;

	case GS_RESTART:
		break;

	case GS_QUIT:
		break;

	default:
		break;
	}

}
