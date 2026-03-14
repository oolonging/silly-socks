#include "../../pch.h"
#include "testing-ui-elements.hpp"
#include "../../Managers/UIManager.hpp"

///////////////////////////////////////////
// Button testing
///////////////////////////////////////////

// Create a bunch of buttons
UI_Elements::Button* button1;
UI_Elements::Button* button2;
UI_Elements::Button* button3;
UI_Elements::Button* button4;


// helper function
void meow(void) { printf("Meow\n"); }

// Globals to shift the values later
Shapes::SHAPE_MODE drawMode = Shapes::CORNER;

void testingUI_Load() {

}

void testingUI_Initialize() {
	// Initialize the 4 buttons
	button1 = UIManager::create<UI_Elements::Button>("button1", 50.0f, -25.0f, 100.0f, 50.0f, "button1", drawMode);
	button2 = UIManager::create<UI_Elements::Button>("button2", -50.0f, -25.0f, -100.0f, 50.0f, "button2", drawMode);
	button3 = UIManager::create<UI_Elements::Button>("button3", -50.0f, 25.0f, -100.0f, -50.0f, "button3", drawMode);
	button4 = UIManager::create<UI_Elements::Button>("button4", 50.0f, 25.0f, 100.0f, -50.0f, "button4", drawMode);

}

void testingUI_Update() {
	
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


