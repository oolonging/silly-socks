#include "testing-ui-elements.hpp"
#include "AEEngine.h"
#include "../../UI_Elements/UI_Elements.hpp"
#include "../../GameStateManager.hpp"
#include "../../Managers/UIManager.hpp"

// Quick button references
UI_Elements::Button* myButton = nullptr;
UI_Elements::Button* yourButton = nullptr;
UI_Elements::Button* theirButton = nullptr;

// Helper function for button
void meow(void) {
	printf("Meow\n");
}

void testingUI_Load() {
	// Create the buttons
	UIManager::create<UI_Elements::Button>("myButton", 0.0f, 0.0f, 100.0f, 50.0f, "Meow", Shapes::CENTER);
	UIManager::create<UI_Elements::Button>("yourButton", 150.0f, 25.0f, 100.0f, 50.0f, "Meow", Shapes::CORNER);
	UIManager::create<UI_Elements::Button>("theirButton", 350.0f, 25.0f, 100.0f, 50.0f, "Meow", Shapes::CORNER);
}
void testingUI_Initialize() {
	// set the functions to meow
	myButton = UIManager::getButton("myButton");
	yourButton = UIManager::getButton("yourButton");
	theirButton = UIManager::getButton("theirButton");

	myButton->setOnClick(meow);
	yourButton->setOnClick(meow);
	theirButton->setOnClick(meow);
}

void testingUI_Update() {
}

void testingUI_Draw() {
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// Draw the buttons
	myButton->draw();
	yourButton->draw();
	theirButton->draw();
}

void testingUI_Free() {

}

void testingUI_Unload() {

}
