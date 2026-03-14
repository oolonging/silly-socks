#include "testing-ui-elements.hpp"
#include "../../Managers/UIManager.hpp"

///////////////////////////////////////////
// Button testing
///////////////////////////////////////////

// Create a bunch of buttons
UI_Elements::Button* graphicalButton;
UI_Elements::Button* colorButton;

// Create a progress bar
UI_Elements::ProgressBar* mouseXBar;
UI_Elements::ProgressBar* mouseXCornerBar;
float mouseX;

// helper function
void meow(void) { printf("Meow\n"); }

// Globals to shift the values later
Shapes::SHAPE_MODE drawMode = Shapes::CORNER;

void testingUI_Load() {

}

void testingUI_Initialize() {
	// Initialize the 2 buttons
	graphicalButton = UIManager::create<UI_Elements::Button>("graphicalButton", -200.0f, 0.0f, 100.0f, 50.0f, "Graphics");
	colorButton = UIManager::create<UI_Elements::Button>("colorButton", 200.0f, 0.0f, 100.0f, 50.0f, "Color");

	// Initialize progress bar 
	mouseXBar = UIManager::create<UI_Elements::ProgressBar>("mouseXBar", 0.0f, 0.0f, 1600.0f, 30.0f, &mouseX, 0.0f, 1600.0f, Shapes::CENTER);
	mouseXCornerBar = UIManager::create<UI_Elements::ProgressBar>("mouseXCornerBar", -800.0f, -15.0f, 1600.0f, 30.0f, &mouseX, 0.0f, 1600.0f, Shapes::CORNER);

	// Strip color button of graphics
	colorButton->clearTextures();

	// set the functions for the two buttons
	graphicalButton->setOnClick(meow);
	colorButton->setOnClick(meow);
}

void testingUI_Update() {
	// Update mouseX value
	mouseX = Input::getScreenMouseX();
}

void testingUI_Draw() {
	// Draw all UI elements
	UIManager::drawAll();
}

void testingUI_Free() {
	// clear all UI elements
	UIManager::clear();
}

void testingUI_Unload() {

}
