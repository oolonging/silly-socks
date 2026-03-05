#include "AEEngine.h"
#include "testing-xuan.hpp"
#include <iostream>
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../Managers/TileManager.hpp"


namespace TutorialScreen {

	Entity::NPC* activeSpeaker = nullptr;
	UI_Elements::DialogueBox dialogueBox;

}

void Xuan_Load() {


}

void Xuan_Initialize() {

	TileManager::init();

	// Load tile layout from file AFTER init
	TileManager::loadTileLayout("Assets/Maps/testing.txt");

	// Initialize entities
	EntityManager::init();

	auto* smelly = EntityManager::create<Entity::NPC>("smelly", -500.0f, 50.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
	auto* dummy = EntityManager::create<Entity::Enemy>("dummy", -100.0f, 50.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);

	smelly->setSprite(AEGfxTextureLoad("Assets/Images/Entities/smelly.png")); // plus 1 poorly scaled placeholder becauase i dont want my lecturers staring at me while i work


	smelly->setDialogLines({
		"Hey [PLAYER]! Welcome to the dungeons! There's 4 levels to each dungeon, each with increasing levels of difficulty. We're on the first floor, so it's relatively safe for now.",
		"You'll need to learn how to defend yourself. Here! I've taken the liberty to pass you a [CARROT SWORD]. I'll walk you through this floor, and then you'll be on your own for the next few.",
		"First off, see that [DUMMY] over there? It's supposed to represent enemies.\n This one here won't fight back, but if you encounter real ones in the dungeon, they'll attack on sight.",
		"Use your [LEFT MOUSE BUTTON] to attack! Go give it a try.",

		"@",

		"Nice! OK, seems like my job here is done. I'm gonna go clock off for the day. You have fun though!",
		"Oh- The next floor? Right, sorry. Just head to that teleporter over there to continue.",

		"@"
	});

	smelly->setIdleLines({
		"Go on! Don't feel bad! The dummy won't kill itself y'know... ",
		"If it makes you feel any better- or, er- less guilty, You'll need to get rid of enemies while you traverse the dungeons anyway. Might as well give it a try?"
	});


	// Initialize dialogue box
	TutorialScreen::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	auto* player = EntityManager::getPlayer("player");
	player->setPosition(-600.0f, 50.0f);


	Inventory::init();
	Inventory::Item* item = Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD);

	Inventory::Weapon* weapon = dynamic_cast<Inventory::Weapon*>(item);

	if (weapon)
	{
		player->setWeapon(weapon);
		player->setAtkSpd(weapon->getAttackSpeed());
	}

}


void Xuan_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* smelly = EntityManager::getNPC("smelly");
	auto* dummy = EntityManager::getEnemy("dummy");

	// Update the player
	player->update();
	player->tickAttackTimer();

	static Animations::Indicator smellind{ smelly->getX(), smelly->getY() };
	static Animations::Indicator dummind{ dummy->getX(), dummy->getY() };

	updateIndicator(smellind);
	updateIndicator(dummind);



	// ========================= Conversations ========================


	if (TutorialScreen::dialogueBox.getIsActive() && TutorialScreen::activeSpeaker) {
		TutorialScreen::activeSpeaker->speak(TutorialScreen::dialogueBox);
	}


	// condition fulfilled? resume
	if (smelly->getIsPaused() && smelly->getConditionTrue() && !TutorialScreen::dialogueBox.getIsActive()) {
		Animations::drawIndicator(smellind);
		smelly->resumeDialogue(TutorialScreen::dialogueBox);
		TutorialScreen::activeSpeaker = smelly;
	}


	// convo
	if (!TutorialScreen::dialogueBox.getIsActive() && !smelly->getIsPaused()) {
		if (smellind.active == 1) {
			Animations::drawIndicator(smellind);
		}
		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			smelly->getX(), smelly->getY(),
			75.0f,
			smelly->getWidth(), smelly->getHeight()
		)) {
			TutorialScreen::activeSpeaker = smelly;
			smelly->speak(TutorialScreen::dialogueBox);
			smellind.active = 0;
		}

	}


	// idle
	if (!TutorialScreen::dialogueBox.getIsActive() && smelly->getIsPaused() && !smelly->getConditionTrue()) {
		if (smellind.active == 1) {
			Animations::drawIndicator(smellind);
		}
		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			smelly->getX(), smelly->getY(),
			75.0f,
			smelly->getWidth(), smelly->getHeight()
		)) {
			TutorialScreen::activeSpeaker = smelly;
			smelly->speak(TutorialScreen::dialogueBox);
		}
	}




	// ========================= Fight Mechanic ==========================

	// dummy indicator & attack
	if (!(TutorialScreen::dialogueBox.getIsActive()) && smelly->getIsPaused() && dummy->isAlive()) {
		Animations::drawIndicator(dummind);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && player->canAttack()) {
			player->attack(*dummy);
			player->resetAttackTimer();

		}

		if (dummy->getHp() <= 0) {
			dummy->setHp(0);
			smelly->setConditionTrue();
			//smelly->resumeDialogue(TutorialScreen::dialogueBox);
			//TutorialScreen::activeSpeaker = smelly;
		}
	}
}

void Xuan_Draw() {

	auto* player = EntityManager::getPlayer("player");
	auto* dummy = EntityManager::getEnemy("dummy");

	//Color::background(Color::Preset::White);
	//Color::fill(Color::Preset::Blue);

	// TileManager::draw();

	EntityManager::draw("smelly");
	EntityManager::draw("player");
	

	if (dummy->getHp() > 0) {
		dummy->draw(*player);
	}
	else {
		// placaeholder here for now but hopefully switch to sprite of destroyed dummy
		dummy->draw(*player);
	}

	// Draw dialogue box if active
	TutorialScreen::dialogueBox.draw();
}

void Xuan_Free() {}

void Xuan_Unload() {
	EntityManager::clear();
	TileManager::exit();
}
