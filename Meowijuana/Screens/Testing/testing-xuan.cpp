#include "AEEngine.h"
#include "testing-xuan.hpp"
#include <iostream>
#include "../../GameStateManager.hpp"
#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../Managers/TileManager.hpp"


namespace TutorialScreen {

	Entity::NPC* activeSpeaker = nullptr;
	UI_Elements::DialogueBox dialogueBox;

}

void Xuan_Load() {


}

void Xuan_Initialize() {

	TileManager::init();

	// Load tile layout from file AFTER init
	TileManager::loadTileLayout("Assets/Maps/testing.txt");

	// Initialize entities
	EntityManager::init();

	// Initialize dialogue box
	TutorialScreen::dialogueBox = UI_Elements::DialogueBox(0.0f, 0.0f, 1000.0f, 200.0f, "", "", nullptr, Shapes::CENTER);
}


void Xuan_Update() {

	auto* player = EntityManager::getPlayer("player");
	auto* smelly = EntityManager::getNPC("smelly");

	// Update the player
	player->update();
	static Animations::Indicator smellind{ smelly->getX(), smelly->getY() };
	updateIndicator(smellind);


	if (smellind.active == 1) {
		Animations::drawIndicator(smellind);
	}

	if (!TutorialScreen::dialogueBox.getIsActive()) {
		TutorialScreen::activeSpeaker = nullptr;

		// Interaction
		if (Collision::collidedWith(
			player->getX(), player->getY(),
			smelly->getX(), smelly->getY(),
			75.0f,
			smelly->getWidth(), smelly->getHeight()
		)) {
			TutorialScreen::activeSpeaker = smelly;
			smelly->speak(TutorialScreen::dialogueBox);
			smellind.active = 0;

		}

	}


	else if (TutorialScreen::dialogueBox.getIsActive() && TutorialScreen::activeSpeaker) {
		TutorialScreen::activeSpeaker->speak(TutorialScreen::dialogueBox);
	}

}

void Xuan_Draw() {

	Color::background(Color::Preset::White);
	Color::fill(Color::Preset::Blue);

	// TileManager::draw();

	EntityManager::draw("smelly");
	EntityManager::draw("player");
	

	// Draw dialogue box if active
	TutorialScreen::dialogueBox.draw();
}

void Xuan_Free() {}

void Xuan_Unload() {
	EntityManager::clear();
	TileManager::exit();
}
