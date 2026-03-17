#include "../pch.h"
#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "FarmScreen.hpp"
#include "../Managers/EntityManager.hpp"
#include "../../World.hpp"

bool firstLoad = true;
bool firstDungeon = true;

extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;

// For world grid
std::pair<int, int> prevActiveT;
std::pair<int, int> activeT;

extern World::worldGrid grid;
bool onGrid = false;

bool firstStartGame = true;

float lastposX = 0;
float lastposY = 0;

namespace FarmNPC {

	Entity::NPC* activeSpeaker = nullptr;
	UI_Elements::DialogueBox dialogueBox;

	enum class TutorialState {
		GER_TALK,
		GER_IDLE,
		GER_TALK2,
		FINISHED
	};

	TutorialState state = TutorialState::GER_TALK;

}

void Farm_Load() 
{
	showInventory = true;
}


void Farm_Initialize() {

	// Initialising stuffs
	EntityManager::init();

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
	inv.setPlayer(EntityManager::getPlayer("player"));
	
	inv.loadInventory(player, gameData);

	// Set player position
	if (firstStartGame)
	{
		player->setX(0);
		player->setY(0);
	}
	else
	{
		player->setX(lastposX);
		player->setY(lastposY);
	}

	// Initialising NPCs
	auto* Gerald = EntityManager::create<Entity::NPC>("Gerald", -50.0f, 0.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);

	Gerald->setSprite(AEGfxTextureLoad("Assets/Images/Entities/Gerald_Stationary.png")); 

	Gerald->setDialogLines({
		"Welcome to Catastrofarm! I heard you are new here so here are some seeds to get you started on your journey! Press E when hovering over a empty crop spot.",

		"@",

		"Good Job! After planting the seeds, it will grow everytime you finish a dungeon level so make sure you make full use of your farm space!",
		"You can get more seeds while exploring the dungeons so look out for that!",

		"@"
		});

	Gerald->setIdleLines({
		"Hey you can't leave without planting those! Try Planting the seeds that I have just given you!"
		});

	// Initialize dialogue box
	FarmNPC::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	// Initialise Grid Stuff
	grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	grid.initMapTexture();
	grid.initTextureBox();
	
	if (firstStartGame)
	{
		grid.fillGrid("../../Assets/LevelMaps/Farm_layout.txt");
	}

	else
	{
		std::ifstream file("../../Assets/LevelMaps/Farm_User_layout.txt");

		if (!file.is_open())
		{
			grid.fillGrid("../../Assets/LevelMaps/Farm_layout.txt");
		}

		else
		{
			file.close();
			grid.fillGrid("../../Assets/LevelMaps/Farm_User_layout.txt");
		}
	}

	// Growing plant
	grid.growPlants(grid);

}

void Farm_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* Gerald = EntityManager::getNPC("Gerald");

	// Player update
	player->update(grid);
	inv.update();

	activeT = World::activeTile(player->getX(), player->getY(), grid);

	// Keybindings for tests
	if (AEInputCheckTriggered(AEVK_F9))
	{
		onGrid = !onGrid;
	}

	if (AEInputCheckTriggered(AEVK_F10))
	{
		World::dungeonTracker[World::checkNum] = true;
		grid.growPlants(grid);
	}

	if (AEInputCheckTriggered(AEVK_E))
	{
		World::interactTile(activeT, grid, inv, *player);
	}

	// Makes it so that user is stuck in tutorial area until done w tutorial, it player completed tutorial already, teleporter shld be normal
	if (firstStartGame)
	{
		if (!inv.findItem(*player,Inventory::ItemID::CARROT_SEEDS) && !inv.findItem(*player, Inventory::ItemID::CHERRY_SEEDS))
		{
			World::standOnTile(next, *player, grid, GS_X);
		}
		else
		{
			std::pair<int, int> currTile = grid.getIndex(player->getX(), player->getY());
			if (grid.getTileID(currTile.first, currTile.second) == World::Teleporter)
			{
				FarmNPC::activeSpeaker = Gerald;
				Gerald->idleSpeak(FarmNPC::dialogueBox);

			}
			else
			{
				World::standOnTile(next, *player, grid, GS_X); // handle non-teleporter tiles
			}
		}
	}

	else
	{
		World::standOnTile(next, *player, grid, GS_X);
	}

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
			inv.giveItem(*player, Inventory::ItemID::CARROT_SEEDS, 3);
			inv.giveItem(*player, Inventory::ItemID::CHERRY_SEEDS, 6);
		}

		break;


	case FarmNPC::TutorialState::GER_IDLE:

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			Gerald->getX(), Gerald->getY(),
			75.0f,
			Gerald->getWidth(), Gerald->getHeight()))
		{
			FarmNPC::activeSpeaker = Gerald;
			Gerald->idleSpeak(FarmNPC::dialogueBox);
		}

		break;

	// Check if all seeds are planted then next step in dialogue
	case FarmNPC::TutorialState::GER_TALK2:
			
		FarmNPC::activeSpeaker = Gerald;
		Gerald->resumeDialogue(FarmNPC::dialogueBox);
				
		if (Gerald->getIsPaused()) {
			FarmNPC::state = FarmNPC::TutorialState::FINISHED;
		}

		break;
	}

	if (inv.isEmpty(*player) && FarmNPC::state == FarmNPC::TutorialState::GER_IDLE)
	{
		FarmNPC::state = FarmNPC::TutorialState::GER_TALK2;
	}

}


void Farm_Draw() {

	// Rendering state
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);

	auto* player = EntityManager::getPlayer("player");
	auto* Gerald = EntityManager::getNPC("Gerald");

	grid.drawTexture(grid);

	if (onGrid)
	{
		World::drawGrid();
	}

	World::drawTile(activeT, grid);
	EntityManager::draw("Gerald");

	player->draw();

	if (showInventory)
	{
		inv.draw();
	}	

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
	grid.outWorldMap("../../Assets/LevelMaps/Farm_User_layout.txt");

	auto* player = EntityManager::getPlayer("player");
	auto* Gerald = EntityManager::getNPC("Gerald");

	lastposX = player->getX();
	lastposY = player->getY();

	std::pair<int, int> currTile = grid.getIndex(lastposX, lastposY);

	if (grid.getTileID(currTile.first, currTile.second) == World::Teleporter)
	{
		currTile.second -= 1;
		std::pair<float, float> newCords = World::getWorldCoords(currTile, grid);
		lastposX = newCords.first;
		lastposY = newCords.second;
	}

	/*grid.unloadMapTexture();
	Inventory::ItemRegistry::cleanup();*/

	firstStartGame = false;

	inv.saveInventory(player, gameData);
	inv.setPlayer(nullptr);

}

void Farm_Unload() 
{
	/*Inventory::unload();*/

	auto* Gerald = EntityManager::getNPC("Gerald");
	if (Gerald && Gerald->getSprite()) {
		AEGfxTextureUnload(Gerald->getSprite()); // unload sprite
	}

	FarmNPC::activeSpeaker = nullptr;
	EntityManager::clear();

}


