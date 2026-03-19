#include "../../pch.h"
#include "testing-sprites.hpp"

#include "../../Graphics.hpp"
#include "../../Managers/SpriteManager.hpp"


// Animations


void testingSprites_Load() {
	// Initialize sprite manager
	SpriteManager::init();

	// Load the cat sprite sheet
	SpriteManager::loadSpriteSheet("cat", "Assets/Images/SpriteSheets/cat_spritesheet.png", 384.0f, 256.0f, 32.0f, 32.0f);

	// Load the cat walk animation
	SpriteManager::createAnimationFromRow("catWalk", "cat", 4, 3, 3, 0.21f, true);

}

void testingSprites_Initialize() {
											
}

void testingSprites_Update() {
	SpriteManager::updateAnimationTime(AEFrameRateControllerGetFrameTime());

	// just a line from one point to another

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::fill(Color::Preset::Black);
	Color::strokeWeight(5);
	//Shapes::line(-100.0f, -100.0f, 100.0f, 100.0f);
	//Shapes::line(-100.0f, 100.0f, 100.0f, -100.0f);
}

void testingSprites_Draw() {
	SpriteManager::drawAnimation("catWalk", 0.0f, 0.0f, 128.0f, 128.0f);


}

void testingSprites_Free() {

}

void testingSprites_Unload() {
	SpriteManager::clear();
}


