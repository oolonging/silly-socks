#include "../../pch.h"
#include "testing-xuan.hpp"

#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../World.hpp"


extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;

// will update with new dialogue once tiletypes and consumables come into play

World::worldGrid Griddtwo;
std::pair<int, int> prevActiveTile2;
std::pair<int, int> activeTile2;
static Animations::Indicator smellind;
static Animations::Indicator dummind;


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


}

void Xuan_Initialize() {

	//TileManager::init();

	Griddtwo.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	Griddtwo.initMapTexture();
	Griddtwo.initTextureBox();
	Griddtwo.fillGrid("../../Assets/LevelMaps/DungeonNPCRoom.txt");
	Griddtwo.outWorldMap("../../Assets/LevelMaps/Checkalso.txt");


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

		//inv.giveItem(*player, Inventory::ItemID::CARROT_SWORD, 1); 
	}

}


void Xuan_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* smelly = EntityManager::getNPC("smelly");
	auto* dummy = EntityManager::getEnemy("dummy");
	smelly->setCharName("Smelly");

	activeTile2 = World::activeTile(player->getX(), player->getY(), Griddtwo);

	/*if (AEInputCheckTriggered(AEVK_E))
	{
		World::interactTile(activeTile2, Griddtwo);
	}*/


	// Update the player
	player->update(Griddtwo);
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
		World::standOnTile(next, *player, Griddtwo, GS_TUTDUN);
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


	inv.update();
}


void Xuan_Draw() {

	auto* player = EntityManager::getPlayer("player");
	auto* dummy = EntityManager::getEnemy("dummy");
	auto* smelly = EntityManager::getNPC("smelly");

	//Color::background(Color::Preset::White);
	//Color::fill(Color::Preset::Blue);

	/*TileManager::draw();*/

	Griddtwo.drawTexture(Griddtwo);
	World::drawTile(activeTile2, Griddtwo);
	World::drawTile({ 0,0 }, Griddtwo);

	EntityManager::draw("smelly");

	
	if (showInventory)
	{
		inv.draw();
	}

	if (!dummy->dead()) {
		dummy->draw(*player, Griddtwo, false);
	}
	else {
		// placaeholder here for now but hopefully switch to sprite of destroyed dummy
		/*dummy->draw(*player, Griddtwo, false);*/
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
				break;

		}

	}

	EntityManager::draw("player");

	// Draw dialogue box on top of everything
	TutorialScreen::dialogueBox.draw();
}

void Xuan_Free() {
	
	auto* player = EntityManager::getPlayer("player");
	Griddtwo.unloadMapTexture();
	World::freeGrid();

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

	//TileManager::exit();
} 


