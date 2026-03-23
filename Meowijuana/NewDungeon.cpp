#include "../../pch.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Managers/SpriteManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "World.hpp"
#include "NewDungeon.hpp"
#include "TutorialDungeon.hpp"
#include "Settings.hpp"


extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;

extern World::worldGrid grid;
std::pair<int, int> activeGridTile;
AEGfxTexture* bgDungeon = nullptr;

static Entity::Player* localPlayer;
static Inventory::Weapon* pWeapon;

bool ifClear;

//namespace Death {
//	float opacity = 0.0f;
//	bool dead = false;
//	float fade;
//}

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
	Settings::currentScreen = "NewDungeon.cpp";

	EntityManager::init();

	localPlayer = EntityManager::getPlayer("player");

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(localPlayer, gameData);

	// spawn 3 enemies
	EntityManager::spawnEnemies(2, 400.0f, 400.0f);

	// give the player a weapon
	pWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD));
	if (pWeapon) {
		localPlayer->setWeapon(pWeapon);
		localPlayer->setAtkSpd(pWeapon->getAttackSpeed());
	}


	
}

void NewDungeon_Update() {
	auto* player = EntityManager::getPlayer("player");
	player->update(grid);
	inv.update(player);

	if (AEInputCheckTriggered(AEVK_LBUTTON) && player->canAttack()) {
		EntityManager::attackEnemies(*player);
		World::checkCarrotSwordConsume(inv, *player);
		player->resetAttackTimer(); // reset once after hitting all enemies
	}

	grid.replacingID(World::Teleporter1, World::TeleporterBlue);
	World::standOnTile(next, *player, grid, GS_TUTDUN, World::TeleporterBlue);

	EntityManager::updateEnemies(*localPlayer);


	if (player->getHp() <= 0) {
		player->setHp(0);

		Death::dead = true;
		player->isDead = true;

		if (Death::opacity < 255.0f) Death::opacity += 2.0f;;

		if (Death::opacity >= 255.0f) {
			Death::opacity = 255.0f;
			player->isDead = false;
			Death::deathCounter++;
			next = GS_RESPAWN;
		}

		if (Death::deathCounter >= 3)
		{
			next = GS_LOSE;
		}
	}

	if (AEInputCheckTriggered(AEVK_0))
	{
		World::dungeonTracker[1] = true;
		World::checkNum = 1;
	}

	if (AEInputCheckTriggered(AEVK_F10))
	{
		Death::deathCounter = 3;
	}

	if (EntityManager::allEnemiesDead())
	{
		World::dungeonTracker[1] = true;
		World::checkNum = 1;
	}


	World::standOnTile(next, *player, grid, GS_FARM, World::TeleporterGreen);
}

void NewDungeon_Draw() {
	Graphics::image(0, 0, 1600.0f, 900.0f, bgDungeon, Shapes::CENTER);
	grid.drawTexture(grid);
	World::drawTile(activeGridTile, grid);
	World::drawTile({ 0,0 }, grid);

	EntityManager::draw("player");
	EntityManager::drawEnemies(*localPlayer, grid, false);

	if (showInventory)
	{
		inv.draw();
	}
}

void NewDungeon_Free() {
	auto* player = EntityManager::getPlayer("player");
	grid.unloadMapTexture();
	World::freeGrid();

	inv.saveInventory(player, gameData);
	inv.clear(player);
	inv.setPlayer(nullptr);
}

void NewDungeon_Unload() {
	AEGfxTextureUnload(bgDungeon);
	bgDungeon = nullptr;
}


