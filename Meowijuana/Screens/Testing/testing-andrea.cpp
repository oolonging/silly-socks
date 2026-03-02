#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../World.hpp"
#include "../../Entity.hpp"
#include "testing-andrea.hpp"

// Entities player and enemy
Entity::Player user;
std::pair<int, int> prevActiveTile;
std::pair<int, int> activeTile;

World::worldGrid Griddy;
bool turnGridOn = true;

void Andrea_Load() {
}

void Andrea_Initialize() 
{
	user = Entity::Player(
		-400.0f, -400.0f,
		50.0f, 50.0f,
		100.0f, 5.0f, 0.0f
	);

	Griddy.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
	Griddy.initMapTexture();
	Griddy.initTextureBox();
	Griddy.fillGrid("../../Assets/LevelMaps/Farm_layout.txt");
	Griddy.outWorldMap("../../Assets/LevelMaps/Check.txt");

}

void Andrea_Update() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 100, 100, 100, 255 });
	Text::text("Andrea's testing screen", 0, 0);
	activeTile = World::activeTile(user.getX(), user.getY(), Griddy);

	if (AEInputCheckTriggered(AEVK_F9))
	{
		turnGridOn = !turnGridOn;
	}

	if (AEInputCheckTriggered(AEVK_E))
	{
		World::interactTile(activeTile, Griddy);
	}

	user.update();
}

void Andrea_Draw() 
{
	Griddy.drawTexture(Griddy);

	if (turnGridOn)
	{
		World::drawGrid();
	}

	World::drawTile(activeTile, Griddy);
	World::drawTile({0,0}, Griddy);
	user.draw();
}

void Andrea_Free() 
{
}

void Andrea_Unload() {}