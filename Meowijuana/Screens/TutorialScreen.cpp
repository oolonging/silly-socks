#include "TutorialScreen.hpp"
#include "AEEngine.h"
#include "../Graphics.hpp"
#include "../Entity.hpp"
#include "../UI_Elements.hpp"

extern Entity::Player testPlayer;

namespace TutorialScreen {

	// Testing UI Element dialogue box
	

	// test 1
	UI_Elements::DialogueBox testDialogue;
}

void Tutorial_Load() {
}

void Tutorial_Initialize() {
	// Non default constructor
	float dialogueWidth = 1200.0f;
	float dialogueHeight = 250.0f;
	float dialogueX = -dialogueWidth * 0.5f;
	float dialogueY = -300.0f;


	TutorialScreen::testDialogue = UI_Elements::DialogueBox(
		dialogueX, dialogueY, dialogueWidth, dialogueHeight,
		"Tutorial Guide", "Welcome to the tutorial. Dis da tutorial",
		nullptr, // No sprites available to test yet,
		Shapes::CORNER
	);

	// defaullt constructor

	// Activate it so it shows up
	TutorialScreen::testDialogue.activate();
}

void Tutorial_Update() {
	testPlayer.update();

	// just for debugging: reactivates the dialogue box when you press space
	if (AEInputCheckTriggered(AEVK_SPACE)) {
		TutorialScreen::testDialogue.activate();
	}
}

void Tutorial_Draw() {
	Color::background(Color::Preset::White);
	Color::fill(Color::Preset::Blue);
	Text::text("Dis da Tutorial", 0.0f, 0.0f);

	// TODO :the text isnt aligned properly, also look into why the default style wasnt working earlier
	TutorialScreen::testDialogue.draw();

	testPlayer.draw();
}

void Tutorial_Free() {}

void Tutorial_Unload() {}
