#include "../../pch.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Managers/SpriteManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "World.hpp"
#include "NewDungeon.hpp"


extern World::worldGrid grid;
std::pair<int, int> activeGridTile;
AEGfxTexture* bgDungeon = nullptr;



namespace Death {
	float opacity = 0.0f;
	bool dead = false;
	float fade;
}

void NewDungeon_Load() {
	SpriteManager::init();


	//// although it does look better smaller :(
	grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initTextureBox();
	//grid.initMapTextureSprite("Assets/DungeonTileData.txt");


	// this one is?? okay-ish????
	bgDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/NewDungeon.png");
	grid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Dungeon.txt");
}

void NewDungeon_Initialize() {

	EntityManager::init();
	
}

void NewDungeon_Update() {
	auto* player = EntityManager::getPlayer("player");
	player->update(grid);
	

	grid.replacingID(World::Teleporter1, World::TeleporterBlue);
	World::standOnTile(next, *player, grid, GS_TUTDUN, World::TeleporterBlue);


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

void NewDungeon_Draw() {
	Graphics::image(0, 0, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), bgDungeon, Shapes::CENTER);
	grid.drawTexture(grid);
	World::drawTile(activeGridTile, grid);
	World::drawTile({ 0,0 }, grid);

	EntityManager::draw("player");
}

void NewDungeon_Free() {
	grid.unloadMapTexture();
	World::freeGrid();
}

void NewDungeon_Unload() {
	AEGfxTextureUnload(bgDungeon);
	bgDungeon = nullptr;
}


