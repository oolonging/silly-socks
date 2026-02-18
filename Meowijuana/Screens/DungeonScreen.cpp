#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "DungeonScreen.hpp"

void Dungeon_Load() {

}

void Dungeon_Initialize() {

}

void Dungeon_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Dungeon screen", 0, 0);
}

void Dungeon_Draw() {

}

void Dungeon_Free() {

}

void Dungeon_Unload() {

}
