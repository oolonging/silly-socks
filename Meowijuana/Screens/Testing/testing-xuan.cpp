#include "../../pch.h"
#include "testing-xuan.hpp"

#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../World.hpp"
#include "../../Settings.hpp"
#include "../../Managers/SpriteManager.hpp"


extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;
AEGfxTexture* respawnDungeon = nullptr;

// will update with new dialogue once tiletypes and consumables come into play

extern World::worldGrid grid;
std::pair<int, int> prevActiveTile2;
std::pair<int, int> activeTile2;
static Animations::Indicator smellind;
static Animations::Indicator dummind;
static bool hasCarrotSword = false;


namespace TutorialScreen {

	Entity::NPC* activeSpeaker = nullptr;
	UI_Elements::DialogueBox dialogueBox;

	enum class TutorialState {
		SMELLY_TALK,
		SMELLY_IDLE,
		SMELLY_PART2,
		FINISHED

	};

	TutorialState state = TutorialState::SMELLY_TALK;
	bool deadDummy = false; // oh no :(

}

void Xuan_Load() {

	SpriteManager::init();

	grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initTextureBox();

	respawnDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/Respawn.png");
	grid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Respawn.txt");

}

void Xuan_Initialize() {
	Settings::currentScreen = "testing-xuan.cpp";


	//TileManager::init();

	/*grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);*/
	//grid.initMapTexture();
	//grid.initTextureBox();


	//grid.fillGrid("../../Assets/LevelMaps/DungeonNPCRoom.txt");
	//grid.outWorldMap("../../Assets/LevelMaps/Checkalso.txt");


	// Initialize entities
	EntityManager::init();

	auto* dummy = EntityManager::create<Entity::Enemy>("dummy", -100.0f, 50.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
	auto* smelly = EntityManager::getNPC("smelly");

	smelly->setSprite(AEGfxTextureLoad("Assets/Images/Entities/smelly.png")); // plus 1 poorly scaled placeholder becauase i dont want my lecturers staring at me while i work


	smelly->setDialogLines({
		"Hey [PLAYER]! Welcome to the dungeons! There's 4 levels to each dungeon, each with increasing levels of difficulty. We're on the first floor, so it's relatively safe for now.",
		"You'll need to learn how to defend yourself. Here! I've taken the liberty to pass you a [CARROT SWORD]. I'll walk you through this floor, and then you'll be on your own for the next few.",
		"First off, see that [DUMMY] over there? It's supposed to represent enemies.\n This one here won't fight back, but if you encounter real ones in the dungeon, they'll attack on sight.",
		"Use your [LEFT MOUSE BUTTON] to attack! Go give it a try.",

		"@",

		"Nice! OK, seems like my job here is done. I'm gonna go clock off for the day.",
		"To get to the next floor, just head to that teleporter over there to continue. They're those white squares on the ground. The next time you come back in, you'll be directed straight to the first floor.",
		"You can continue in the dungeons if you'd like! I'll cart you out if something bad happens.",
		"See you around!",

		"@"
	});

	smelly->setIdleLines({
		"Go on! Don't feel bad!",
		"The dummy can't grow arms and kill itself y'know...",

		"#",

		"If it makes you feel any better- or, er- less guilty, You'll need to get rid of enemies while you traverse the dungeons anyway. Might as well give it a try?"
	});


	// Initialize dialogue box
	TutorialScreen::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	auto* player = EntityManager::getPlayer("player");
	player->setPosition(-800.0f, 50.0f);

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(player, gameData);

	Inventory::Item* item = Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD);

	Inventory::Weapon* weapon = dynamic_cast<Inventory::Weapon*>(item);

	if (weapon)
	{
		player->setWeapon(weapon);
		player->setAtkSpd(weapon->getAttackSpeed());
		Inventory::Item* item = Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD);
		Inventory::Weapon* weapon = dynamic_cast<Inventory::Weapon*>(item);
	}
	// check if player already has it
	for (int i = 0; i < player->getInventorySize(); i++)
	{
		if (player->getInventoryItem(i) != nullptr &&
			player->getInventoryItem(i)->getID() == Inventory::ItemID::CARROT_SWORD_INV)
		{
			hasCarrotSword = true;
			break;
		}
	}

	// only give if first time AND doesn't have it
	if (!hasCarrotSword)
	{
		inv.giveItem(*player, Inventory::ItemID::CARROT_SWORD_INV, 1);
	}

	smelly->setCharName("Soroor");
	smelly->setSprite(AEGfxTextureLoad("Assets/Images/Entities/Soroor_Stationary.png"));

}


void Xuan_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* smelly = EntityManager::getNPC("smelly");
	auto* dummy = EntityManager::getEnemy("dummy");

	activeTile2 = World::activeTile(player->getX(), player->getY(), grid);

	/*if (AEInputCheckTriggered(AEVK_E))
	{
		World::interactTile(activeTile2, grid);
	}*/


	// Update the player
	player->update(grid);
	player->tickAttackTimer();

	smellind.x = smelly->getX();
	smellind.y = smelly->getY();

	dummind.x = dummy->getX();
	dummind.y = dummy->getY();

	updateIndicator(smellind);
	updateIndicator(dummind);



	// ========================= Conversations ========================

	if (TutorialScreen::dialogueBox.getIsActive() && TutorialScreen::activeSpeaker) {

		if (smelly->getIsIdling()) {
			TutorialScreen::activeSpeaker->idleSpeak(TutorialScreen::dialogueBox);
		}
		else {
			TutorialScreen::activeSpeaker->speak(TutorialScreen::dialogueBox);
		}


		return;
	}


	switch (TutorialScreen::state) {

	case TutorialScreen::TutorialState::SMELLY_TALK:

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			smelly->getX(), smelly->getY(),
			75.0f,
			smelly->getWidth(), smelly->getHeight()
		)) {
			TutorialScreen::activeSpeaker = smelly;
			smelly->speak(TutorialScreen::dialogueBox);
		}

		if (smelly->getIsPaused()) {
			TutorialScreen::state = TutorialScreen::TutorialState::SMELLY_IDLE;
		}

		break;



	case TutorialScreen::TutorialState::SMELLY_IDLE:

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			smelly->getX(), smelly->getY(),
			75.0f,
			smelly->getWidth(), smelly->getHeight()
		)) {
			TutorialScreen::activeSpeaker = smelly;
			smelly->idleSpeak(TutorialScreen::dialogueBox);
		}


		// attack
		if (!(TutorialScreen::dialogueBox.getIsActive()) && dummy->isAlive()) {

			if (AEInputCheckTriggered(AEVK_LBUTTON) && player->canAttack()) {
				player->attack(*dummy);
				player->resetAttackTimer();

			}

			if (dummy->getHp() <= 0) {
				dummy->setHp(0);
				TutorialScreen::deadDummy = true;
				TutorialScreen::state = TutorialScreen::TutorialState::SMELLY_PART2;
			}
		}

		break;



	case TutorialScreen::TutorialState::SMELLY_PART2:

		if (TutorialScreen::deadDummy && AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			smelly->getX(), smelly->getY(),
			75.0f,
			smelly->getWidth(), smelly->getHeight()
		)) {
			TutorialScreen::activeSpeaker = smelly;
			smelly->resumeDialogue(TutorialScreen::dialogueBox);
			TutorialScreen::deadDummy = false; // it gets revived for my code logic ig??
			return;
		}

		if (!TutorialScreen::deadDummy && !TutorialScreen::dialogueBox.getIsActive()) {
			TutorialScreen::state = TutorialScreen::TutorialState::FINISHED;
		}

		break;


	
	case TutorialScreen::TutorialState::FINISHED:
		grid.replacingID(World::Teleporter1, World::TeleporterBlue);
		World::standOnTile(next, *player, grid, GS_TUTDUN, World::TeleporterBlue);
		// if interact with teleporter set next game state to tutorialdungeon.cpp
		break;
	}


	// to port over, will have if doornotlocked condition too
	if (player->getX() < -(AEGfxGetWindowWidth() / 2)) {
		next = GS_FARM;
	}

	if (AEInputCheckTriggered(AEVK_F10))
	{
		World::dungeonTracker[World::checkNum] = true;
	}

	if (AEInputCheckTriggered(AEVK_F9))
	{
		inv.giveItem(*player, Inventory::ItemID::CARROT_SEEDS, 3);
	}


	inv.update(player);
}


void Xuan_Draw() {

	auto* player = EntityManager::getPlayer("player");
	auto* dummy = EntityManager::getEnemy("dummy");
	auto* smelly = EntityManager::getNPC("smelly");

	//Color::background(Color::Preset::White);
	//Color::fill(Color::Preset::Blue);

	/*TileManager::draw();*/

	grid.drawTexture(grid);
	Graphics::image(0, 0, AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), respawnDungeon, Shapes::CENTER);
	World::drawTile(activeTile2, grid);

	EntityManager::draw("smelly");
	
	if (showInventory)
	{
		inv.draw();
	}

	if (!dummy->dead()) {
		dummy->draw(*player, grid, false);
	}
	else {
		// placaeholder here for now but hopefully switch to sprite of destroyed dummy
		/*dummy->draw(*player, grid, false);*/
	}


	if (!TutorialScreen::dialogueBox.getIsActive()) {

		switch (TutorialScreen::state) {

			case TutorialScreen::TutorialState::SMELLY_TALK:
				if (smellind.active == 1) {
					Animations::drawIndicator(smellind);
				}
				break;


			case TutorialScreen::TutorialState::SMELLY_IDLE:
				Animations::drawIndicator(dummind);
				break;

			case TutorialScreen::TutorialState::SMELLY_PART2:
				if (smellind.active == 1) {
					Animations::drawIndicator(smellind);
				}
				break;

			case TutorialScreen::TutorialState::FINISHED:
				World::drawIndicatorsOnTileType(grid, World::TeleporterBlue, smellind);
				break;

		}

	}

	EntityManager::draw("player");

	// Draw dialogue box on top of everything
	TutorialScreen::dialogueBox.draw();
}

void Xuan_Free() {
	
	auto* player = EntityManager::getPlayer("player");
	/*grid.unloadMapTexture();
	World::freeGrid();*/

	inv.saveInventory(player, gameData);
	inv.setPlayer(nullptr);

}

void Xuan_Unload() {
	auto* smelly = EntityManager::getNPC("smelly");
	if (smelly && smelly->getSprite()) {
		AEGfxTextureUnload(smelly->getSprite()); // unload sprite
	}

	TutorialScreen::activeSpeaker = nullptr;
	EntityManager::clear();

	AEGfxTextureUnload(respawnDungeon);
	respawnDungeon = nullptr;

	//TileManager::exit();
} 


