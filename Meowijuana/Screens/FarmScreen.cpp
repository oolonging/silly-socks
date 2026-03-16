#include "../pch.h"
#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "FarmScreen.hpp"
#include "../Managers/EntityManager.hpp"
#include "../../World.hpp"

bool firstLoad = true;
bool firstDungeon = true;

UI_Elements::PlayerInventory inv;

// For world grid
std::pair<int, int> prevActiveT;
std::pair<int, int> activeT;

World::worldGrid Grid;
//bool turnGridOn = false;

namespace FarmNPC {

	Entity::NPC* activeSpeaker = nullptr;
	UI_Elements::DialogueBox dialogueBox;

	enum class TutorialState {
		GER_TALK,
		GER_IDLE,
		FINISHED

	};

	TutorialState state = TutorialState::GER_TALK;

}

void Farm_Load() 
{
	Inventory::load();
}


void Farm_Initialize() {

	// Initialising stuffs
	EntityManager::init();
	Inventory::init();

	// Initialising Player
	auto* player = EntityManager::getPlayer("player");
	player->setPosition(-800.0f, 50.0f);
	player->setSpeed(10.f);

	// Setting position for inventory
	float screenWidth = AEGfxGetWindowWidth();
	float screenHeight = AEGfxGetWindowHeight();
	float invWidth = inv.getSlotSize() * 9 + player->getInventorySize() * 8;
	float offset = 10.0f;

	float x = -invWidth / 2.0f;         // centered horizontally
	float y = -(screenHeight / 2.0f) + inv.getSlotSize() + offset; // near the bottom 

	// Setting inventory to bottom 
	inv.setPosition(x, y);
	inv.setPlayer(player);

	// Set player position
	player->setX(0);
	player->setY(0);

	// Temporary using the same layout as my temp farm stuff
	Grid.fillGrid("../../Assets/LevelMaps/Farm_layout.txt");

	// Initialising NPCs
	auto* Gerald = EntityManager::create<Entity::NPC>("Gerald", -50.0f, 0.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);

	Gerald->setSprite(AEGfxTextureLoad("Assets/Images/Entities/Gerald_Stationary.png")); 

	Gerald->setDialogLines({
		"Insert Dialogue Here",

		"@",

		"Next Dialogue Here",

		"@"
		});

	Gerald->setIdleLines({
		"Go on! Don't feel bad!",
		});

	// Initialize dialogue box
	FarmNPC::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	// Initialise Grid Stuff
	Grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	Grid.initMapTexture();
	Grid.initTextureBox();
	Grid.fillGrid("../../Assets/LevelMaps/Farm_layout.txt");
	Grid.outWorldMap("../../Assets/LevelMaps/Check.txt");

}

void Farm_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* Gerald = EntityManager::getNPC("Gerald");

	// Rendering state
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);

	// Player update

	player->update();
	inv.update();

	activeT = World::activeTile(player->getX(), player->getY(), Grid);

	/*if (AEInputCheckTriggered(AEVK_F9))
	{
		turnGridOn = !turnGridOn;
	}*/

	if (AEInputCheckTriggered(AEVK_E))
	{
		World::interactTile(activeT, Grid, inv, *player);
	}

	// Will change this to after the player talks to Gerald
	if (AEInputCheckTriggered(AEVK_F1))
	{
		inv.giveCarrotSeeds(*player);
	}

	World::standOnTile(next, *player, Grid, GS_DUNGEON);

	World::collidableNearby(*player, Grid);

	// NPC TALKING STUFFS

	if (FarmNPC::dialogueBox.getIsActive() && FarmNPC::activeSpeaker) {

		if (Gerald->getIsIdling()) {
			FarmNPC::activeSpeaker->idleSpeak(FarmNPC::dialogueBox);
		}
		else {
			FarmNPC::activeSpeaker->speak(FarmNPC::dialogueBox);
		}


		return;
	}

	switch (FarmNPC::state) {

	case FarmNPC::TutorialState::GER_TALK:

		FarmNPC::activeSpeaker = Gerald;
		Gerald->speak(FarmNPC::dialogueBox);

		if (Gerald->getIsPaused()) {
			FarmNPC::state = FarmNPC::TutorialState::GER_IDLE;
		}

		break;
	}

	// Press E at dungeon entrance 
	World::standOnTile(next, *player, Grid, GS_X);

}



void Farm_Draw() {

	auto* player = EntityManager::getPlayer("player");
	auto* Gerald = EntityManager::getNPC("Gerald");

	World::drawTile(activeT, Grid);
	Grid.drawTexture(Grid);

	/*if (turnGridOn)
	{
		World::drawGrid();
	}*/

	World::drawTile(activeT, Grid);
	EntityManager::draw("Gerald");

	player->draw();

	inv.draw();

	// Basically the stuff that draws the floating pointer on top of the npc
	/*if (!FarmNPC::dialogueBox.getIsActive()) {

		switch (FarmNPC::state) {

		case FarmNPC::TutorialState::GER_TALK:
			if (smellind.active == 1) {
				Animations::drawIndicator(smellind);
			}
			break;


		case FarmNPC::TutorialState::GER_IDLE:
			Animations::drawIndicator(dummind);
			break;

		case FarmNPC::TutorialState::FINISHED:
			break;

		}

	}*/


	// Draw dialogue box on top of everything
	FarmNPC::dialogueBox.draw();

}

void Farm_Free() 
{
	auto* player = EntityManager::getPlayer("player");
	auto* Gerald = EntityManager::getNPC("Gerald");

	Grid.unloadMapTexture();
	World::freeGrid();
	Inventory::ItemRegistry::cleanup();
	player->freeInventory();

}

void Farm_Unload() 
{
	Inventory::unload();

	auto* Gerald = EntityManager::getNPC("Gerald");
	if (Gerald && Gerald->getSprite()) {
		AEGfxTextureUnload(Gerald->getSprite()); // unload sprite
	}

	FarmNPC::activeSpeaker = nullptr;
	EntityManager::clear();

}


