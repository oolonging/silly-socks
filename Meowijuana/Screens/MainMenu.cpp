#include "../pch.h"
#include "MainMenu.hpp"

#include "../Managers/UIManager.hpp"
#include "../GameStateManager.hpp"

// Button functions
void navigateToGame(void) { next = GS_FARM; }
void navigateToSettings(void) { next = GS_SETTINGS; }
void navigateToCredits(void) { next = GS_CREDITS; }
void navigateToTest(void) { next = GS_A; }
void navigateToTutorial(void) { next = GS_DUNGEON; }

namespace {
	// Group everything into a state struct to isolate variables per game state
	struct MainMenuState {
		UI_Elements::Button* creditsButton;
		UI_Elements::Button* playButton;
		UI_Elements::Button* settingsButton;
		UI_Elements::Button* testButton;
		UI_Elements::Button* tutorialButton;
		
		// Pointer to background for Main menu
		AEGfxTexture* CatastropheLogo = nullptr;
	};

	// Unique pointer to manage the state's lifetime strictly between Load and Unload
	std::unique_ptr<MainMenuState> state;
}

void Mainmenu_Load() {
	state = std::make_unique<MainMenuState>();

	// Load background image
	state->CatastropheLogo = AEGfxTextureLoad("Assets/Images/Backgrounds/Cat.png");
}

void Mainmenu_Initialize()
{
	// set the button values
	state->creditsButton = UIManager::create<UI_Elements::Button>("creditsButton", -300.0f, -200.0f, 200.0f, 100.0f, "Credits", Shapes::CENTER);
	state->playButton = UIManager::create<UI_Elements::Button>("playButton", 0.0f, -200.0f, 200.0f, 100.0f, "Play", Shapes::CENTER);
	state->settingsButton = UIManager::create<UI_Elements::Button>("settingsButton", 300.0f, -200.0f, 200.0f, 100.0f, "Settings", Shapes::CENTER);
	state->testButton = UIManager::create<UI_Elements::Button>("testButton", 400.0f, 200.0f, 200.0f, 100.0f, "Test", Shapes::CENTER);
	state->tutorialButton = UIManager::create<UI_Elements::Button>("tutorialButton", 600.0f, 400.0f, 200.0f, 100.0f, "Tutorial", Shapes::CENTER);
	
	// set the button functions
	state->playButton->setOnClick(navigateToGame);
	state->creditsButton->setOnClick(navigateToCredits);
	state->settingsButton->setOnClick(navigateToSettings);
	state->testButton->setOnClick(navigateToTest);
	state->tutorialButton->setOnClick(navigateToTutorial);
}

void Mainmenu_Update() {
	// TODO: consider separating the update and render logic of buttons for easier debugging
}

void Mainmenu_Draw()
{
	if (!state) return;

	// Draw the catastrophe logo in the middle of the screen
	Graphics::image(0, 0, 1600, 900, state->CatastropheLogo, Shapes::CENTER);

	// Draw all UI elements
	UIManager::drawAll();
}

void Mainmenu_Free() {
}

void Mainmenu_Unload()
{
	if (state) {
		AEGfxTextureUnload(state->CatastropheLogo);
		state.reset(); // Destroy state memory so it's fresh the next time we enter Main Menu
	}

	UIManager::clear();
}


