#include "AEEngine.h"
#include "UI_Elements.hpp"
#include "GameStateManager.hpp"

extern int current, previous, next;

// Buttons to display on the main menu
UI_Elements::Button creditsButton;
UI_Elements::Button playButton;
UI_Elements::Button settingsButton;

AEGfxTexture* CatastropheLogo = nullptr;

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

}

void Go_play()
{
	next = GS_LEVEL1;
}

void Mainmenu_Update()
{
	// If button 1 is pressed, change to level 1 instead
	playButton.setOnClick(Go_play);

}
void Mainmenu_Draw()
{
	// Draw the logo in the center of the screen
	Graphics::image(0, 0, 1600, 900, CatastropheLogo, Shapes::CENTER);

	AEGfxRenderMode(AE_GFX_RM_COLOR);
	AEGfxBlendMode(AE_GFX_BM_BLEND);

	creditsButton.draw();
	creditsButton.draw();
	playButton.draw();
	settingsButton.draw();
}

void Mainmenu_Free()
{
	creditsButton = UI_Elements::Button{};
	playButton = UI_Elements::Button{};
	settingsButton = UI_Elements::Button{};
}

void Mainmenu_Unload()
{
	AEGfxTextureUnload(CatastropheLogo);
}
