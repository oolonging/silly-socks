// ---------------------------------------------------------------------------
// includes

// Precomiled headers
#include "pch.h"

#include <crtdbg.h> // To check for memory leaks

#include "Graphics.hpp"
#include "UI_Elements/UI_Elements.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Settings.hpp"
#include "GameStateManager.hpp"
#include "InputManager.hpp"
#include "Managers/UIManager.hpp"
#include "AudioManager.hpp"
#include "Managers/ParticleManager.hpp"
#include "Managers/AchievementManager.hpp"
#include "Inventory.hpp"
#include "PauseMenu.hpp"


// ---------------------------------------------------------------------------
// main

extern World::worldGrid grid;
extern GameData gameData;
extern UI_Elements::PlayerInventory inv;
extern bool showInventory;

namespace AudioManager {
	Audio audio;
}
float bgVolume = 100.0f;
float sfxVolume = 140.0f;

// Temporary function to help navigate between the screens
void screenSwitcher(void) {
	if (AEInputCheckTriggered(AEVK_NUMPAD1))
		next = GS_TUTORIAL;

	if (AEInputCheckTriggered(AEVK_NUMPAD2))
		next = GS_MAIN_MENU;

	if (AEInputCheckTriggered(AEVK_NUMPAD3))
		next = GS_SETTINGS;

	if (AEInputCheckTriggered(AEVK_NUMPAD4))
		next = GS_CREDITS;

	if (AEInputCheckTriggered(AEVK_NUMPAD5))
		next = GS_FARM;

	if (AEInputCheckTriggered(AEVK_NUMPAD6))
		next = GS_DUNGEON;

	if (AEInputCheckTriggered(AEVK_NUMPAD7))
		next = GS_TESTING;

	if (AEInputCheckTriggered(AEVK_NUMPAD8))
		next = GS_LEVEL1;

	if (AEInputCheckTriggered(AEVK_NUMPAD9))
		next = GS_LEVEL2;

	if (AEInputCheckTriggered(AEVK_NUMPAD0))
		next = GS_TUTDUN;

	if (AEInputCheckTriggered(AEVK_NUM_MINUS))
		next = GS_NEW;


	if(AEInputCheckTriggered(AEVK_0))
		next = GS_BACK;

	if (AEInputCheckTriggered(AEVK_R))
		next = GS_RESTART;


}

// Helper function to initialize subsystem
void InitSystems(int windowWidth, int windowHeight) {
	Shapes::init();
	grid.initGrid(windowWidth, windowHeight, 50);

	Inventory::load();
	Inventory::init();

	// Font initialization
	Text::createFont("Assets/Fonts/buggy-font.ttf", Settings::gDefaultTextSize, "default");
	Text::createFont("Assets/Fonts/comic-sans.ttf", Settings::gDefaultTextSize, "comicsans");
	Text::createFont("Assets/Fonts/impact.ttf", Settings::gDefaultTextSize, "impact");

	Text::setFont("default"); // Redundant: but basically this is how you use it

	// set text align to center
	Text::textAlign(Text::CENTER_H, Text::CENTER_V);

	// init custom cursor(s);
	Input::init();

	// init the achievementManager
	AchievementManager::get().init("Assets/achievement.txt");

	// Immediately earn the achievement "Start the game"
	// ill add this back later when the achievemnts are more fleshed out / have actual graphics to render	
	//AchievementManager::get().unlockAchievement("Start the game");

	// load the meshes for global particles
	gParticles.init();

	//audio
	AudioManager::audio.init();
	AudioManager::audio.loadBGM("Assets/bgm.mp3");
	AudioManager::audio.loadSFX("Assets/Sounds/hit.wav");
	AudioManager::audio.playBGM(bgVolume / 100.0f, true);

}

void ShutdownSystems(void) {
	// Free resources
	UIManager::clear();
	World::freeGrid();
	Shapes::exit();
	Text::exit();
	Input::exit();
	AudioManager::audio.exit();

	// clear out the fonts that went unloaded
	Text::destroyFont("default");
	Text::destroyFont("comicsans");
	Text::destroyFont("impact");
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// Create a 1600 x 900 windowed application at 60 FPS
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
	AESysSetWindowTitle("Silly Socks | Meowijuana");

	// reset the system modules
	AESysReset();

	// Init subsystem
	InitSystems(1600, 900);

	GSM_Initialize(GS_SPLASH);

	while (current != GS_QUIT) {
		// set the function pointers for current state
		GSM_Update();

		// Enter state
		fpLoad();
		fpInitialize();
		PauseMenu_Initialize();

		//game states that can be paused
		bool canPause = (current == GS_LEVEL1 ||
			current == GS_LEVEL2 ||
			current == GS_DUNGEON ||
			current == GS_FARM ||
			current == GS_TUTDUN ||
			current == GS_TUTORIAL
			);

		// Level loop
		while (current == next) {
			AESysFrameStart();
			// Reset Cursor type
			Input::gCursorType = Input::POINTER;

			// update animation time
			SpriteManager::updateAnimationTime();

			// temp removing while i test hitting sound
			//if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			//	AudioManager::audio.playSFX(sfxVolume / 100.0f);
			//}

			// temporary  during the debugging phase
			screenSwitcher();

			//pause logic
			if (AEInputCheckTriggered(AEVK_ESCAPE)) {
				next = GS_QUIT;
			}

			if (AEInputCheckTriggered(AEVK_P) && canPause)
				isPaused = !isPaused;

			if (isPaused) {
				PauseMenu_Update();
			}
			else {
				fpUpdate();  //only runs level update when not paused
			}

			//draw game
			fpDraw();

			//draw pause menu
			if (isPaused)
				PauseMenu_Draw();

			// Update input manager every frame
			Input::update();

			// gParticles
			gParticles.update();
			gParticles.draw();

			// Draw debug overlay and toggle with F3
			if (AEInputCheckTriggered(AEVK_F3)) Settings::toggleDebugMode();
			if (Settings::gDebugMode) Settings::drawDebugOverlay();

			// Draw achievement popup
			AchievementManager::get().update();
			AchievementManager::get().draw();

			AESysFrameEnd();

			// Handle restart
			if (next == GS_RESTART) {
				fpFree();
				fpInitialize();
				next = current;
			}

			if(next == GS_BACK) {
				next = previous;
			}
		}

		// Exit state
		fpFree();
		fpUnload();

		previous = current;
		current = next;
	}

	// Free global stuffs
	grid.unloadMapTexture();
	World::freeGrid();
	Inventory::unload();      

	// Shutdown subsystem
	ShutdownSystems();
	
	AESysExit();
}


