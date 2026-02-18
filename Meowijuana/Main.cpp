// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include <iostream>
#include "AEEngine.h"

#include "Graphics.hpp"
#include "UI_Elements.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Settings.hpp"
#include "GameStateManager.hpp"

// ---------------------------------------------------------------------------
// main

// Temporary function to help navigate between the screens
void screenSwitcher(void) {
	if (AEInputCheckTriggered(AEVK_1)) // Works
		next = GS_SPLASH;

	if (AEInputCheckTriggered(AEVK_2)) // DOESNT WORK ON RELOAD
		next = GS_MAIN_MENU;

	if (AEInputCheckTriggered(AEVK_3)) // Works: but doesnt go from menu
		next = GS_SETTINGS;

	if (AEInputCheckTriggered(AEVK_4)) // Works
		next = GS_CREDITS;

	if (AEInputCheckTriggered(AEVK_5)) // Works
		next = GS_FARM;

	if (AEInputCheckTriggered(AEVK_6)) // Works
		next = GS_DUNGEON;

	if (AEInputCheckTriggered(AEVK_7)) // Works
		next = GS_TESTING;

	if (AEInputCheckTriggered(AEVK_8)) // Problem when reloading
		next = GS_LEVEL1;

	if (AEInputCheckTriggered(AEVK_9)) // Works I guess?
		next = GS_LEVEL2;

	if (AEInputCheckTriggered(AEVK_0)) // Crashes when done on main menu
		next = GS_RESTART;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Create a 1600 x 900 windowed application at 60 FPS
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
	AESysSetWindowTitle("Silly Socks | Meowijuana");

	// reset the system modules
	AESysReset();

	// Init globals
	Shapes::init();
	World::initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 100);
	Text::setFont("Assets/Fonts/buggy-font.ttf", 10);


	GSM_Initialize(GS_MAIN_MENU);

	// fixed the loop
	while (current != GS_QUIT) {
		// set the function pointers for current state
		GSM_Update();

		// Enter state
		fpLoad();
		fpInitialize();

		// Level loop
		while (current == next) {
			AESysFrameStart();

			// temporary  during the debugging phase
			screenSwitcher();
			if (AEInputCheckTriggered(AEVK_ESCAPE)) {
				next = GS_QUIT;
			}

			fpUpdate();
			fpDraw();

			AESysFrameEnd();

			// Handle restart
			if (next == GS_RESTART) {
				fpFree();
				fpInitialize();
				next = current;
			}
		}

		// Exit state
		fpFree();
		fpUnload();

		previous = current;
		current = next;
	}

	// Free resources
	Shapes::exit();
	Text::unloadFont();
	World::freeGrid();

	AESysExit();
}
