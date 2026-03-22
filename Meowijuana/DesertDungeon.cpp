#include "../../pch.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Managers/SpriteManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../PauseMenu.hpp"
#include "../../Settings.hpp"
#include "World.hpp"
#include "NewDungeon.hpp"



extern World::worldGrid grid;
std::pair<int, int> desertGridTile;
AEGfxTexture* desertDungeon = nullptr;


namespace Death {
	float opacity = 0.0f;
	bool dead = false;
	float fade;
}

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
	Settings::currentScreen = "TutorialDungeon.cpp";

	/*grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initMapTexture();
	grid.initTextureBox();*/

	EntityManager::init();
	auto* tutPlayer = EntityManager::getPlayer("player");

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(tutPlayer, gameData);

}

void DesertDungeon_Update() {
	auto* player = EntityManager::getPlayer("player");
	player->update(grid);



	if (player->getHp() <= 0) {
		player->setHp(0);

		Death::dead = true;
		player->isDead = true;

		if (Death::opacity < 255.0f) Death::opacity += 2.0f;;

		if (Death::opacity >= 255.0f) {
			Death::opacity = 255.0f;
			player->isDead = false;
			next = GS_RESPAWN;
		}

	}
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


