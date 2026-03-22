#include "pch.h"
#include "LoseScreen.hpp"
#include "GameStateManager.hpp"
#include "Graphics.hpp"

void Lose_Load() {

}

void Lose_Initialize() {

}

void Lose_Update() {

}

void Lose_Draw() {

	//black bg
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::textSize(70.0f);
	Text::textAlign(Text::CENTER_H, Text::CENTER_V);

	Text::text("Game Over", 0.0f, 0.0f);

	// Click anywhere to return to main menu
	if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_E)) {
		next = GS_MAIN_MENU;
	}

}

void Lose_Free() {

}

void Lose_Unload() {

}