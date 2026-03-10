#include <iostream>

#include "testing-ui-elements.hpp"
#include "AEEngine.h"
#include "../../GameStateManager.hpp"

// UI manager
#include "../../Managers/UIManager.hpp"

// Globals
bool graphicalRendering = true;
void toggleGraphicalRendering(bool newValue) { 
	graphicalRendering = newValue;
	next = GS_RESTART;
}

float health = 50.0f;
float maxHealth = 100.0f;
float damageModifier = 5.0f;
void heal(void) { health += damageModifier; }
void hurt(void) { health -= damageModifier; }


float backgroundRed = 255.0f;
float backgroundGreen = 255.0f;
float backgroundBlue = 255.0f;


// Create UI Elements
UI_Elements::Button* healButton;
UI_Elements::Button* hurtButton;

UI_Elements::ProgressBar* healthBar;
UI_Elements::Slider* damageModifierSlider;

UI_Elements::TextBox* myTextBox;

UI_Elements::Slider* redSlider;
UI_Elements::Slider* greenSlider;
UI_Elements::Slider* blueSlider;

UI_Elements::Checkbox* graphicalToggle;

void testingUI_Load() {

}

void testingUI_Initialize() {
	// check to see if graphical rendering is enabled
	if (!graphicalRendering) {
		UIManager::clearDefaults();
	}
	else {
		UIManager::initDefaults();
	}

	// Initialize the buttons using the UI manager
	healButton = UIManager::create<UI_Elements::Button>("healButton", -750.0f, 350.0f, 100.0f, 50.0f, "Heal", Shapes::CORNER);
	hurtButton = UIManager::create<UI_Elements::Button>("hurtButton", -700.0f, 200.0f, 100.0f, 50.0f, "Hurt", Shapes::CENTER);

	// Set the buttons to hurt or heal depending on what it do
	healButton->setOnClick(heal);
	hurtButton->setOnClick(hurt);

	// Initialize the progress bar
	healthBar = UIManager::create<UI_Elements::ProgressBar>("healthBar", -550.0f, 350.0f, 500.0f, 50.0f, &health, 0.0f, maxHealth, Shapes::CORNER);

	// Initialize the slider
	damageModifierSlider = UIManager::create<UI_Elements::Slider>("damageModifierSlider", -550.0f, 225.0f, 500.0f, 50.0f, &damageModifier, 0.0f, 10.0f, Shapes::CORNER);

	// Initialize the text box
	myTextBox = UIManager::create<UI_Elements::TextBox>("myTextBox", -700.0f, 100.0f, 500.0f, 250.0f, "Enter some text here");

	// Initiate the RGB sliders
	redSlider = UIManager::create<UI_Elements::Slider>("redSlider", -700.0f, -200.0f, 100.0f, 10.0f, &backgroundRed, 0.0f, 255.0f);
	greenSlider = UIManager::create<UI_Elements::Slider>("greenSlider", -700.0f, -300.0f, 100.0, 10.0f, &backgroundGreen, 0.0f, 255.0f);
	blueSlider = UIManager::create<UI_Elements::Slider>("blueSlider", -700.0f, -400.0f, 100.0f, 10.0f, &backgroundBlue, 0.0f, 255.0f);

	// Initiate the Graphical toggle checkbox
	graphicalToggle = UIManager::create<UI_Elements::Checkbox>("graphicalToggle", 275.0f, 360.0f, 30.0f, "Toggle graphics", graphicalRendering);
	graphicalToggle->setOnChange(toggleGraphicalRendering);
}

void testingUI_Update() {

}

void testingUI_Draw() {
	// reset background
	Color::background(backgroundRed, backgroundGreen, backgroundBlue);

	// Draw the UI elements
	UIManager::drawAll();

	// set text color to black
	Color::textFill(Color::Preset::Black);

	// Label for the damage modifier and HP
	Text::text("HP", -30.0f, 325.0f, Text::LEFT, Text::CENTER_V);
	Text::text("Damage Modifier", -30.0f, 200.0f, Text::LEFT, Text::CENTER_V);

	// labels for the RGB slider at the bottom
	Text::text("Red", -710.0f, -200.0f, Text::RIGHT, Text::BOTTOM);
	Text::text("Green", -710.0f, -300.0f, Text::RIGHT, Text::BOTTOM);
	Text::text("Blue", -710.0f, -400.0f, Text::RIGHT, Text::BOTTOM);
}

void testingUI_Free() {

}

void testingUI_Unload() {

}

