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
#include "InputManager.hpp"

// ---------------------------------------------------------------------------
// main

// Temporary function to help navigate between the screens
void screenSwitcher(void) {
	if (AEInputCheckTriggered(AEVK_1))
		next = GS_SPLASH;

	if (AEInputCheckTriggered(AEVK_2))
		next = GS_MAIN_MENU;

	if (AEInputCheckTriggered(AEVK_3))
		next = GS_SETTINGS;

	if (AEInputCheckTriggered(AEVK_4))
		next = GS_CREDITS;

	if (AEInputCheckTriggered(AEVK_5))
		next = GS_FARM;

	if (AEInputCheckTriggered(AEVK_6))
		next = GS_DUNGEON;

	if (AEInputCheckTriggered(AEVK_7))
		next = GS_TESTING;

	if (AEInputCheckTriggered(AEVK_8))
		next = GS_LEVEL1;

	if (AEInputCheckTriggered(AEVK_9))
		next = GS_LEVEL2;

	if(AEInputCheckTriggered(AEVK_0))
		next = GS_BACK;

	if (AEInputCheckTriggered(AEVK_R))
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
	// TODO: that init function that loads all fonts into memory and gives them proper names too
	Text::createFont("Assets/Fonts/buggy-font.ttf", 10, "default");

	// set text align
	Text::textAlign(Text::CENTER_H, Text::CENTER_V);

	GSM_Initialize(GS_TESTING);

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

			// testing out the debugging feature before I implement it into the rest of the modules, its F3 to toggle
			if (AEInputCheckTriggered(AEVK_F3)) {
				Settings::toggleDebugMode();
			}

			// also temporary in case soemthing breaks without it
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);

			fpUpdate();
			fpDraw();

			// Update input manager every frame
			Input::update();

			AESysFrameEnd();

			// Handle restart
			if (next == GS_RESTART) {
				fpFree();
				fpInitialize();
				next = current;
			}

			if(next == GS_BACK) {
				next = previous;
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
	Text::exit();
	World::freeGrid();

	AESysExit();
}
