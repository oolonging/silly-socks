#include <iostream>

#include "../Tiles.hpp"
#include "../Entity.hpp"
#include "../Managers/EntityManager.hpp"

namespace Cashew {
	// UI_Element Dialogue box
	UI_Elements::DialogueBox dialogueBox;
}

void Testing_Load() {

}

void Testing_Initialize() {
	EntityManager::init();

	// Initialize dialogue box
	Cashew::dialogueBox = UI_Elements::DialogueBox(-600.0f, -300.0f, 1200.0f, 100.0f, "Prasanna", "Hello there! Welcome to Meowijuana!", EntityManager::getNPC("prasanna")->getSprite(), Shapes::CORNER);

	// Initialize world
	World::init();
}

void Testing_Update() {
	// Get references when needed
	auto* player = EntityManager::getPlayer("player");
	auto* prasanna = EntityManager::getNPC("prasanna");
	auto* soroor = EntityManager::getNPC("soroor");

	// Update the player
	player->update();

	// Check for interaction with Prasanna
	if (Collision::collidedWith(
		player->getX(), player->getY(),
		prasanna->getX(), prasanna->getY(),
		75.0f,
		prasanna->getWidth(), prasanna->getHeight()
	)) {
		prasanna->speak(Cashew::dialogueBox);
	}

	// Check for interaction with Soroor
	if (Collision::collidedWith(
		player->getX(), player->getY(),
		soroor->getX(), soroor->getY(),
		75.0f,
		soroor->getWidth(), soroor->getHeight()
	)) {
		soroor->speak(Cashew::dialogueBox);
	}
}

void Testing_Draw() {
	// Draw the grid
	World::draw();

	// Draw all entities (automatically includes player and NPCs)
	EntityManager::drawAll();

	// Draw dialogue box if active
	Cashew::dialogueBox.draw();
}

void Testing_Free() {

}

void Testing_Unload() {
	EntityManager::clear();
	World::exit();
}