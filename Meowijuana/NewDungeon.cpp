#include "../../pch.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Managers/SpriteManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "World.hpp"
#include "NewDungeon.hpp"


World::worldGrid newGrid;
std::pair<int, int> activeGridTile;
AEGfxTexture* bgDungeon = nullptr;

void NewDungeon_Load() {
	SpriteManager::init();


	//// Load the sprite sheet
	//if (!SpriteManager::getSpriteSheet("dungeon")) {

	//	std::cout << "loading file...";
	//	SpriteManager::loadSpriteSheet("dungeon", "Assets/DUNGEON/Tilemap/tilemap_packed.png", 192.0f, 176.0f, 16.0f, 16.0f);
	//}


	//// although it does look better smaller :(
	newGrid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	newGrid.initTextureBox();
	//newGrid.initMapTextureSprite("Assets/DungeonTileData.txt");


	// this one is?? okay-ish????
	//bgDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/NewDungeon.png");
	//newGrid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Dungeon.txt");


	// this whole collision map is cooked bruh
	//bgDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/Desert.png");
	//newGrid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Desert.txt");



	// sign has no collision, but that's also because collision box kinda wonky, half block sign, half block nothing
	bgDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/Farm.png");
	newGrid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Farm.txt");
}

void NewDungeon_Initialize() {

	EntityManager::init();
	
}

void NewDungeon_Update() {
	auto* player = EntityManager::getPlayer("player");
	player->update(newGrid);
	

}

void NewDungeon_Draw() {
	Graphics::image(0, 0, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), bgDungeon, Shapes::CENTER);
	newGrid.drawTexture(newGrid);
	World::drawTile(activeGridTile, newGrid);
	World::drawTile({ 0,0 }, newGrid);

	EntityManager::draw("player");
}

void NewDungeon_Free() {
	newGrid.unloadMapTexture();
	World::freeGrid();
}

void NewDungeon_Unload() {
	AEGfxTextureUnload(bgDungeon);
	bgDungeon = nullptr;
}


