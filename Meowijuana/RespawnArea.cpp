#include "../../pch.h"
#include "RespawnArea.hpp"

#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../World.hpp"
#include "../../Settings.hpp"
#include "../../Managers/SpriteManager.hpp"

// idle lines are broken rn but i'll have to fix after playtest


// Externs for Inventory
extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;
AEGfxTexture* indicatorRespawn = nullptr;


extern World::worldGrid grid;
std::pair<int, int> prevActiveRespawnTile;
AEGfxTexture* respawnDungeon = nullptr;
static bool hasCarrotSword = false;


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

	SpriteManager::init();

	grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initTextureBox();

	respawnDungeon = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/Respawn.png");
	grid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Respawn.txt");
	indicatorRespawn = AEGfxTextureLoad("Assets/Indicators/SpeechBubble.png");


}


void RespawnArea_Initialize() {
	Settings::currentScreen = "RespawnArea.cpp";

	EntityManager::init();

	auto* soroor = EntityManager::getNPC("soroor");
	soroor->setPosition(200, 300);
	soroor->setCharName("Soroor");
	soroor->setSprite(AEGfxTextureLoad("Assets/Images/Entities/Soroor_Stationary.png"));


	soroor->setDialogLines({
		"Hey you. You're finally awake. You were trying to clear the dungeon, right? Walked right into those enemies. You're lucky you got found on time.",
		"Well, it's good that you're safe. Told you i'd drag you back to camp!",
		"Be more careful next time. I'll keep an eye out for you still, but i'm afraid you'll be severely damaged after every encounter. Three will be your limit.",
		"Rest up and gather yourself before you go on your way. You've got a long journey ahead.",

		"@"
		});


	// check if this works btw
	RespawnArea::deathcount += 1;

	// TODO: remember to set idlenum to 0 or smth? 
	// not sure yet lemme check, i may need checkpointing system before this tho
	switch (RespawnArea::deathcount) {

	case 1:
		soroor->setIdleLines({
			"Use your cherries efficiently to stay alive!",

			"#",

			"Did you know you can't access teleporters before rooms are fully cleared?",

			"#",

			"Doors get locked whenever you enter active combat in the dungeons. Such is the way of life...",

			"#",

			"Just head to the exit whenever you're ready!",

			"#",

			"Remember to harvest your crops after every dungeon level!",
			});

		break;

	case 2:
		soroor->setIdleLines({
			"Fancy seeing you again! You come here often?",

			"#",

			"I know we as a species joke about having 9 lives, but maybe don't be the first to test that theory out.",
			});
	}

	// Initialize dialogue box
	RespawnArea::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	auto* player = EntityManager::getPlayer("player");
	player->setPosition(-400.0f, 50.0f);

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(player, gameData);

	RespawnArea::state = RespawnArea::SpeakState::SMELLY_CONDOLENCES;
	RespawnArea::activeSpeaker = nullptr;

}

void RespawnArea_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* soroor = EntityManager::getNPC("soroor");

	player->update(grid);
	inv.update(player);
	
	if (RespawnArea::dialogueBox.getIsActive() && RespawnArea::activeSpeaker) {

		if (soroor->getIsIdling()) {
			RespawnArea::activeSpeaker->idleSpeak(RespawnArea::dialogueBox);
		}
		else {
			RespawnArea::activeSpeaker->speak(RespawnArea::dialogueBox);
		}

		return;
	}


	switch (RespawnArea::state) {

	case RespawnArea::SpeakState::SMELLY_CONDOLENCES:

		RespawnArea::activeSpeaker = soroor;
		soroor->speak(RespawnArea::dialogueBox);

		if (soroor->getIsPaused()) {
			RespawnArea::state = RespawnArea::SpeakState::SMELLY_CONIDLENCES;
		}
		break;



	case RespawnArea::SpeakState::SMELLY_CONIDLENCES:


		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			soroor->getX(), soroor->getY(),
			75.0f,
			soroor->getWidth(), soroor->getHeight()
		)) {
			RespawnArea::activeSpeaker = soroor;
			soroor->setIdling(true);
			soroor->idleSpeak(RespawnArea::dialogueBox);
		}

		grid.replacingID(World::Teleporter1, World::TeleporterBlue);
		World::standOnTile(next, *player, grid, GS_FARM, World::TeleporterBlue);

		break;

	}

}

void RespawnArea_Draw() {

	auto* player = EntityManager::getPlayer("player");
	auto* soroor = EntityManager::getNPC("soroor");

	Graphics::image(0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), respawnDungeon, Shapes::CENTER);
	grid.drawTexture(grid);


	if (showInventory)
	{
		inv.draw();
	}

	EntityManager::draw("soroor");
	EntityManager::draw("player");

	RespawnArea::dialogueBox.draw();


	Animations::drawCoolerIndicator(soroor->getX(), soroor->getY(), indicatorRespawn);

}


void RespawnArea_Free() {

	auto* localPlayer = EntityManager::getPlayer("player");
	inv.saveInventory(localPlayer, gameData);
	inv.setPlayer(nullptr);

	EntityManager::clear();
}


void RespawnArea_Unload() {

	AEGfxTextureUnload(indicatorRespawn);
	indicatorRespawn = nullptr;


	auto* soroor = EntityManager::getNPC("soroor");
	if (soroor && soroor->getSprite()) {
		AEGfxTextureUnload(soroor->getSprite());
	}

	AEGfxTextureUnload(respawnDungeon);
	respawnDungeon = nullptr;

}