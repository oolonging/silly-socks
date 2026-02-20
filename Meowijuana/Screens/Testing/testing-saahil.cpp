#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "testing-saahil.hpp"

void Saahil_Load() {}

void Saahil_Initialize() {}

void Saahil_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Testing Saahil screen", 0, 0);
}

void Saahil_Draw() {}

void Saahil_Free() {}

void Saahil_Unload() {}
