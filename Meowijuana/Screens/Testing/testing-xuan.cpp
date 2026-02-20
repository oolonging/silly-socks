#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "testing-xuan.hpp"

void Xuan_Load() {}

void Xuan_Initialize() {}

void Xuan_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Xuan's testing screen", 0, 0);
}

void Xuan_Draw() {}

void Xuan_Free() {}

void Xuan_Unload() {}
