/*
 * @file LoseScreen.cpp
 * @author Jun
 * @brief Lose Screen after dying
 */

#include "pch.h"
#include "LoseScreen.hpp"

#include "GameStateManager.hpp"
#include "Graphics.hpp"
#include "Settings.hpp"
#include "World.hpp"

void Lose_Load() {

}

void Lose_Initialize() {
	Settings::currentScreen = "LoseScreen.cpp";

}

void Lose_Update() {

}

void Lose_Draw() {

	//black bg
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::textSize(140.0f);
	Text::textAlign(Text::CENTER_H, Text::CENTER_V);
	Text::text("GAME OVER", 0.0f, 0.0f);

	Text::textSize(25.0f);
	Text::text("Click anywhere to return to main menu", 0.0f, -140.0f);

	// Click anywhere to return to main menu
	if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_E)) {
		next = GS_MAIN_MENU;
		World::ResetAllGameState();
	}


}

void Lose_Free() {

}

void Lose_Unload() {

}