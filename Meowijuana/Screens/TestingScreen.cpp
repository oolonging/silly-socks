#include <iostream>

#include "../Tiles.hpp"
#include "../Entity.hpp"
#include "../Managers/EntityManager.hpp"
#include "../Managers/TileManager.hpp"

namespace Cashew {
	// UI_Element Dialogue box
	UI_Elements::DialogueBox dialogueBox;

	// Player Inventory UI
	UI_Elements::PlayerInventory inventoryUI;
	//TODO probably rename to hotbar since thats more accurate
}

void Testing_Load() {

}

void Testing_Initialize() {
	// Initialize tile system (this calls World::init() internally)
	TileManager::init();

	// Load tile layout from file AFTER init
	TileManager::loadTileLayout("Assets/Maps/testing.txt");

	// Initialize entities
	EntityManager::init();

	// Initialize dialogue box
	Cashew::dialogueBox = UI_Elements::DialogueBox(0.0f, 0.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);

	// Initialize inventory UI (bottom center of screen)
	// Adjust x and y positions based on your screen resolution
	auto* player = EntityManager::getPlayer("player");
	Cashew::inventoryUI = UI_Elements::PlayerInventory(
		0.0f,  // x position (adjust to center on your screen)
		0.0f,  // y position (near bottom of screen)
		50.0f,    // slot size
		10.0f,    // spacing between slots
		player,   // player reference
		Shapes::CENTER
	);

	// style the inventory UI
	Cashew::inventoryUI.setStyle({
		{ 0.2f, 0.2f, 0.2f, 0.8f }, // primaryColor
		{ 0.4f, 0.4f, 0.4f, 1.0f }, // secondaryColor
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // strokeColor
		2 // strokeWeight
		});



}

void Testing_Update() {
	// Get references when needed
	auto* player = EntityManager::getPlayer("player");
	auto* prasanna = EntityManager::getNPC("prasanna");
	auto* soroor = EntityManager::getNPC("soroor");

	// Update the player
	player->update();

	// Update inventory UI (handles keyboard input for slot selection)
	Cashew::inventoryUI.update();

	// Check for interaction with Prasanna
	if (prasanna && Collision::collidedWith(
		player->getX(), player->getY(),
		prasanna->getX(), prasanna->getY(),
		75.0f,
		prasanna->getWidth(), prasanna->getHeight()
	)) {
		prasanna->speak(Cashew::dialogueBox);
	}

	// Check for interaction with Soroor
	if (soroor && Collision::collidedWith(
		player->getX(), player->getY(),
		soroor->getX(), soroor->getY(),
		75.0f,
		soroor->getWidth(), soroor->getHeight()
	)) {
		soroor->speak(Cashew::dialogueBox);
	}

	// Save map when pressing F5
	if (AEInputCheckTriggered(AEVK_F5)) {
		std::cout << "F5 pressed - Saving tile layout..." << std::endl;
		TileManager::saveTileLayout("Assets/Maps/testing.txt");
	}
}

void Testing_Draw() {
	// Draw the grid (use TileManager for consistency)
	TileManager::draw();

	// Draw all entities (automatically includes player and NPCs)
	EntityManager::drawAll();

	// Draw inventory UI
	Cashew::inventoryUI.draw();

	// Draw dialogue box if active
	Cashew::dialogueBox.draw();
}

void Testing_Free() {

}

void Testing_Unload() {
	EntityManager::clear();
	TileManager::exit(); // Use TileManager::exit() instead of World::exit()
}