#include "../../pch.h"
#include "RespawnArea.hpp"

#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../World.hpp"
#include "../../Settings.hpp"

// idle lines are broken rn but i'll have to fix after playtest


// Externs for Inventory
extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;


World::worldGrid RespawnAreaGrid;
std::pair<int, int> prevActiveRespawnTile;
static Animations::Indicator smellind;


namespace RespawnArea{

	Entity::NPC* activeSpeaker = nullptr;
	UI_Elements::DialogueBox dialogueBox;
	int deathcount = 0;

	enum class SpeakState {
		SMELLY_CONDOLENCES,
		SMELLY_CONIDLENCES,
		FINISHED

	};

	SpeakState state = SpeakState::SMELLY_CONDOLENCES;

}


void RespawnArea_Load() {

}


void RespawnArea_Initialize() {
	Settings::currentScreen = "RespawnArea.cpp";

	RespawnAreaGrid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	RespawnAreaGrid.initMapTexture();
	RespawnAreaGrid.initTextureBox();
	RespawnAreaGrid.fillGrid("../../Assets/LevelMaps/DungeonNPCRoom.txt");

	EntityManager::init();

	auto* smelly = EntityManager::getNPC("smelly");
	smelly->setPosition(200, 300);
	smelly->setSprite(AEGfxTextureLoad("Assets/Images/Entities/smelly.png"));


	smelly->setDialogLines({
		"Hey you. You're finally awake. You were trying to clear the dungeon, right? Walked right into those enemies. You're lucky you got found on time.",
		"Well, it's good that you're safe. Told you i'd drag you back to camp!",
		"Be more careful next time. I'll keep an eye out for you still, but I've got other things to do y'know.",
		"Rest up and gather yourself before you go on your way. You've got a long journey ahead.",

		"@"
		});


	// check if this works btw
	RespawnArea::deathcount += 1;

	// TODO: remember to set idlenum to 0 or smth? 
	// not sure yet lemme check, i may need checkpointing system before this tho
	switch (RespawnArea::deathcount) {

	case 1:
		smelly->setIdleLines({
			"Use your bombs efficiently to defeat groups of enemies!",

			"#",

			"Did you know you can't access teleporters before rooms are fully cleared?",

			"#",

			"Doors get locked whenever you enter active combat in the dungeons. Such is the way of life...",

			"#",

			"Just head to the exit whenever you're ready!",

			"#",

			"Remember to harvest your crops after every dungeon level!",

			"#",

			"Boy do you have a lot of questions.",

			"#",

			"Not to be rude or anything but I have a wife and kids to go home to y'know",
			"(Not like you'd know, but still)",
			});

		break;

	case 2:
		smelly->setIdleLines({
			"Fancy seeing you again! You come here often?",

			"#",

			"I know we as a species joke about having 9 lives, but maybe don't be the first to test that theory out.",
			});
	}

	// Initialize dialogue box
	RespawnArea::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	auto* player = EntityManager::getPlayer("player");
	player->setPosition(-800.0f, 50.0f);

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(player, gameData);

	RespawnArea::state = RespawnArea::SpeakState::SMELLY_CONDOLENCES;
	RespawnArea::activeSpeaker = nullptr;

}

void RespawnArea_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* smelly = EntityManager::getNPC("smelly");

	player->update(RespawnAreaGrid);
	inv.update(player);
	
	if (RespawnArea::dialogueBox.getIsActive() && RespawnArea::activeSpeaker) {

		if (smelly->getIsIdling()) {
			RespawnArea::activeSpeaker->idleSpeak(RespawnArea::dialogueBox);
		}
		else {
			RespawnArea::activeSpeaker->speak(RespawnArea::dialogueBox);
		}

		return;
	}


	switch (RespawnArea::state) {

	case RespawnArea::SpeakState::SMELLY_CONDOLENCES:

		RespawnArea::activeSpeaker = smelly;
		smelly->speak(RespawnArea::dialogueBox);

		if (smelly->getIsPaused()) {
			RespawnArea::state = RespawnArea::SpeakState::SMELLY_CONIDLENCES;
		}
		break;



	case RespawnArea::SpeakState::SMELLY_CONIDLENCES:

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			smelly->getX(), smelly->getY(),
			75.0f,
			smelly->getWidth(), smelly->getHeight()
		)) {
			RespawnArea::activeSpeaker = smelly;
			smelly->setIdling(true);
			smelly->idleSpeak(RespawnArea::dialogueBox);
		}

		RespawnAreaGrid.replacingID(World::Teleporter, World::ActivatedTeleporter);
		World::standOnTile(next, *player, RespawnAreaGrid, GS_TUTDUN);

		break;

	}

	smellind.x = smelly->getX();
	smellind.y = smelly->getY();
	updateIndicator(smellind);

	if (player->getX() < -(AEGfxGetWindowWidth() / 2)) {
		next = GS_FARM;
	}


}

void RespawnArea_Draw() {

	auto* player = EntityManager::getPlayer("player");
	auto* smelly = EntityManager::getNPC("smelly");

	RespawnAreaGrid.drawTexture(RespawnAreaGrid);
	World::drawTile(prevActiveRespawnTile, RespawnAreaGrid);
	World::drawTile({ 0,0 }, RespawnAreaGrid);

	if (showInventory)
	{
		inv.draw();
	}

	EntityManager::draw("smelly");
	EntityManager::draw("player");

	RespawnArea::dialogueBox.draw();
	Animations::drawIndicator(smellind);

}


void RespawnArea_Free() {

	auto* tutPlayer = EntityManager::getPlayer("player");
	inv.saveInventory(tutPlayer, gameData);
	inv.setPlayer(nullptr);

	RespawnAreaGrid.unloadMapTexture();
	World::freeGrid();

}

void RespawnArea_Unload() {


}