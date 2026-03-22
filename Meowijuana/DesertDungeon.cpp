#include "../../pch.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Managers/SpriteManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "World.hpp"
#include "NewDungeon.hpp"



extern World::worldGrid grid;
std::pair<int, int> desertGridTile;
AEGfxTexture* desertDungeon = nullptr;

void DesertDungeon_Load() {
	SpriteManager::init();


	//// although it does look better smaller :(
	grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initTextureBox();
	//grid.initMapTextureSprite("Assets/DungeonTileData.txt");


	// this one is?? okay-ish????
	desertDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/Desert.png");
	grid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Desert.txt");

}

void DesertDungeon_Initialize() {

	EntityManager::init();

}

void DesertDungeon_Update() {
	auto* player = EntityManager::getPlayer("player");
	player->update(grid);


}

void DesertDungeon_Draw() {
	Graphics::image(0, 0, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), desertDungeon, Shapes::CENTER);
	grid.drawTexture(grid);
	World::drawTile(desertGridTile, grid);

	EntityManager::draw("player");
}

void DesertDungeon_Free() {
	grid.unloadMapTexture();
	World::freeGrid();
}

void DesertDungeon_Unload() {
	AEGfxTextureUnload(desertDungeon);
	desertDungeon = nullptr;
}


