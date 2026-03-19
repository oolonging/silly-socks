//#include "../../pch.h"
//#include "../../GameStateManager.hpp"
//#include "../../Graphics.hpp"
//#include "../../Managers/SpriteManager.hpp"
//#include "../../Managers/EntityManager.hpp"
//#include "World.hpp"
//#include "NewDungeon.hpp"
//
//
//World::worldGrid newGrid;
//std::pair<int, int> activeGridTile;
//
//void NewDungeon_Load() {
//	//SpriteManager::init();
//
//	// Load the cat sprite sheet
//	SpriteManager::loadSpriteSheet("dungeon", "Assets/DUNGEON/Tilemap/tilemap_packed.png", 192.0f, 176.0f, 16.0f, 16.0f);
//
//	// although it does look better smaller :(
//	newGrid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
//	newGrid.initTextureBox();
//	newGrid.initMapTextureSprite("Assets/DungeonTileData.txt");
//	newGrid.fillGrid("../../Assets/LevelMaps/TasteTest.txt");
//}
//
//void NewDungeon_Initialize() {
//
//	EntityManager::init();
//	
//}
//
//void NewDungeon_Update() {
//	auto* player = EntityManager::getPlayer("player");
//	player->update(newGrid);
//	
//
//}
//
//void NewDungeon_Draw() {
//	newGrid.drawTexture(newGrid);
//	World::drawTile(activeGridTile, newGrid);
//	World::drawTile({ 0,0 }, newGrid);
//
//	EntityManager::draw("player");
//}
//
//void NewDungeon_Free() {
//	newGrid.unloadMapTexture();
//	World::freeGrid();
//}
//
//void NewDungeon_Unload() {}
//
//
