#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SplashScreen.hpp"


namespace SplashScreen {
	// Assets
	AEGfxTexture* digipenLogo;

	// Variables for fade in effect
	float opacity{};
}

void Splash_Load() {
	// Load in digipen logo
	SplashScreen::digipenLogo = AEGfxTextureLoad("Assets/Images/Logos/Digipen_Red.png");
}

void Splash_Initialize() {
	SplashScreen::opacity = 0.0f;
}

void Splash_Update() {
	if(SplashScreen::opacity <= 255.0f)
		SplashScreen::opacity++;

	if (SplashScreen::opacity >= 255.0f)
		next = GS_MAIN_MENU;

	// If the user clicks mouse button then just skip the whole sequence
	if (AEInputCheckTriggered(AEVK_LBUTTON))
		next = GS_MAIN_MENU;
}

void Splash_Draw() {
	if (SplashScreen::digipenLogo)
		Graphics::image(0.0f, 0.0f, 1525.0f, 445.0f, SplashScreen::digipenLogo, Shapes::CENTER);
		// TODO: Add functions that gets the width and height of an image. (see also: texture atlas)
	else {
		Color::textFill(255.0f, 0.0f, 0.0f);
		Text::text("Digipen", 0.0f, 0.0f);
	}

	// Cheap transparency hack. TODO: add transparency to image function
	Color::fill(255.0f, 255.0f, 255.0f, 255.0f - SplashScreen::opacity);
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Shapes::rect(0, 0, 1600, 900, Shapes::CENTER);
}

void Splash_Free() {

}

void Splash_Unload() {
	AEGfxTextureUnload(SplashScreen::digipenLogo);
	SplashScreen::digipenLogo = nullptr;
}
