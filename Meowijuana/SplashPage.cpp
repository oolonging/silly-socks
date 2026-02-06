#pragma once

#include "SplashPage.hpp"
#include "AEEngine.h"
#include "UI_Elements.hpp"

extern int current, previous, next;

UI_Elements::Button creditsButton;
UI_Elements::Button playButton;
UI_Elements::Button settingsButton;

AEGfxTexture* SplashScreen = nullptr;

void Splash_Load()
{
	if (!SplashScreen)
		SplashScreen = AEGfxTextureLoad("Assets/Cat.png");
}

void Splash_Initialize()
{
	creditsButton = UI_Elements::Button(Shapes::Quad{ {-300.0f, -200.0f}, 200.0f, 100.0f }, "Credits", Shapes::CENTER);
	playButton = UI_Elements::Button(Shapes::Quad{ {0.0f, -200.0f}, 200.0f, 100.0f }, "Play", Shapes::CENTER);
	settingsButton = UI_Elements::Button(Shapes::Quad{ {300.0f, -200.0f}, 200.0f, 100.0f }, "Settings", Shapes::CENTER);

}

void Go_play()
{
	next += 1;
}

void Splash_Update()
{
		// If button 1 is pressed, change to level 1 instead
		playButton.setOnClick(Go_play);

}
void Splash_Draw()
{
	Graphics::image(0, 0, 1600, 900, SplashScreen, Shapes::CENTER);

	AEGfxRenderMode(AE_GFX_RM_COLOR);
	AEGfxBlendMode(AE_GFX_BM_BLEND);

	creditsButton.draw();
	creditsButton.draw();
	playButton.draw();
	settingsButton.draw();
}

void Splash_Free()
{
	creditsButton = UI_Elements::Button{};
	playButton = UI_Elements::Button{};
	settingsButton = UI_Elements::Button{};
}

void Splash_Unload()
{
	AEGfxTextureUnload(SplashScreen);
}
