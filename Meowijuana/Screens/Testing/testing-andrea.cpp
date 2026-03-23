#include "../../pch.h"
#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../World.hpp"
#include "../../Entity.hpp"
#include "testing-andrea.hpp"

// Entities player + Inventory creation
Entity::Player user;
UI_Elements::PlayerInventory inven;

// For world grid
std::pair<int, int> prevActiveTile;
std::pair<int, int> activeTile;

World::worldGrid Griddy;
bool turnGridOn = true;

void Andrea_Load() 
{
	Inventory::load();
}

void Andrea_Initialize() 
{
	Inventory::init();

	// Setting position for inventory
	float screenWidth = static_cast<float>(AEGfxGetWindowWidth());
	float screenHeight = static_cast<float>(AEGfxGetWindowHeight());
	float invWidth = inven.getSlotSize() * 9 + user.getInventorySize() * 8;
	float offset = 10.0f;

	float x = -invWidth / 2.0f;         // centered horizontally
	float y = -(screenHeight / 2.0f) + inven.getSlotSize() + offset; // near the bottom 

	inven.setPosition(x, y);
	inven.setPlayer(&user);

	user.setX(0);
	user.setY(0); 

	Griddy.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	Griddy.initMapTexture();
	Griddy.initTextureBox();
	Griddy.fillGrid("../../Assets/LevelMaps/Farm_layout.txt");

}

void Andrea_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Text::text("Andrea's testing screen", 0, 0);
	
	//World::collidableNearby(user, Griddy);

	activeTile = World::activeTile(user.getX(), user.getY(), Griddy);

	if (AEInputCheckTriggered(AEVK_F9))
	{
		turnGridOn = !turnGridOn;
	}

	if (AEInputCheckTriggered(AEVK_E))
	{
		World::interactTile(activeTile, Griddy, inven, user);
	}

	/*if (AEInputCheckTriggered(AEVK_F1))
	{
		inven.giveSeeds(user);
		Inventory::Item* test = user.getInventoryItem(0);
		if (test != nullptr)
			printf("Slot 0 has: %s x%d\n", test->getName().c_str(), test->getCount());
		else
			printf("Slot 0 is empty!\n");
	}*/

	World::standOnTile(next, user, Griddy, GS_X, World::TeleporterBlue);

	user.update();
	/*inven.update();*/
}

void Andrea_Draw() 
{
	Griddy.drawTexture(Griddy);

	if (turnGridOn)
	{
		World::drawGrid();
	}

	World::drawTile(activeTile, Griddy);
	user.draw();
	inven.draw();
}

void Andrea_Free() 
{
	Griddy.unloadMapTexture();
	World::freeGrid();
	Inventory::ItemRegistry::cleanup(); 
	user.freeInventory();
}

void Andrea_Unload() 
{
	Inventory::unload();
}

