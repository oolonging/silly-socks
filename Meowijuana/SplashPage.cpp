#pragma once

#include "SplashPage.hpp"
#include "AEEngine.h"
#include "UI_Elements.hpp"

extern int current, previous, next;

UI_Elements::Button testButton;
UI_Elements::Button testButton1;
UI_Elements::Button thirdtest;

AEGfxTexture* SplashScreen = nullptr;

void Splash_Load()
{
	if (!SplashScreen)
		SplashScreen = AEGfxTextureLoad("Assets/Cat.png");
}

void Splash_Initialize()
{
	testButton = UI_Elements::Button(Shapes::Quad{ {-300.0f, -200.0f}, 200.0f, 100.0f }, "Credits", Shapes::CENTER);
	testButton1 = UI_Elements::Button(Shapes::Quad{ {0.0f, -200.0f}, 200.0f, 100.0f }, "Play", Shapes::CENTER);
	thirdtest = UI_Elements::Button(Shapes::Quad{ {300.0f, -200.0f}, 200.0f, 100.0f }, "Settings", Shapes::CENTER);

}

void Go_play()
{
	next += 1;
}

void Splash_Update()
{
		// If button 1 is pressed, change to level 1 instead
		testButton1.setOnClick(Go_play);

}
void Splash_Draw()
{
	Graphics::image(0, 0, 1600, 900, SplashScreen, Shapes::CENTER);

	AEGfxTextureMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	testButton.draw();
	testButton1.draw();
	thirdtest.draw();
}

void Splash_Free()
{
	testButton = UI_Elements::Button{};
	testButton1 = UI_Elements::Button{};
	thirdtest = UI_Elements::Button{};
}

void Splash_Unload()
{
	AEGfxTextureUnload(SplashScreen);
}
