#include <iostream>

#include "testing-ui-elements.hpp"
#include "AEEngine.h"

// UI manager
#include "../../Managers/UIManager.hpp"

// Globals
float health = 50.0f;
float maxHealth = 100.0f;
float damageModifier = 5.0f;


// Create UI Elements
UI_Elements::Button* healButton;
UI_Elements::Button* hurtButton;

UI_Elements::ProgressBar* healthBar;


// Helper functions
void heal(void) {
	std::cout << "The heal function has been called" << std::endl;
	health += damageModifier;

	std::cout << "Health: " << health << std::endl;

	// texture debug
	if (healthBar->getTexture().primaryTexture == nullptr) {
		std::cout << "Health has no texture";
		printf("Health has no texture\n");
	}
}
void hurt(void) { health -= damageModifier; }


void testingUI_Load() {

}

void testingUI_Initialize() {
	// Initialize the buttons using the UI manager
	healButton = UIManager::create<UI_Elements::Button>("healButton", -750.0f, 350.0f, 100.0f, 50.0f, "Heal", Shapes::CORNER);
	hurtButton = UIManager::create<UI_Elements::Button>("hurtButton", -700.0f, 200.0f, 100.0f, 50.0f, "Hurt", Shapes::CENTER);

	// Set the buttons to hurt or heal depending on what it do
	healButton->setOnClick(heal);
	hurtButton->setOnClick(hurt);

	// Initialize the progress bar
	healthBar = UIManager::create<UI_Elements::ProgressBar>("healthBar", -550.0f, 350.0f, 500.0f, 50.0f, health, 0.0f, maxHealth, Shapes::CORNER);
}

void testingUI_Update() {

}

void testingUI_Draw() {
	// reset background
	Color::background(Color::Preset::White);

	// Draw the UI elements
	healthBar->setValue(health);
	UIManager::drawAll();
}

void testingUI_Free() {

}

void testingUI_Unload() {

}

