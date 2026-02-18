#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SplashScreen.hpp"

void Splash_Load() {

}

void Splash_Initialize() {

}

void Splash_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Splash screen", 0, 0);
}

void Splash_Draw() {

}

void Splash_Free() {

}

void Splash_Unload() {

}
