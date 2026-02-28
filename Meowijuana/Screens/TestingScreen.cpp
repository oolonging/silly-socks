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

	// Item textures
	AEGfxTexture* swordIcon = nullptr;
	AEGfxTexture* grenadeIcon = nullptr;
	AEGfxTexture* bombIcon = nullptr;

	// Items
	Inventory::Item* sword = nullptr;
	Inventory::Item* grenade = nullptr;
	Inventory::Item* bomb = nullptr;
}

void Testing_Load() {

	// Load icons (temporary while i test)
	Cashew::swordIcon = AEGfxTextureLoad("Assets/Images/Items/sword.png");
	Cashew::grenadeIcon = AEGfxTextureLoad("Assets/Images/Items/grenade.png");
	Cashew::bombIcon = AEGfxTextureLoad("Assets/Images/Items/bomb.png");

}

void Testing_Initialize() {
	// Initialize tile system (this calls World::init() internally)
	TileManager::init();

	// Load tile layout from file AFTER init
	TileManager::loadTileLayout("Assets/Maps/testing.txt");

	// Initialize entities
	EntityManager::init();

	// Create items with icons
	Cashew::sword = new Inventory::Item(1, "Sword", "A sharp blade", 10.0f, Cashew::swordIcon);
	Cashew::grenade = new Inventory::Item(2, "Grenade", "Explosive device", 25.0f, Cashew::grenadeIcon);
	Cashew::bomb = new Inventory::Item(3, "Bomb", "Big boom!", 50.0f, Cashew::bombIcon);

	// Get player and add items to inventory
	auto* player = EntityManager::getPlayer("player");
	if (player) {
		player->setInventoryItem(0, Cashew::sword);
		player->setInventoryItem(1, Cashew::grenade);
		player->setInventoryItem(2, Cashew::bomb);
		player->setInventoryItem(4, Cashew::sword);  // Another sword in slot 5
		player->setInventoryItem(7, Cashew::bomb);   // Another bomb in slot 8

		// Initialize inventory UI (bottom center of screen)
		Cashew::inventoryUI = UI_Elements::PlayerInventory(
			-250.0f,  // x position (adjust to center on your screen)
			-350.0f,  // y position (near bottom of screen)
			50.0f,    // slot size
			10.0f,    // spacing between slots
			player,   // player reference
			Shapes::CENTER
		);

		// style the inventory UI
		// comment out if its ugly
		Cashew::inventoryUI.setStyle({
			{ 51.0f, 51.0f, 51.0f, 204.0f }, // primaryColor - dark gray
			{ 102.0f, 102.0f, 102.0f, 255.0f }, // secondaryColor - lighter gray
			{ 255.0f, 255.0f, 255.0f, 255.0f }, // strokeColor - white
			2 // strokeWeight
			});
	}

	// Initialize dialogue box
	Cashew::dialogueBox = UI_Elements::DialogueBox(0.0f, 0.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);
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
	// Clean up items
	delete Cashew::sword;
	delete Cashew::grenade;
	delete Cashew::bomb;
}

void Testing_Unload() {
	EntityManager::clear();
	TileManager::exit(); // Use TileManager::exit() instead of World::exit()

	// Unload textures
	if (Cashew::swordIcon) AEGfxTextureUnload(Cashew::swordIcon);
	if (Cashew::grenadeIcon) AEGfxTextureUnload(Cashew::grenadeIcon);
	if (Cashew::bombIcon) AEGfxTextureUnload(Cashew::bombIcon);

}