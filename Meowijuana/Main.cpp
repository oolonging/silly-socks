// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

#include "Graphics.hpp"
#include "UI_Elements.hpp"
#include "Entity.hpp"



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

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Silly Socks | Meowijuana");

	// reset the system modules
	AESysReset();

	Shapes::init();
	Text::setFont("Assets/Fonts/buggy-font.ttf", 20);

	// test button
	UI_Elements::Button testButton;
	UI_Elements::Button testButton1;
	UI_Elements::Button thirdtest;

	// test player
	Entity::Player testPlayer = Entity::Player(0.0f, 0.0f, 50.0f, 50.0f, 100.0f, 5.0f, 0.0f);

	// set the button to the center of the screen
	testButton = UI_Elements::Button(Shapes::Quad{ {0.0f, 0.0f}, 200.0f, 100.0f }, "corner tada", Shapes::CORNER);
	testButton1 = UI_Elements::Button(Shapes::Quad{ {-150.0f, 0.0f}, 200.0f, 100.0f }, "center tada", Shapes::CENTER);
	thirdtest = UI_Elements::Button(Shapes::Quad{ {-300.0f, -200.0f}, 200.0f, 100.0f }, "just in case tada", Shapes::CORNER);

	

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// reset background
		Color::background(Color::CL_Color_Create(255, 255, 0));

		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// Your own update logic goes here
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);


		// Test button
		testButton.draw();
		testButton1.draw();
		thirdtest.draw();

		// Test player
		testPlayer.draw();

		// Your own rendering logic goes here


		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	// Free shapes
	Shapes::exit();

	// Free textures

	// free font (it's free?)
	Text::unloadFont();

	// free the system
	AESysExit();
}