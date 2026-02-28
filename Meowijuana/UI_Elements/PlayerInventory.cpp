#include "UI_Elements.hpp"
#include "../Entity.hpp"

// PlayerInventory implementation
UI_Elements::PlayerInventory::PlayerInventory(float x, float y, float slotSize, float slotSpacing, Entity::Player* player, Shapes::SHAPE_MODE mode)
	: UI_Element(x, y, slotSize * 9 + slotSpacing * 8, slotSize, mode),
	playerRef(player),
	slotCount(9),
	slotSize(slotSize),
	slotSpacing(slotSpacing),
	selectedSlot(0) {

	// Set default selected slot color (bright yellow/gold for visibility)
	selectedColor = { 255.0f, 255.0f, 0.0f, 255.0f };
	selectedBorderThickness = 4.0f;

	// Set default style for inventory slots
	style.primaryColor = { 51.0f, 51.0f, 51.0f, 204.0f }; // Dark semi-transparent background
	style.secondaryColor = { 102.0f, 102.0f, 102.0f, 255.0f }; // Lighter gray for items
	style.strokeColor = { 255.0f, 255.0f, 255.0f, 255.0f }; // White border
	style.strokeWeight = 2;
}

UI_Elements::PlayerInventory::PlayerInventory(void)
	: UI_Element(),
	playerRef(nullptr),
	slotCount(9),
	slotSize(50.0f),
	slotSpacing(10.0f),
	selectedSlot(0) {

	selectedColor = { 255.0f, 255.0f, 0.0f, 255.0f };
	selectedBorderThickness = 4.0f;

	style.primaryColor = { 51.0f, 51.0f, 51.0f, 204.0f };
	style.secondaryColor = { 102.0f, 102.0f, 102.0f, 255.0f };
	style.strokeColor = { 255.0f, 255.0f, 255.0f, 255.0f };
	style.strokeWeight = 2;
}

void UI_Elements::PlayerInventory::handleKeyInput() {
	// Check keys 1-9 for slot selection
	if (AEInputCheckTriggered(AEVK_1)) setSelectedSlot(0);
	else if (AEInputCheckTriggered(AEVK_2)) setSelectedSlot(1);
	else if (AEInputCheckTriggered(AEVK_3)) setSelectedSlot(2);
	else if (AEInputCheckTriggered(AEVK_4)) setSelectedSlot(3);
	else if (AEInputCheckTriggered(AEVK_5)) setSelectedSlot(4);
	else if (AEInputCheckTriggered(AEVK_6)) setSelectedSlot(5);
	else if (AEInputCheckTriggered(AEVK_7)) setSelectedSlot(6);
	else if (AEInputCheckTriggered(AEVK_8)) setSelectedSlot(7);
	else if (AEInputCheckTriggered(AEVK_9)) setSelectedSlot(8);
}

void UI_Elements::PlayerInventory::drawSlot(int slotIndex, float slotX, float slotY) {
	// Set fill color for slot background
	Color::fill(style.primaryColor);

	// Set stroke color and weight for slot border
	Color::stroke(style.strokeColor);
	Color::strokeWeight(style.strokeWeight);

	// Draw the slot box
	Shapes::rect(slotX, slotY, slotSize, slotSize, drawMode);

	// TODO: Draw item icon if slot has an item
	// if (playerRef && slotIndex < playerRef->getInventorySize()) {
	//     Inventory::Item* item = playerRef->getInventoryItem(slotIndex);
	//     if (item != nullptr) {
	//         // Draw item icon/sprite here
	//         // You'll need to add texture/icon support to your Item class
	//     }
	// }
}

void UI_Elements::PlayerInventory::drawSelectedIndicator(float slotX, float slotY) {
	// Set stroke color for selected slot border
	Color::stroke(selectedColor);
	Color::strokeWeight(static_cast<int>(selectedBorderThickness));

	// Don't fill the selected indicator, only draw border
	Color::noFill();

	// Draw a highlighted border around the selected slot
	Shapes::rect(slotX, slotY, slotSize, slotSize, drawMode);
}

void UI_Elements::PlayerInventory::draw(void) {
	float currentX = x;
	float currentY = y;

	// Draw all 9 inventory slots
	for (int i = 0; i < slotCount; ++i) {
		drawSlot(i, currentX, currentY);

		// Draw selection indicator if this is the selected slot
		if (i == selectedSlot) {
			drawSelectedIndicator(currentX, currentY);
		}

		// Move to next slot position
		currentX += slotSize + slotSpacing;
	}
}

void UI_Elements::PlayerInventory::update() {
	handleKeyInput();
}

int UI_Elements::PlayerInventory::getSelectedSlot() const {
	return selectedSlot;
}

void UI_Elements::PlayerInventory::setSelectedSlot(int slot) {
	if (slot >= 0 && slot < slotCount) {
		selectedSlot = slot;

		// TODO: Update the player's selectedInventorySlot
		// You'll need to add this field to your Player class
		// if (playerRef) {
		//     playerRef->setSelectedInventorySlot(selectedSlot);
		// }
	}
}

void UI_Elements::PlayerInventory::setPlayer(Entity::Player* player) {
	playerRef = player;
}