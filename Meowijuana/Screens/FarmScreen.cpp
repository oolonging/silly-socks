/*
 * @file FarmScreen.cpp
 * @author Andrea
 * @brief Main farm screen 
 */

#include "../pch.h"
#include "../GameStateManager.hpp"

#include "../Graphics.hpp"
#include "FarmScreen.hpp"
#include "../Managers/EntityManager.hpp"
#include "../../World.hpp"
#include "../Settings.hpp"
#include "../../Managers/UIManager.hpp"

bool firstLoad = true;
bool firstDungeon = true;

extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;
static Animations::Indicator dirt;

static Entity::Player* localPlayer = nullptr;
static Entity::NPC* localGerald = nullptr;

AEGfxTexture* bg = nullptr;
AEGfxTexture* indicator = nullptr;

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

//popup
static UI_Elements::PopupBox* cropPopup;
static UI_Elements::PopupBox* inventoryPopup;
static UI_Elements::PopupBox* harvestPopup;
static UI_Elements::PopupBox* dungeonPopup;

void Farm_Load()
{
	showInventory = true;

	grid.initMapTexture();
	grid.initTextureBox();

	bg = AEGfxTextureLoad("Assets/LevelMaps/NewDungeons/Backgrounds/Farm.png");
	indicator = AEGfxTextureLoad("Assets/Indicators/SpeechBubble.png");
}

void Farm_Initialize() 
{
	// Set the debug "Current Screen" property
	Settings::currentScreen = "FarmScreen.cpp";

	// init Entitymanager
	EntityManager::clear();
	EntityManager::init();

	// Initialising Player
	localPlayer = EntityManager::getPlayer("player");
	auto* player = localPlayer;

	// Initialising NPCs
	localGerald = EntityManager::getNPC("gerald");
	auto* gerald = localGerald;
	gerald->setCharName("Gerald");

	// check if all the dungeons are cleared, if so then naviate to the win screen
	if (World::dungeonTracker[0] && World::dungeonTracker[1] && World::dungeonTracker[2]) {
		next = GS_WIN;
	}

	// Restart everything after clearing the game 
	if (World::restartLevels[0])
	{
		gerald->restartDialogue();
		gerald->restartIdle();
		firstStartGame = true;
		FarmNPC::state = FarmNPC::TutorialState::GER_TALK;
		World::restartLevels[0] = false;
	}

	player->setPosition(-800.0f, 50.0f);
	player->setSpeed(10.f);

	FarmNPC::dialogueBox.setSpeaker("gerald");

	// Setting position for inventory 

	float screenHeight = static_cast<float>(AEGfxGetWindowHeight());

	float invWidth = inv.getSlotSize() * 9 + player->getInventorySize() * 8;
	float offset = 10.0f;

	float x = -invWidth * 0.5f;         // centered horizontally
	float y = -(screenHeight * 0.5f) + inv.getSlotSize() + offset; // near the bottom 


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

	gerald->setDialogLines({
		"Welcome to Catastrofarm! You must be the newly hired help\nto clear the dungeons.",
		"I assume you don't have any gear right now, but don't worry-\nWe'll suit you up.",
		"Against the enemies here, your best defense would be a\n[CARROT SWORD].",
		"The more carrots you have in your inventory, the more damage\nit deals.",
		"Cherries give you HP if you're ever low, so stock up on those\nwhen you have the time as well.",
		"I'll pass you some seeds to get you started on your\njourney.", 

		"@",
		
		"After planting, crops will grow every time you finish\na dungeon level.",
		"So, be sure to plant as many as you can while dungeoning\nso that you can get the most out of your time.",
		"Defeated enemies have a chance to drop seeds,\nso you'll always have a steady supply. Good\nluck!", 
		"Head over to the newly opened teleporter on the bottom right\nwhenever you're ready.", 
		
		"@"
		});

	if (!World::dungeonTracker[0])
		gerald->setIdleLines({
			"You still have seeds on you. Try\nPlanting them before you go off!"
			});


	// Initialize dialogue box
	FarmNPC::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	// Initialise Grid Stuff

	if (firstStartGame)
	{
		grid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Farm.txt");
	}

	else
	{
		std::ifstream file("../../Assets/LevelMaps/Farm_User_layout.txt");

		if (!file.is_open())
		{
			grid.fillGrid("Assets/LevelMaps/NewDungeons/BackgroundCollisions/Farm.txt");
		}

		else
		{
			file.close();
			grid.fillGrid("../../Assets/LevelMaps/Farm_User_layout.txt");
		}
	}

	// Growing plant
	grid.growPlants(grid);

	//tutorial popup boxes
	inventoryPopup = UIManager::create<UI_Elements::PopupBox>("inventoryPopup", 100.0f, -80.0f, 500.0f, 250.0f, "Inventory", "Access your items", "with your keyboard numbers!");
	inventoryPopup->setOnDismiss([]() {
		inventoryPopup->hide();
		cropPopup->show();
		});

	cropPopup = UIManager::create<UI_Elements::PopupBox>("cropPopup", -400.0f, 380.0f, 450.0f, 250.0f, "Planting Crops", "Press E over a crop tile", "to plant a seed!");
	cropPopup->setOnDismiss([]() {
		cropPopup->hide();
		});

	harvestPopup = UIManager::create<UI_Elements::PopupBox>("harvestPopup", -500.0f, 380.0f, 400.0f, 250.0f, "Harvesting Crops", "Press E over your crops", "to harvest them!");
	harvestPopup->setOnDismiss([]() {
		harvestPopup->hide();
		});

	//show harvest popup only on second visit
	// Change this later to find for crop finished tile instead
	if (World::dungeonTracker[0] && grid.findTile(grid, World::CarrotCropTile)) {
		harvestPopup->show();;
	}

	else {
		harvestPopup->hide();
	}

	dungeonPopup = UIManager::create<UI_Elements::PopupBox>("dungeonPopup", 300.0f, -100.0f, 450.0f, 250.0f, "Dungeons", "Access the dungeons through", "these teleporters!");
	dungeonPopup->setOnDismiss([]() {
		dungeonPopup->hide();
		});

}

void Farm_Update() {

	auto* player = localPlayer;
	auto* gerald = localGerald;

	// Player update
	player->update(grid);
	inv.update(player);

	activeT = World::activeTile(player->getX(), player->getY(), grid);

	// Keybindings for tests
	if (AEInputCheckTriggered(AEVK_F11)) // Clear Inventory
	{
		World::dungeonTracker[World::checkNum] = true;
		inv.clear(player);
		firstStartGame = false;
		grid.replacingID(World::Teleporter3, World::TeleporterRed);
	}

	if (AEInputCheckTriggered(AEVK_F9)) // Toggle Grid
	{
		onGrid = !onGrid;
	}

	if (AEInputCheckTriggered(AEVK_F10)) // Plant grow wow!
	{
		World::dungeonTracker[World::checkNum] = true;
		grid.growPlants(grid);
	}
	
	if (AEInputCheckTriggered(AEVK_F8))
	{
		for (int i = 0; i < 3; i++)
			World::dungeonTracker[i] = true;
	}

	if ((AEInputCheckTriggered(AEVK_E) || AEInputCheckTriggered(AEVK_LBUTTON)) && !inv.isEmpty(*player))
	{
		World::interactTile(activeT, grid, inv, *player);

		if (cropPopup->getVisible())
		{
			if (inv.findItemCount(*player, Inventory::ItemID::CARROT_SEEDS) < 3 || inv.findItemCount(*player, Inventory::ItemID::CHERRY_SEEDS) < 6)
				cropPopup->hide();
		}
	}

	// Makes it so that user is stuck in tutorial area until done w tutorial, it player completed tutorial already, teleporter shld be normal
	if (firstStartGame)
	{
		if (!inv.findItem(*player, Inventory::ItemID::CARROT_SEEDS) && !inv.findItem(*player, Inventory::ItemID::CHERRY_SEEDS))
		{
			World::standOnTile(next, *player, grid, GS_X, World::TeleporterBlue);
		}
		else
		{
			std::pair<int, int> currTile = grid.getIndex(player->getX(), player->getY());
			if (grid.getTileID(currTile.first, currTile.second) == World::Teleporter1)
			{
				FarmNPC::activeSpeaker = gerald;
				gerald->idleSpeak(FarmNPC::dialogueBox);

			}
			else
			{
				World::standOnTile(next, *player, grid, GS_X, World::TeleporterBlue);
			}
		}
	}

	else
	{	
		// Basically checking if not cleared then able to go to level 1, 2 and 3 respectively
		if (!World::dungeonTracker[0])
			World::standOnTile(next, *player, grid, GS_X, World::TeleporterBlue);

		if (!World::dungeonTracker[1])
			World::standOnTile(next, *player, grid, GS_NEW, World::TeleporterGreen);
		
		if (!World::dungeonTracker[2])
			World::standOnTile(next, *player, grid, GS_DESERT, World::TeleporterRed);

		// Making sure that prev level closes to prevent most bugs
		if (World::dungeonTracker[0] && !World::dungeonTracker[1])
		{
			grid.replacingID(World::TeleporterBlue, World::Teleporter1);
			grid.replacingID(World::Teleporter2, World::TeleporterGreen);

		}

		if (World::dungeonTracker[1])
		{
			grid.replacingID(World::TeleporterGreen, World::Teleporter2);
			grid.replacingID(World::Teleporter3, World::TeleporterRed);
		}

	}


	// NPC TALKING STUFFS

	if (FarmNPC::dialogueBox.getIsActive() && FarmNPC::activeSpeaker) {

		if (gerald->getIsIdling()) {
			FarmNPC::activeSpeaker->idleSpeak(FarmNPC::dialogueBox);
		}

		else {
			FarmNPC::activeSpeaker->speak(FarmNPC::dialogueBox);
		}

		return;
	}

	switch (FarmNPC::state) {

	case FarmNPC::TutorialState::GER_TALK:

		FarmNPC::activeSpeaker = gerald;

		gerald->speak(FarmNPC::dialogueBox);

		if (gerald->getIsPaused()) {
			FarmNPC::state = FarmNPC::TutorialState::GER_IDLE;
			inv.giveItem(*player, Inventory::ItemID::CARROT_SEEDS, 3);
			inv.giveItem(*player, Inventory::ItemID::CHERRY_SEEDS, 6);

			inventoryPopup->show();
		}

		break;


	case FarmNPC::TutorialState::GER_IDLE:

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			gerald->getX(), gerald->getY(),
			75.0f,
			gerald->getWidth(), gerald->getHeight()))
		{
			FarmNPC::activeSpeaker = gerald;
			gerald->idleSpeak(FarmNPC::dialogueBox);
		}

		break;

		// Check if all seeds are planted then next step in dialogue
	case FarmNPC::TutorialState::GER_TALK2:

		FarmNPC::activeSpeaker = gerald;
		gerald->resumeDialogue(FarmNPC::dialogueBox);

		if (gerald->dialogueDone()) {
			FarmNPC::state = FarmNPC::TutorialState::FINISHED;
			grid.replacingID(World::Teleporter1, World::TeleporterBlue);
			dungeonPopup->show();
		}

		break;

	case FarmNPC::TutorialState::FINISHED:

		gerald->setIdleLines({
			"Make sure not to faint too many times in the dungeons.\nIt'll have consequences!", "#",
			"Just in case you blanked out just now- carrots increase your\nsword damage, and cherries can be used to heal.", "#",
			});

		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			gerald->getX(), gerald->getY(),
			75.0f,
			gerald->getWidth(), gerald->getHeight()
		)) {
			FarmNPC::activeSpeaker = gerald;
			gerald->setIdling(true);
			gerald->idleSpeak(FarmNPC::dialogueBox);

			if (gerald->idleDone())
			{
				gerald->restartIdle();
			}
		}

	}

	if (inv.isEmpty(*player) && FarmNPC::state == FarmNPC::TutorialState::GER_IDLE)
	{
		FarmNPC::state = FarmNPC::TutorialState::GER_TALK2;
	}

}


void Farm_Draw() {
	Graphics::image(0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), bg, Shapes::CENTER);

	// Rendering state
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);

	auto* player = localPlayer;
	auto* gerald = localGerald;

	Color::background(Color::Preset::Purple); // The more purple you can see the better, not every single tile should be drawing its own graphic
	
	grid.drawTexture(grid);

	if (onGrid)
	{
		World::drawGrid();
	}

	World::drawTile(activeT, grid);
	EntityManager::draw("gerald");

	player->draw();

	if (showInventory)
	{
		inv.draw();
	}	

	// Basically the stuff that draws the floating pointer on top of the npc
	if (!FarmNPC::dialogueBox.getIsActive()) {
;

		switch (FarmNPC::state) {

		case FarmNPC::TutorialState::GER_IDLE:
			World::drawIndicatorsOnTileType(grid, World::EmptyCropTile, dirt);
			Animations::drawCoolerIndicator(gerald->getX(), gerald->getY(), indicator);
			break;

		case FarmNPC::TutorialState::FINISHED:

			//point to different teleporters depending on cleared
			if (World::dungeonTracker[1]) {
				World::drawIndicatorsOnTileType(grid, World::TeleporterRed, dirt);
			}

			else if (World::dungeonTracker[0]) {
				World::drawIndicatorsOnTileType(grid, World::TeleporterGreen, dirt);
			}

			else {
				World::drawIndicatorsOnTileType(grid, World::TeleporterBlue, dirt);
			}

			Animations::drawCoolerIndicator(gerald->getX(), gerald->getY(), indicator);
			break;

		}
	}

	// Draw dialogue box on top of everything 
	FarmNPC::dialogueBox.draw();

	inventoryPopup->draw();
	cropPopup->draw();
	harvestPopup->draw();
	dungeonPopup->draw();

	Settings::drawLivesLeft();
}

void Farm_Free() 
{
	grid.outWorldMap("../../Assets/LevelMaps/Farm_User_layout.txt");

	auto* player = localPlayer;
	
	lastposX = player->getX();
	lastposY = player->getY();

	std::pair<int, int> currTile = grid.getIndex(lastposX, lastposY);

	if (grid.getTileID(currTile.first, currTile.second) == World::TeleporterBlue)
	{
		currTile.second -= 1;
		std::pair<float, float> newCords = World::getWorldCoords(currTile, grid);
		lastposX = newCords.first + 20.0f;
		lastposY = newCords.second + 20.0f;
	}

	/*Inventory::ItemRegistry::cleanup();*/

	firstStartGame = false;

	inv.saveInventory(player, gameData);
	inv.clear(player);
	inv.setPlayer(nullptr);


	auto* gerald = localGerald;
	if (gerald && gerald->getSprite()) {
		AEGfxTextureUnload(gerald->getSprite()); // unload sprite
	}

	UIManager::clear();
	EntityManager::clear();
}

void Farm_Unload()
{
	/*Inventory::unload();*/

	FarmNPC::activeSpeaker = nullptr;

	AEGfxTextureUnload(indicator);
	AEGfxTextureUnload(bg);

	bg = nullptr;
	/*grid.unloadMapTexture();*/
}
