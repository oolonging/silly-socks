#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../World.hpp"
#include "testing-andrea.hpp"

void Andrea_Load() {}

void Andrea_Initialize() {}

void Andrea_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Andrea's testing screen", 0, 0);
}

void Andrea_Draw() {}

void Andrea_Free() {}

void Andrea_Unload() {}
