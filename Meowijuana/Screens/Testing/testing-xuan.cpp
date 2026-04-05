/**
 * @file	 testing-xuan.cpp
 * @author	 Yu Xuan
 * @brief	 cpp for the dungoen tutoria (the one with the npc)
 */


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
AEGfxTexture* tutorialDungeon = nullptr;
AEGfxTexture* indicatorCool = nullptr;

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
		SORAWR_TALK,
		SORAWR_IDLE,
		SORAWR_PART2,
		FINISHED

	};

	TutorialState state = TutorialState::SORAWR_TALK;
	bool deadDummy = false; // oh no :(


	float noWeaponTimer = 0.0f;
	bool showNoWeaponText = false;

}

void Xuan_Load() {

	SpriteManager::init();

	grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initTextureBox();

	tutorialDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/Respawn.png");
	grid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Respawn.txt");
	indicatorCool = AEGfxTextureLoad("Assets/Indicators/SpeechBubble.png");

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

	UNREFERENCED_PARAMETER(dummy);

	auto* SORAWR = EntityManager::getNPC("soroor");
	
	// Restart everything after clearing the game
	if (World::restartLevels[1])
	{
		SORAWR->restartDialogue();
		SORAWR->restartIdle();
		TutorialScreen::state = TutorialScreen::TutorialState::SORAWR_TALK;
		World::restartLevels[1] = true;
		hasCarrotSword = false;
	}

	SORAWR->setDialogLines({
		"Welcome to the dungeons! There's 3 levels to a dungeon,\neach with increasing levels of difficulty.",
		"We're on the first floor, so it's relatively safe for\nnow.",
		"You'll need to learn how to defend yourself. Here! I've taken\nthe liberty to pass you a [CARROT SWORD].",
		"I'll walk you through this floor, and then you'll be\non your own for the next few.",
		"First off, see that [DUMMY] over there?\nIt's supposed to represent enemies.",
		"This one here won't fight back, but if you\nencounter real ones in the dungeon,\nthey'll attack on sight.",
		"Equip your weapon and use your [LEFT MOUSE BUTTON]\nto attack! Go give it a try.",

		"@",

		"Nice! To gain access to the next floor,\njust head to that teleporter over there\nand clear out the hallways to continue.",
		"If you ever faint, you'll get sent straight back here.",
		"Don't worry. I'll cart you out if something bad happens.\nSee you around!",

		"@"
	});

	SORAWR->setIdleLines({
		"Go on! Don't feel bad!",
		"The dummy can't grow arms and kill itself y'know...",

		"#",

		"If it makes you feel any better- or, er- less guilty,\nYou'll need to get rid of enemies while you traverse\nthe dungeons anyway.",
		"Might as well give it a try?"
	});


	// Initialize dialogue box
	TutorialScreen::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	auto* player = EntityManager::getPlayer("player");
	player->setPosition(-400.0f, 50.0f);

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(player, gameData);

	Inventory::Item* item = Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD);

	Inventory::Weapon* weapon = dynamic_cast<Inventory::Weapon*>(item);

	if (weapon)
	{
		player->setWeapon(weapon);
		player->setAtkSpd(weapon->getAttackSpeed());
		item = Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD);
		weapon = dynamic_cast<Inventory::Weapon*>(item);
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

	SORAWR->setCharName("Soroor");
	/*SORAWR->setSprite(AEGfxTextureLoad("Assets/Images/Entities/Soroor_Stationary.png"));*/

}


void Xuan_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* SORAWR = EntityManager::getNPC("soroor");
	auto* dummy = EntityManager::getEnemy("dummy");

	activeTile2 = World::activeTile(player->getX(), player->getY(), grid);

	/*if (AEInputCheckTriggered(AEVK_E))
	{
		World::interactTile(activeTile2, grid);
	}*/


	// Update the player
	player->update(grid);
	player->tickAttackTimer();

	smellind.x = SORAWR->getX();
	smellind.y = SORAWR->getY();

	dummind.x = dummy->getX();
	dummind.y = dummy->getY();

	updateIndicator(smellind);
	updateIndicator(dummind);



	// visual reminder

	if (TutorialScreen::showNoWeaponText) {
		TutorialScreen::noWeaponTimer -= static_cast<float>(AEFrameRateControllerGetFrameTime());

		if (TutorialScreen::noWeaponTimer <= 0.0f) {
			TutorialScreen::showNoWeaponText = false;
		}
	}



	// ========================= Conversations ========================

	if (TutorialScreen::dialogueBox.getIsActive() && TutorialScreen::activeSpeaker) {

		if (SORAWR->getIsIdling()) {
			TutorialScreen::activeSpeaker->idleSpeak(TutorialScreen::dialogueBox);
		}
		else {
			TutorialScreen::activeSpeaker->speak(TutorialScreen::dialogueBox);
		}


		return;
	}


	switch (TutorialScreen::state) {

	case TutorialScreen::TutorialState::SORAWR_TALK:

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			SORAWR->getX(), SORAWR->getY(),
			75.0f,
			SORAWR->getWidth(), SORAWR->getHeight()
		)) {
			TutorialScreen::activeSpeaker = SORAWR;
			SORAWR->speak(TutorialScreen::dialogueBox);
		}

		if (SORAWR->getIsPaused()) {
			TutorialScreen::state = TutorialScreen::TutorialState::SORAWR_IDLE;
		}

		break;



	case TutorialScreen::TutorialState::SORAWR_IDLE:

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			SORAWR->getX(), SORAWR->getY(),
			75.0f,
			SORAWR->getWidth(), SORAWR->getHeight()
		)) {
			TutorialScreen::activeSpeaker = SORAWR;
			SORAWR->idleSpeak(TutorialScreen::dialogueBox);
		}


		// attack
		if (!(TutorialScreen::dialogueBox.getIsActive()) && dummy->isAlive()) {


			if (AEInputCheckTriggered(AEVK_LBUTTON) && !player->isHoldingWeapon()) {
				TutorialScreen::showNoWeaponText = true;
				TutorialScreen::noWeaponTimer = 2.0f;
			}


			else if (AEInputCheckTriggered(AEVK_LBUTTON) && player->canAttack()) {
				player->attack(*dummy);
				player->resetAttackTimer();

			}


			if (dummy->getHp() <= 0) {
				dummy->setHp(0);
				TutorialScreen::deadDummy = true;
				TutorialScreen::state = TutorialScreen::TutorialState::SORAWR_PART2;
			}
		}

		break;



	case TutorialScreen::TutorialState::SORAWR_PART2:

		if (TutorialScreen::deadDummy) {
			TutorialScreen::activeSpeaker = SORAWR;
			SORAWR->resumeDialogue(TutorialScreen::dialogueBox);
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

	// Clear all dungeons
	if (AEInputCheckTriggered(AEVK_F10))
	{
		World::dungeonTracker[World::checkNum] = true;
	}

	inv.update(player);
}


void Xuan_Draw() {

	auto* player = EntityManager::getPlayer("player");
	auto* dummy = EntityManager::getEnemy("dummy");
	auto* SORAWR = EntityManager::getNPC("soroor");

	//Color::background(Color::Preset::White);
	//Color::fill(Color::Preset::Blue);

	/*TileManager::draw();*/

	Graphics::image(0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), tutorialDungeon, Shapes::CENTER);
	grid.drawTexture(grid);

	EntityManager::draw("soroor");
	
	if (showInventory)
	{
		inv.draw();
	}

	if (dummy->getHp() > 0) {
		dummy->draw(*player, grid, false);
	}
	//else {
	//	// placaeholder here for now but hopefully switch to sprite of destroyed dummy
	//	/*dummy->draw(*player, grid, false);*/
	//}


	if (!TutorialScreen::dialogueBox.getIsActive()) {

		switch (TutorialScreen::state) {

			case TutorialScreen::TutorialState::SORAWR_TALK:
				if (smellind.active == 1) {
					Animations::drawCoolerIndicator(SORAWR->getX(), SORAWR->getY(), indicatorCool);
				}
				break;


			case TutorialScreen::TutorialState::SORAWR_IDLE:
				Animations::drawIndicator(dummind);
				break;

			case TutorialScreen::TutorialState::SORAWR_PART2:
				if (smellind.active == 1) {
					Animations::drawCoolerIndicator(SORAWR->getX(), SORAWR->getY(), indicatorCool);
				}
				break;

			case TutorialScreen::TutorialState::FINISHED:
				World::drawIndicatorsOnTileType(grid, World::TeleporterBlue, smellind);
				break;

		}

	}

	EntityManager::draw("player");


	if (TutorialScreen::showNoWeaponText) {
		Text::text("Equip your weapon first!", -120, -250);
	}


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
	auto* SORAWR = EntityManager::getNPC("SORAWR");
	if (SORAWR && SORAWR->getSprite()) {
		AEGfxTextureUnload(SORAWR->getSprite()); // unload sprite
	}

	TutorialScreen::activeSpeaker = nullptr;
	EntityManager::clear();

	AEGfxTextureUnload(tutorialDungeon);
	tutorialDungeon = nullptr;

	//TileManager::exit();
} 


