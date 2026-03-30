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
static bool restarted = false;

struct NewDungeonState {
	bool visited = false;
	bool cleared = false;
};

NewDungeonState newDungeonState;


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
	localPlayer->setPosition(500.0f, 300.0f);
	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(localPlayer, gameData);

	// spawn 3 enemies
	if(!newDungeonState.cleared)
	EntityManager::spawnEnemies(2, 400.0f, 200.0f, 1.5f);

	// give the player a weapon
	pWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD));
	if (pWeapon) {
		localPlayer->setWeapon(pWeapon);
		localPlayer->setAtkSpd(pWeapon->getAttackSpeed());
	}

	if (!newDungeonState.cleared) {
		// disable the way back.
		grid.replacingID(World::TeleporterGreen, World::Teleporter2);
	}

	// marks this dungeon as visited
	newDungeonState.visited = true;
}

void NewDungeon_Update() {
	localPlayer->update(grid);
	localPlayer->tickAttackTimer();
	inv.update(localPlayer);

	// Temp restart after every win or lose
	if (World::restartLevels[3])
	{
		newDungeonState.visited = false;
		newDungeonState.cleared = false;
		World::restartLevels[3] = false;
	}

	// sets the room as cleared when all the enemies are dead
	if (EntityManager::allEnemiesDead()) {
		World::dungeonTracker[1] = true;
		newDungeonState.cleared = true;
		grid.replacingID(World::Teleporter2, World::TeleporterGreen);
	}


	// Attacking and using of cherry 
	if (AEInputCheckTriggered(AEVK_LBUTTON) && localPlayer->canAttack()) {
		EntityManager::attackEnemies(*localPlayer);
		World::checkCarrotSwordConsume(inv, *localPlayer);
		localPlayer->resetAttackTimer(); // reset once after hitting all enemies
	}
	if (AEInputCheckTriggered(AEVK_E))
	{
		World::useInventoryItem(grid, inv, *localPlayer);
	}

	grid.replacingID(World::Teleporter1, World::TeleporterBlue);
	World::standOnTile(next, *localPlayer, grid, GS_TUTDUN, World::TeleporterBlue);

	EntityManager::updateEnemies(*localPlayer);

	if (AEInputCheckTriggered(AEVK_0))
	{
		World::dungeonTracker[1] = true;
		World::checkNum = 1;
		newDungeonState.cleared = true;
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


	World::standOnTile(next, *localPlayer, grid, GS_FARM, World::TeleporterGreen);


	Clamping::clamper(localPlayer);
}

void NewDungeon_Draw() {
	Graphics::image(0, 0, 1600.0f, 900.0f, bgDungeon, Shapes::CENTER);
	grid.drawTexture(grid);
	//World::drawTile(activeGridTile, grid);

	EntityManager::drawEnemies(*localPlayer, grid, false);

	if (showInventory)
	{
		inv.draw();
	}

	// Draw indicator over teleporter when dungeon is cleared
	if (newDungeonState.cleared)
	{
		static Animations::Indicator teleporterIndicator;
		World::drawIndicatorsOnTileType(grid, World::TeleporterGreen, teleporterIndicator);
	}

	// draw the local player last
	localPlayer->draw();
}

void NewDungeon_Free() {
	//grid.unloadMapTexture();
	//World::freeGrid();

	inv.saveInventory(localPlayer, gameData);
	inv.clear(localPlayer);
	inv.setPlayer(nullptr);

	EntityManager::clear();
}

void NewDungeon_Unload() {
	AEGfxTextureUnload(bgDungeon);
	bgDungeon = nullptr;
}


