#include "../pch.h"
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
	// Set render mode to color
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// Set fill color for slot background
	Color::fill(style.primaryColor);

	// Set stroke color and weight for slot border
	Color::stroke(style.strokeColor);
	Color::strokeWeight(style.strokeWeight);

	// Draw the slot box
	Shapes::rect(slotX, slotY, slotSize, slotSize, drawMode);

	// Draw slot number above the slot
	Color::textFill(255, 255, 255);
	Text::text(std::to_string(slotIndex + 1).c_str(),
		slotX + slotSize / 2.0f,
		slotY + slotSize * 0.5f); // above the slot

	// Draw item icon if slot has an item
	if (playerRef && slotIndex < playerRef->getInventorySize()) {
		Inventory::Item* item = playerRef->getInventoryItem(slotIndex);
		if (item != nullptr && item->getIcon() != nullptr) {
			// Set up transformation matrix for icon
			AEMtx33 transform{};

			float iconX = slotX;
			float iconY = slotY;

			// Adjust position based on draw mode
			if (drawMode == Shapes::CORNER) {
				iconX += slotSize / 2.0f;
				iconY -= slotSize / 2.0f;
			}

			AEMtx33Scale(&transform, slotSize * 0.8f, slotSize * 0.8f); // Scale icon to 80% of slot size
			AEMtx33TransApply(&transform, &transform, iconX, iconY);

			// Draw the icon texture
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTransform(transform.m);
			//AEGfxSetTintColor(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxTextureSet(item->getIcon(), 0, 0);
			//AEGfxMeshDraw(Shapes::getSquareMesh(), AE_GFX_MDM_TRIANGLES);
			Graphics::image(iconX, iconY, slotSize * 0.8f, slotSize * 0.8f, item->getIcon(), Shapes::CENTER);

			// Reset state after texture is drawn
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			AEGfxTextureSet(nullptr, 0, 0);
			//TODO: this is the one i had to modify
		}

		// Draw count in bottom right of slot
		if (item != nullptr && item->getCount() >= 1)
		{
			Color::textFill(255, 255, 255);
			Text::text(std::to_string(item->getCount()).c_str(),
				slotX + slotSize * 0.6f,
				slotY - slotSize * 0.7f);
		}
	}
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

void UI_Elements::PlayerInventory::update(Entity::Player* player) {
	handleKeyInput();

	int currentSlot = this->getSelectedSlot();
	Inventory::Item* currentItem = player->getInventoryItem(currentSlot);

	if (currentItem != nullptr)
	{
		if (currentItem->getID() == Inventory::ItemID::CARROT_SWORD_INV)
		{
			player->weaponCheck(true);
		}
		else
		{
			player->weaponCheck(false);
		}
	}
	else
	{
		player->weaponCheck(false); // no item in slot, no weapon
	}
}

int UI_Elements::PlayerInventory::getSelectedSlot() const {
	return selectedSlot;
}

void UI_Elements::PlayerInventory::setSelectedSlot(int slot) {
	if (slot >= 0 && slot < slotCount) {
		selectedSlot = slot;

		if (playerRef)
		{
			playerRef->setSelectedInventorySlot(selectedSlot);
		}
	}
}

void UI_Elements::PlayerInventory::setPlayer(Entity::Player* player) {
	playerRef = player;
}

void UI_Elements::PlayerInventory::setPosition(float newX, float newY) {
	x = newX;
	y = newY;
}

UI_Elements::PlayerInventory::~PlayerInventory() {
	playerRef = nullptr;
}

void UI_Elements::PlayerInventory::giveItem(Entity::Player& player, int itemID, int itemCount)
{
	for (int i = 0; i < player.getInventorySize(); i++)
	{
		if (player.getInventoryItem(i) != nullptr)
		{
			if (player.getInventoryItem(i)->getID() == itemID)
			{
				int count = player.getInventoryItem(i)->getCount();
				player.getInventoryItem(i)->setCount(count += itemCount);
				return;
			}
		}

		else
		{
			Inventory::Item* item = Inventory::ItemRegistry::createItem(itemID);
			item->setCount(itemCount);
			player.setInventoryItem(i, item);
			return;
		}
	}
}

bool UI_Elements::PlayerInventory::findItem(Entity::Player& player, int check)
{
	for (int i = 0; i < player.getInventorySize(); i++)
	{
		Inventory::Item* item = player.getInventoryItem(i);
		if (item != nullptr && item->getID() == check)
		{
			return true; 
		}
	}
	return false;
}

int UI_Elements::PlayerInventory::findItemCount(Entity::Player& player, int check)
{
	int count = 0;

	for (int i = 0; i < player.getInventorySize(); i++)
	{
		Inventory::Item* item = player.getInventoryItem(i);
		if (item != nullptr && item->getID() == check)
		{
			count = item->getCount();
			return count;
		}
	}
	return count;
}

bool UI_Elements::PlayerInventory::isEmpty(Entity::Player& player)
{
	for (int i = 0; i < player.getInventorySize(); i++)
	{
		Inventory::Item* item = player.getInventoryItem(i);
		if (item != nullptr)
		{
			return false;
		}
	}
	return true;
}

void UI_Elements::PlayerInventory::clear(Entity::Player* player)
{
	for (int i = 0; i < this->slotCount; i++)
	{
		player->clearInventorySlot(i);
	}
}

void UI_Elements::PlayerInventory::saveInventory(Entity::Player* player, GameData& gameData)
{
	if (!player) return;

	// clear old saved data first 
	gameData.inventory.clear();
	gameData.selectedSlot = player->getSelectedInventorySlot();

	for (int i = 0; i < player->getInventorySize(); i++)
	{
		Inventory::Item* item = player->getInventoryItem(i);

		if (item)
		{
			gameData.inventory.push_back({ item->getID(), item->getCount() });
		}
		else
		{
			gameData.inventory.push_back({ -1, 0 }); // empty slot
		}
	}
}

void UI_Elements::PlayerInventory::loadInventory(Entity::Player* player, GameData& gameData)
{
	player->setSelectedInventorySlot(gameData.selectedSlot);

	if (!player) return;

	if (gameData.inventory.empty()) return;

	// clear player's current inventory first
	for (int i = 0; i < player->getInventorySize(); i++)
	{
		player->setInventoryItem(i, nullptr);
	}

	// restore items
	for (int i = 0; i < gameData.inventory.size(); i++)
	{
		int id = gameData.inventory[i].first;
		int count = gameData.inventory[i].second;

		if (id < 0) continue; // skip empty slots

		Inventory::Item* item = Inventory::ItemRegistry::createItem(id);
		if (!item) continue;

		item->setCount(count);
		player->setInventoryItem(i, item);
	}
}

