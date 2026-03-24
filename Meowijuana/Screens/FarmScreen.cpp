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
static bool shownHarvestPopup = false;

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
}

void Farm_Initialize() {
	// Set the debug "Current Screen" property
	Settings::currentScreen = "FarmScreen.cpp";
	
	// check if all the dungeons are cleared, if so then naviate to the win screen
	if (World::dungeonTracker[0] && World::dungeonTracker[1] && World::dungeonTracker[2]) {
		next = GS_WIN;
	}


	// init Entitymanager
	EntityManager::init();

	// Initialising Player
	localPlayer = EntityManager::getPlayer("player");
	auto* player = localPlayer;
	player->setPosition(-800.0f, 50.0f);
	player->setSpeed(10.f);



	FarmNPC::dialogueBox.setSpeaker("Gerald");



	// Setting position for inventory 
	float screenWidth = static_cast<float>(AEGfxGetWindowWidth());
	float screenHeight = static_cast<float>(AEGfxGetWindowHeight());
	float invWidth = inv.getSlotSize() * 9 + player->getInventorySize() * 8;
	float offset = 10.0f;

	float x = -invWidth / 2.0f;         // centered horizontally
	float y = -(screenHeight / 2.0f) + inv.getSlotSize() + offset; // near the bottom 

	indicator = AEGfxTextureLoad("Assets/Indicators/SpeechBubble.png");

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
	localGerald = EntityManager::create<Entity::NPC>("Gerald", -50.0f, 100.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
	auto* Gerald = localGerald;
	Gerald->setCharName("Gerald"); 
	Gerald->setSprite(AEGfxTextureLoad("Assets/Images/Entities/Gerald_Stationary.png"));


	Gerald->setDialogLines({
		"Welcome to Catastrofarm! I heard you are new here so here are some seeds to get you started on your journey! Press E when hovering over a empty crop spot.", "@",
		"Good Job! After planting the seeds, it will grow everytime you finish a dungeon level so make sure you make full use of your farm space!",
		"You can get more seeds while exploring the dungeons so look out for that!", "@"
	});

	if (!World::dungeonTracker[0])
	Gerald->setIdleLines({
		"Hey you can't leave without planting those! Try Planting the seeds that I have just given you!"
	});

	else
		Gerald->setIdleLines({
		"Carrots increase your sword damage!", "@",
		"Cherries can be used to heal!", "@",
		"Have fun farming!", "@"
			});

	// Initialize dialogue box
	FarmNPC::dialogueBox = UI_Elements::DialogueBox(0.0f, -300.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	// Initialise Grid Stuff
	/*grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);*/
	
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
	inventoryPopup = UIManager::create<UI_Elements::PopupBox>("inventoryPopup", 120.0f, -80.0f, 500.0f, 250.0f, "Inventory", "This is your inventory.", "Access your items with your keyboard numbers!");
	inventoryPopup->setOnDismiss([]() {
		inventoryPopup->hide();
		cropPopup->show();
		});

	cropPopup = UIManager::create<UI_Elements::PopupBox>("cropPopup", -400.0f, 380.0f, 350.0f, 250.0f, "Planting Crops", "These are crop tiles!", "Press E over one to plant a seed!");
	cropPopup->setOnDismiss([]() {
		cropPopup->hide();
		});

	harvestPopup = UIManager::create<UI_Elements::PopupBox>("harvestPopup", -500.0f, 380.0f, 400.0f, 250.0f, "Harvesting Crops", "Looks like your crops have grown!", "Press E over them to harvest them!");
	harvestPopup->setOnDismiss([]() {
		harvestPopup->hide();
		});

	//show harvest popup only on second visit
	if (!firstStartGame && !shownHarvestPopup) {
		harvestPopup->show();
		shownHarvestPopup = true;
	}
	else {
		harvestPopup->hide();
	}

	dungeonPopup = UIManager::create<UI_Elements::PopupBox>("dungeonPopup", 300.0f, -100.0f, 400.0f, 250.0f, "Dungeons", "Access the dungeons through", "these teleporters!");
	dungeonPopup->setOnDismiss([]() {
		dungeonPopup->hide();
		});

}

void Farm_Update() {

	auto* player = localPlayer;
	auto* Gerald = localGerald;

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


	if (AEInputCheckTriggered(AEVK_E))
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
		if (!inv.findItem(*player,Inventory::ItemID::CARROT_SEEDS) && !inv.findItem(*player, Inventory::ItemID::CHERRY_SEEDS))
		{
			World::standOnTile(next, *player, grid, GS_X, World::TeleporterBlue);
		}
		else
		{
			std::pair<int, int> currTile = grid.getIndex(player->getX(), player->getY());
			if (grid.getTileID(currTile.first, currTile.second) == World::Teleporter1)
			{
				FarmNPC::activeSpeaker = Gerald;
				Gerald->idleSpeak(FarmNPC::dialogueBox);

			}
			else
			{
				World::standOnTile(next, *player, grid, GS_X, World::TeleporterBlue);
			}
		}
	}

	else
	{
		World::standOnTile(next, *player, grid, GS_X, World::TeleporterBlue);
		World::standOnTile(next, *player, grid, GS_NEW, World::TeleporterGreen);
		World::standOnTile(next, *player, grid, GS_DESERT, World::TeleporterRed);
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

			inventoryPopup->show();
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
				
		if (Gerald->dialogueDone()) {
			FarmNPC::state = FarmNPC::TutorialState::FINISHED;
			grid.replacingID(World::Teleporter1, World::TeleporterBlue);
			dungeonPopup->show();
		}

		break;

	case FarmNPC::TutorialState::FINISHED:
		
		Gerald->setIdleLines({
		"Carrots increase your sword damage!", "#",
		"Cherries can be used to heal!", "#",
		"Have fun farming!", "#"
			});
		
		if (AEInputCheckTriggered(AEVK_E) && Collision::collidedWith(
			player->getX(), player->getY(),
			Gerald->getX(), Gerald->getY(),
			75.0f,
			Gerald->getWidth(), Gerald->getHeight()
		)) {
			FarmNPC::activeSpeaker = Gerald;
			Gerald->setIdling(true);
			Gerald->idleSpeak(FarmNPC::dialogueBox);
	
			if (Gerald->idleDone())
			{
				Gerald->restartIdle();
			}
		}

	}

	if (inv.isEmpty(*player) && FarmNPC::state == FarmNPC::TutorialState::GER_IDLE)
	{
		FarmNPC::state = FarmNPC::TutorialState::GER_TALK2;
	}

	if (World::dungeonTracker[0])
	{
		grid.replacingID(World::Teleporter2, World::TeleporterGreen);

	}

	if (World::dungeonTracker[1])
	{
		grid.replacingID(World::Teleporter3, World::TeleporterRed);
	}
}

void Farm_Draw() {

	Graphics::image(0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), bg, Shapes::CENTER);

	// Rendering state
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);

	auto* player = localPlayer;
	auto* Gerald = localGerald;

	Color::background(Color::Preset::Purple); // The more purple you can see the better, not every single tile should be drawing its own graphic
	
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
	if (!FarmNPC::dialogueBox.getIsActive()) {
;

		switch (FarmNPC::state) {

		case FarmNPC::TutorialState::GER_IDLE:
			World::drawIndicatorsOnTileType(grid, World::EmptyCropTile, dirt);
			Animations::drawCoolerIndicator(Gerald->getX(), Gerald->getY(), indicator);
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

			Animations::drawCoolerIndicator(Gerald->getX(), Gerald->getY(), indicator);
			break;

		}
	}

	// Draw dialogue box on top of everything 
	FarmNPC::dialogueBox.draw();

	inventoryPopup->draw();
	cropPopup->draw();
	harvestPopup->draw();
	dungeonPopup->draw();
	
}

void Farm_Free() 
{
	grid.outWorldMap("../../Assets/LevelMaps/Farm_User_layout.txt");

	auto* player = localPlayer;
	auto* Gerald = localGerald;

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

	/*grid.unloadMapTexture();
	Inventory::ItemRegistry::cleanup();*/

	firstStartGame = false;

	inv.saveInventory(player, gameData);
	inv.clear(player);
	inv.setPlayer(nullptr);
}

void Farm_Unload() 
{
	/*Inventory::unload();*/

	auto* Gerald = localGerald;
	if (Gerald && Gerald->getSprite()) {
		AEGfxTextureUnload(Gerald->getSprite()); // unload sprite
	}

	AEGfxTextureUnload(indicator);

	FarmNPC::activeSpeaker = nullptr;
	EntityManager::clear();

}
