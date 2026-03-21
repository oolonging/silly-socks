#include "../pch.h"
#include "TutorialScreen.hpp"

#include "../Graphics.hpp"
#include "../Entity.hpp"
#include "../UI_Elements/UI_Elements.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../Settings.hpp"

extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;

namespace TutorialScreen {

	// Testing UI Element dialogue box
	

	// test 1
	UI_Elements::DialogueBox testDialogue;

	AEGfxTexture* pTex;
}

void Tutorial_Load() {
	TutorialScreen::pTex = AEGfxTextureLoad("Assets/mycat.png");
}

void Tutorial_Initialize() {
	Settings::currentScreen = "TutorialScreen.cpp";
	
	// Non default constructor
	float dialogueWidth = 1200.0f;
	float dialogueHeight = 250.0f;
	float dialogueX = -dialogueWidth * 0.5f;
	float dialogueY = 0.0f;

	EntityManager::init();
	auto* tutPlayer = EntityManager::getPlayer("player");

	inv.setPlayer(EntityManager::getPlayer("player"));
	inv.loadInventory(tutPlayer, gameData);

	inv.setPlayer(tutPlayer);
	inv.loadInventory(tutPlayer, gameData);

	TutorialScreen::testDialogue = UI_Elements::DialogueBox(
		dialogueX, dialogueY, dialogueWidth, dialogueHeight,
		"Tutorial Guide", "Hey, you. You’re finally awake. You were trying to cross the border, right? Walked right into that Imperial ambush, same as us, and that thief over there",
		TutorialScreen::pTex, // No sprites available to test yet,
		Shapes::CORNER
	);

	// defaullt constructor

	// Activate it so it shows up
	TutorialScreen::testDialogue.activate();
}

void Tutorial_Update() {
	auto* tutPlayer = EntityManager::getPlayer("player");
	tutPlayer->update();
	inv.update();

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
	
	auto* tutPlayer = EntityManager::getPlayer("player");
	tutPlayer->draw();
	
	if (showInventory)
	{
		inv.draw();
	}
}

void Tutorial_Free() 
{
	auto* tutPlayer = EntityManager::getPlayer("player");
	inv.saveInventory(tutPlayer, gameData);
	inv.setPlayer(nullptr);
}

void Tutorial_Unload() {
	AEGfxTextureUnload(TutorialScreen::pTex);
	TutorialScreen::pTex = nullptr;
}


