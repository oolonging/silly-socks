#include "AEEngine.h"
#include "UI_Elements.hpp"
#include "GameStateManager.hpp"

extern int current, previous, next;

// Buttons to display on the main menu
UI_Elements::Button creditsButton;
UI_Elements::Button playButton;
UI_Elements::Button settingsButton;

AEGfxTexture* CatastropheLogo = nullptr;

// void functions for buttons
void navigateToGame(void) { next = GS_LEVEL1; }
void navigateToSettings(void) { next = GS_SETTINGS; }
void navigateToCredits(void) { next = GS_CREDITS; }

void Mainmenu_Load()
{
	if (!CatastropheLogo)
		CatastropheLogo = AEGfxTextureLoad("Assets/Cat.png");
}

void Mainmenu_Initialize()
{
	creditsButton = UI_Elements::Button(Shapes::Quad{ {-300.0f, -200.0f}, 200.0f, 100.0f }, "Credits", Shapes::CENTER);
	playButton = UI_Elements::Button(Shapes::Quad{ {0.0f, -200.0f}, 200.0f, 100.0f }, "Play", Shapes::CENTER);
	settingsButton = UI_Elements::Button(Shapes::Quad{ {300.0f, -200.0f}, 200.0f, 100.0f }, "Settings", Shapes::CENTER);

	playButton.setOnClick(navigateToGame);
	creditsButton.setOnClick(navigateToCredits);
	settingsButton.setOnClick(navigateToSettings);
}

void Mainmenu_Update() {}

void Mainmenu_Draw()
{
	Graphics::image(0, 0, 1600, 900, CatastropheLogo, Shapes::CENTER);

	AEGfxRenderMode(AE_GFX_RM_COLOR);
	AEGfxBlendMode(AE_GFX_BM_BLEND);

	creditsButton.draw();
	creditsButton.draw();
	playButton.draw();
	settingsButton.draw();
}

void Mainmenu_Free() {}

void Mainmenu_Unload()
{
	AEGfxTextureUnload(CatastropheLogo);
}
