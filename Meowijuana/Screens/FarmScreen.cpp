#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "FarmScreen.hpp"

void Farm_Load() {

}

void Farm_Initialize() {

}

void Farm_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Farm Screen", 0, 0);

}

void Farm_Draw() {

}

void Farm_Free() {

}

void Farm_Unload() {

}
