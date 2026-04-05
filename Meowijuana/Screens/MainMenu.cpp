/*
 * @file MainMenu.cpp
 * @author Jun
 * @brief Main Screen
 */

#include "../pch.h"
#include "MainMenu.hpp"

#include "../Managers/UIManager.hpp"
#include "../GameStateManager.hpp"
#include "../Settings.hpp"

// Button functions
void navigateToGame(void) { next = GS_FARM; }
void navigateToSettings(void) { next = GS_SETTINGS; }
void navigateToCredits(void) { next = GS_CREDITS; }


namespace {
	// Group everything into a state struct to isolate variables per game state
	struct MainMenuState {
		UI_Elements::Button* creditsButton;
		UI_Elements::Button* playButton;
		UI_Elements::Button* settingsButton;
		UI_Elements::Button* quitButton;

		// popup box to confirm on quitting game
		UI_Elements::PopupBox* quitConfirmPopup;

		// Pointer to background for Main menu
		AEGfxTexture* CatastropheLogo = nullptr;
	};

	// Unique pointer to manage the state's lifetime strictly between Load and Unload
	std::unique_ptr<MainMenuState> state;
}

// On leaving the game
void confirmExit(void) { state->quitConfirmPopup->show(); }
void hidePopup(void) { state->quitConfirmPopup->hide(); }

void exitGame(void) { next = GS_QUIT; }


void Mainmenu_Load() {
	state = std::make_unique<MainMenuState>();

	// Load images
	state->CatastropheLogo = AEGfxTextureLoad("Assets/Images/Backgrounds/main.png");
}

void Mainmenu_Initialize()
{
	Settings::currentScreen = "MainMenu.cpp";

	// set the button values
	state->creditsButton = UIManager::create<UI_Elements::Button>("creditsButton", -300.0f, -200.0f, 200.0f, 100.0f, "Credits", Shapes::CENTER);
	state->playButton = UIManager::create<UI_Elements::Button>("playButton", 0.0f, -200.0f, 200.0f, 100.0f, "Play", Shapes::CENTER);
	state->settingsButton = UIManager::create<UI_Elements::Button>("settingsButton", 300.0f, -200.0f, 200.0f, 100.0f, "Settings", Shapes::CENTER);
	state->quitButton = UIManager::create<UI_Elements::Button>("quitButton", 0.0f, -350.0f, 200.0f, 100.0f, "Quit", Shapes::CENTER);

	// Set the confirm popup box
	state->quitConfirmPopup = UIManager::create<UI_Elements::PopupBox>(
		"quitPopup",
		0.0f, 0.0f, 400.0f, 400.0f,
		"Are you sure?",
		"Press confirm to exit",
		"or close to stay"
	);

	// set the button functions
	state->playButton->setOnClick(navigateToGame);
	state->creditsButton->setOnClick(navigateToCredits);
	state->settingsButton->setOnClick(navigateToSettings);
	state->quitButton->setOnClick(confirmExit);

	// Set the buttons for the confirm popup
	state->quitConfirmPopup->setOnconfirm(exitGame);
	state->quitConfirmPopup->setOnDismiss(hidePopup);
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

void Mainmenu_Free() {}

void Mainmenu_Unload()
{
	if (state) {
		AEGfxTextureUnload(state->CatastropheLogo);
		state.reset(); // Destroy state memory so it's fresh the next time we enter Main Menu
	}

	UIManager::clear();
}
