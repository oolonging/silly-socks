#include <iostream>
#include "GameStateManager.hpp"

// New imports, remove this comment later
#include "Screens/SplashScreen.hpp"
#include "Screens/MainMenu.hpp"
#include "Screens/SettingsScreen.hpp"
#include "Screens/CreditsScreen.hpp"
#include "Screens/FarmScreen.hpp"
#include "Screens/DungeonScreen.hpp"
#include "Screens/TestingScreen.hpp"

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

		// Utility states (TODO: technically these 2 can be removed since itll be caught by default but we'll see)
		case GS_RESTART:
			break;

		case GS_QUIT:
			break;

		default:
			break;
	}
}
