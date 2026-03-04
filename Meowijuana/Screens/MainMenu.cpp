#include "AEEngine.h"
#include "../UI_Elements/UI_Elements.hpp"
#include "../GameStateManager.hpp"

// Buttons to display on the main 
// TODO: global variables break state isolation. find a way around it
static UI_Elements::Button creditsButton;
static UI_Elements::Button playButton;
static UI_Elements::Button settingsButton;
static UI_Elements::Button testButton;
static UI_Elements::Button tutorialButton;

// image asset to load in the middle of the screen
AEGfxTexture* CatastropheLogo = nullptr;

// void functions for buttons
void navigateToGame(void) { next = GS_LEVEL1; }
void navigateToSettings(void) { next = GS_SETTINGS; }
void navigateToCredits(void) { next = GS_CREDITS; }
void navigateToTest(void) { next = GS_A; }
void navigateToTutorial(void) { next = GS_X; }

void Mainmenu_Load() {
	CatastropheLogo = AEGfxTextureLoad("Assets/Images/Backgrounds/Cat.png");
}

void Mainmenu_Initialize()
{
	// set the button values
	creditsButton = UI_Elements::Button(Shapes::Quad{ {-300.0f, -200.0f}, 200.0f, 100.0f }, "Credits", Shapes::CENTER);
	playButton = UI_Elements::Button(Shapes::Quad{ {0.0f, -200.0f}, 200.0f, 100.0f }, "Play", Shapes::CENTER);
	settingsButton = UI_Elements::Button(Shapes::Quad{ {300.0f, -200.0f}, 200.0f, 100.0f }, "Settings", Shapes::CENTER);
	testButton = UI_Elements::Button(Shapes::Quad{ {400.0f, 200.0f}, 200.0f, 100.0f }, "Test", Shapes::CENTER);
	tutorialButton = UI_Elements::Button(Shapes::Quad{ {600.0f, 400.0f}, 200.0f, 100.0f }, "Tutorial", Shapes::CENTER);
	
	// set the button functions
	playButton.setOnClick(navigateToGame);
	creditsButton.setOnClick(navigateToCredits);
	settingsButton.setOnClick(navigateToSettings);
	testButton.setOnClick(navigateToTest);
	tutorialButton.setOnClick(navigateToTutorial);
}

void Mainmenu_Update() {
	// TODO: consider separating the update and render logic of buttons for easier debugging
}

void Mainmenu_Draw()
{
	// Draw the catastrophe logo in the middle of the screen
	Graphics::image(0, 0, 1600, 900, CatastropheLogo, Shapes::CENTER);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	creditsButton.draw();
	playButton.draw();
	settingsButton.draw();
	testButton.draw();
	tutorialButton.draw();
}

void Mainmenu_Free() {}

void Mainmenu_Unload()
{
	AEGfxTextureUnload(CatastropheLogo);
	CatastropheLogo = nullptr;
}
