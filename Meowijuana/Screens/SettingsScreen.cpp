#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SettingsScreen.hpp"

void Settings_Load() {

}

void Settings_Initialize() {

}

void Settings_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Settings screen", 0, 0);
}

void Settings_Draw() {
	
}

void Settings_Free() {

}

void Settings_Unload() {

}
