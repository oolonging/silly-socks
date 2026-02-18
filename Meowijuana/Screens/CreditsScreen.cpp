#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "CreditsScreen.hpp"

void Credits_Load() {

}

void Credits_Initialize() {

}

void Credits_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Credits screen", 0, 0);
}

void Credits_Draw() {

}

void Credits_Free() {

}

void Credits_Unload() {

}
