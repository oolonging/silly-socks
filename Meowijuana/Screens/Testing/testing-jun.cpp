#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "testing-jun.hpp"

void Jun_Load() {}

void Jun_Initialize() {}

void Jun_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Jun's testing screen", 0, 0);
}

void Jun_Draw() {}

void Jun_Free() {}

void Jun_Unload() {}
