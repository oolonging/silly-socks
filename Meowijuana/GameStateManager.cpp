#include "pch.h"
#include "GameStateManager.hpp"

// Main game screens
#include "Screens/SplashScreen.hpp"
#include "Screens/MainMenu.hpp"
#include "Screens/SettingsScreen.hpp"
#include "Screens/CreditsScreen.hpp"
#include "Screens/TutorialScreen.hpp"
#include "Screens/FarmScreen.hpp"
#include "Screens/DungeonScreen.hpp"
#include "RespawnArea.hpp"
#include "WinScreen.hpp"
#include "LoseScreen.hpp"

// testing imports
#include "Screens/TestingScreen.hpp"
#include "Screens/Testing/testing-andrea.hpp"
#include "Screens/Testing/testing-xuan.hpp"
#include "Screens/Testing/testing-sprites.hpp"
#include "Screens/Testing/testing-ui-elements.hpp"
#include "TutorialDungeon.hpp"
#include "NewDungeon.hpp"
#include "DesertDungeon.hpp"

// the ones to be removed soon
#include "Screens/Level1.hpp"
#include "Screens/Level2.hpp"

int previous = 0;
int current = 0;
int next = 0;

FP fpLoad = nullptr;
FP fpInitialize = nullptr;
FP fpUpdate = nullptr;
FP fpDraw = nullptr;
FP fpFree = nullptr;
FP fpUnload = nullptr;

void GSM_Initialize(int startingState) { previous = current = next = startingState; }

void GSM_Update()
{
	switch (current)
	{
		// Starting states
		case GS_SPLASH:
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

		
		// Post main menu states
		case GS_SETTINGS:
			fpLoad = Settings_Load;
			fpInitialize = Settings_Initialize;
			fpUpdate = Settings_Update;
			fpDraw = Settings_Draw;
			fpFree = Settings_Free;
			fpUnload = Settings_Unload;
			break;

		case GS_CREDITS:
			fpLoad = Credits_Load;
			fpInitialize = Credits_Initialize;
			fpUpdate = Credits_Update;
			fpDraw = Credits_Draw;
			fpFree = Credits_Free;
			fpUnload = Credits_Unload;
			break;

		// Gameplay states
		case GS_TUTORIAL:
			fpLoad = Tutorial_Load;
			fpInitialize = Tutorial_Initialize;
			fpUpdate = Tutorial_Update;
			fpDraw = Tutorial_Draw;
			fpFree = Tutorial_Free;
			fpUnload = Tutorial_Unload;
			break;

		case GS_FARM:
			fpLoad = Farm_Load;
			fpInitialize = Farm_Initialize;
			fpUpdate = Farm_Update;
			fpDraw = Farm_Draw;
			fpFree = Farm_Free;
			fpUnload = Farm_Unload;
			break;

		case GS_DUNGEON: 
			fpLoad = Dungeon_Load;
			fpInitialize = Dungeon_Initialize;
			fpUpdate = Dungeon_Update;
			fpDraw = Dungeon_Draw;
			fpFree = Dungeon_Free;
			fpUnload = Dungeon_Unload;
			break;

		case GS_TUTDUN:
			fpLoad = TutorialDungeon_Load;
			fpInitialize = TutorialDungeon_Initialize;
			fpUpdate = TutorialDungeon_Update;
			fpDraw = TutorialDungeon_Draw;
			fpFree = TutorialDungeon_Free;
			fpUnload = TutorialDungeon_Unload;
			break;

		// Testing states for individual game mechanics
		case GS_TESTING:
			fpLoad = Testing_Load;
			fpInitialize = Testing_Initialize;
			fpUpdate = Testing_Update;
			fpDraw = Testing_Draw;
			fpFree = Testing_Free;
			fpUnload = Testing_Unload;
			break;

		// Legacy states for testing purposes, will be removed soon
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

		case GS_A:
			fpLoad = Andrea_Load;
			fpInitialize = Andrea_Initialize;
			fpUpdate = Andrea_Update;
			fpDraw = Andrea_Draw;
			fpFree = Andrea_Free;
			fpUnload = Andrea_Unload;
			break;

		case GS_X:
			fpLoad = Xuan_Load;
			fpInitialize = Xuan_Initialize;
			fpUpdate = Xuan_Update;
			fpDraw = Xuan_Draw;
			fpFree = Xuan_Free;
			fpUnload = Xuan_Unload;
			break;

		case GS_NEW:
			fpLoad = NewDungeon_Load;
			fpInitialize = NewDungeon_Initialize;
			fpUpdate = NewDungeon_Update;
			fpDraw = NewDungeon_Draw;
			fpFree = NewDungeon_Free;
			fpUnload = NewDungeon_Unload;
			break;

		case GS_DESERT:
			fpLoad = DesertDungeon_Load;
			fpInitialize = DesertDungeon_Initialize;
			fpUpdate = DesertDungeon_Update;
			fpDraw = DesertDungeon_Draw;
			fpFree = DesertDungeon_Free;
			fpUnload = DesertDungeon_Unload;
			break;

		case GS_RESPAWN:
			fpLoad = RespawnArea_Load;
			fpInitialize = RespawnArea_Initialize;
			fpUpdate = RespawnArea_Update;
			fpDraw = RespawnArea_Draw;
			fpFree = RespawnArea_Free;
			fpUnload = RespawnArea_Unload;
			break;

		case GS_WIN:
			fpLoad = Win_Load;
			fpInitialize = Win_Initialize;
			fpUpdate = Win_Update;
			fpDraw = Win_Draw;
			fpFree = Win_Free;
			fpUnload = Win_Unload;
			break;

		case GS_LOSE:
			fpLoad = Lose_Load;
			fpInitialize = Lose_Initialize;
			fpUpdate = Lose_Update;
			fpDraw = Lose_Draw;
			fpFree = Lose_Free;
			fpUnload = Lose_Unload;
			break;

		// Utility states (TODO: technically these 2 can be removed since itll be caught by default but we'll see)
		case GS_RESTART:
			break;

		case GS_QUIT:
			break;

		default:
			break;
	}
}


