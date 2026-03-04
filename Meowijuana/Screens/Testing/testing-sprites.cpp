#include "testing-sprites.hpp"
#include "AEEngine.h"

#include "../../Managers/SpriteManager.hpp"



// Sprites
SpriteManager::Sprite whiteCat{};
SpriteManager::Sprite brownCat{};

void testingSprites_Load() {
	// Initialize sprite manager
	SpriteManager::init();

	// Load the cat sprite sheet
	SpriteManager::loadSpriteSheet("cat", "Assets/Images/SpriteSheets/cat_spritesheet.png", 384.0f, 256.0f, 32.0f, 32.0f);

}

void testingSprites_Initialize() {

	whiteCat = SpriteManager::createSprite("cat", 1, 0);
	brownCat = SpriteManager::createSprite("cat", 4, 0);
}

void testingSprites_Update() {

}

void testingSprites_Draw() {

	// Draw the sprites
	SpriteManager::drawSprite(whiteCat, 0.0f, 0.0f, 32.0f, 32.0f);
	SpriteManager::drawSprite(brownCat, 50.0f, 50.0f, 32.0f, 32.0f);

}

void testingSprites_Free() {

}

void testingSprites_Unload() {
	SpriteManager::clear();
}
