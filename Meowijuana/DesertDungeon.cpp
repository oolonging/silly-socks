#include "../../pch.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Managers/SpriteManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../PauseMenu.hpp"
#include "../../Settings.hpp"
#include "World.hpp"
#include "NewDungeon.hpp"
#include "TutorialDungeon.hpp"

extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;

extern World::worldGrid grid;
std::pair<int, int> desertGridTile;
AEGfxTexture* desertDungeon = nullptr;

struct DesertDungeonState {
	bool visited = false;
	bool cleared = false;

	// Entities

	Inventory::Weapon* pWeapon;

	// local player
	Entity::Player* localPlayer = nullptr;


};

DesertDungeonState desertDungeonState;


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
	Settings::currentScreen = "DesertDungeon.cpp";

	EntityManager::init();

	/*grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initMapTexture();
	grid.initTextureBox();*/

	EntityManager::init();
	desertDungeonState.localPlayer = EntityManager::getPlayer("player");

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(desertDungeonState.localPlayer, gameData);
	
	// Initialize the boss if the room is not cleared yet
	if (!desertDungeonState.cleared) {
		// summon the boss enemy
		EntityManager::spawnEnemies(1, 10.0f, 10.0f, 3);

		// Give the boss a weapon
		Inventory::Weapon* eWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::WOODEN_SWORD));
		
		// Make Teleporter Deactivated
		grid.replacingID(World::TeleporterRed, World::Teleporter3);
	}


	if (!desertDungeonState.visited) {
		
	}

	// Initialize local player
	desertDungeonState.localPlayer = EntityManager::getPlayer("player");
	desertDungeonState.localPlayer->setPosition(0.0f, 350.0f);

	// Give the player a weapon
	desertDungeonState.pWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD));
	if (desertDungeonState.pWeapon) {
		desertDungeonState.localPlayer->setWeapon(desertDungeonState.pWeapon);
		desertDungeonState.localPlayer->setAtkSpd(desertDungeonState.pWeapon->getAttackSpeed());
	}
}

void DesertDungeon_Update() {
	auto* player = desertDungeonState.localPlayer;
	player->update(grid);
	player->tickAttackTimer();
	inv.update(player);

	if (EntityManager::allEnemiesDead()) {
		World::dungeonTracker[2] = true;
		desertDungeonState.cleared = true;
		grid.replacingID(World::Teleporter3, World::TeleporterRed);
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON) && player->canAttack()) {
		EntityManager::attackEnemies(*player);
		World::checkCarrotSwordConsume(inv, *player);
		player->resetAttackTimer(); // reset once after hitting all enemies
	}


	if (EntityManager::allEnemiesDead())
	{
		World::dungeonTracker[2] = true;
		World::checkNum = 2;
	}

	if (AEInputCheckTriggered(AEVK_0))
	{
		World::dungeonTracker[2] = true;
		World::checkNum = 2;
	}

	if (AEInputCheckTriggered(AEVK_F10))
	{
		Death::deathCounter = 3;
	}

	if (World::dungeonTracker[World::checkNum])
	{
		int count = 0;

		for (int i = 0; i < World::checkNum; i++)
		{
			if (World::dungeonTracker[i] == false) break;
			else count++;
		}

		if (count == World::checkNum + 1) // if all 3 stages clear, show win page
		{
			next = GS_WIN;
		}
	}

	// update the boss entity
	EntityManager::updateEnemies(*(desertDungeonState.localPlayer));

	World::standOnTile(next, *player, grid, GS_FARM, World::TeleporterRed);

	Clamping::clamper(player);
}

void DesertDungeon_Draw() {
	Graphics::image(0, 0, 1600.0f, 900.0f, desertDungeon, Shapes::CENTER);
	grid.drawTexture(grid);
	World::drawTile(desertGridTile, grid);


	// draws the boss enemy
	EntityManager::drawEnemies(*(desertDungeonState.localPlayer), grid, false);

	
	if (showInventory)
	{
		inv.draw();
	}


	// Draw indicator over teleporter when dungeon is cleared
	if (World::dungeonTracker[2])
	{
		static Animations::Indicator teleporterIndicator;
		World::drawIndicatorsOnTileType(grid, World::TeleporterRed, teleporterIndicator);
	}

	// Draw the player entity
	desertDungeonState.localPlayer->draw();
}

void DesertDungeon_Free() {
	//grid.unloadMapTexture();
	//World::freeGrid();

	inv.saveInventory(desertDungeonState.localPlayer, gameData);
	inv.clear(desertDungeonState.localPlayer);
	inv.setPlayer(nullptr);

	EntityManager::clear();
}

void DesertDungeon_Unload() {
	AEGfxTextureUnload(desertDungeon);
	desertDungeon = nullptr;
}


