// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include <iostream>
#include "AEEngine.h"

#include "Graphics.hpp"
#include "UI_Elements.hpp"
#include "GameStateManager.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Settings.hpp"

extern int current, previous, next;
extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;


// void meow
void meow(void) {
	std::cout << "Meow" << std::endl;
}

// ---------------------------------------------------------------------------
// main

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

	AEGfxTexture* dungeonTile = AEGfxTextureLoad("Assets/Tiles/DUNGEON_TILE.png");
	UI_Elements::Button sampleButton = UI_Elements::Button(Shapes::Quad{ {-800.0, -350.0f}, 200.0f, 100.0f }, "Sample Button", Shapes::CORNER);
	sampleButton.setOnClick(meow);

	GSM_Initialize(Splash);

	while (current != GS_QUIT)
	{
		if (current != GS_RESTART)
		{
			GSM_Update();
			fpLoad();
		}
		else
		{
			next = previous;
			current = previous;
		}

		fpInitialize();
		Color::noStroke();

		// -------- LEVEL LOOP --------
		while (next == current)
		{
			AESysFrameStart();

			// the fuck? why does removing this line break it
			Graphics::image(-800.0f, 450.0f, 1600.0f, 900.0f, dungeonTile, Shapes::CORNER);
			Color::fill(255, 255, 255);

			// try tiling it

			for (int i = 0; i < 16; i++) {
				for (int j = 0; j < 9; j++) {
					Graphics::image(-800.0f + i * 100.0f, 450.0f - j * 100.0f, 100.0f, 100.0f, dungeonTile, Shapes::CORNER);
				}
			}

			sampleButton.draw();

			if (AEInputCheckTriggered(AEVK_ESCAPE) ||
				0 == AESysDoesWindowExist())
			{
				next = GS_QUIT;
			}

			fpUpdate();
			
			fpDraw();

			AESysFrameEnd();
		}
		// ----------------------------

		fpFree();

		if (next != GS_RESTART)
			fpUnload();

		previous = current;
		current = next;
	}

	// Free shapes
	Shapes::exit();

	// Free texture dungeonTile
	AEGfxTextureUnload(dungeonTile);

	// free font (it's free?)
	Text::unloadFont();

	// Free the grid
	World::freeGrid();

	// free the system
	AESysExit();
}