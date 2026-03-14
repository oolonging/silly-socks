#include "../pch.h"
#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "FarmScreen.hpp"
#include "../Managers/EntityManager.hpp"

bool firstdungeon = true;

void Farm_Load() {

}


void Farm_Initialize() {

	EntityManager::init();

	// potentially for seeds
	// Inventory::init();



}



void Farm_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Farm Screen", 0, 0);

	// player update
	auto* player = EntityManager::getPlayer("player");
	player->update();



	// Level transition check
	if (player->getX() > (AEGfxGetWindowWidth() / 2) && firstdungeon) {
		firstdungeon = false;
		next = GS_X;
	}
	else if (player->getX() > (AEGfxGetWindowWidth() / 2)) {
		next = GS_DUNGEON;
	}
}



void Farm_Draw() {
	auto* player = EntityManager::getPlayer("player");
	player->draw();


}

void Farm_Free() {

}

void Farm_Unload() {

}


