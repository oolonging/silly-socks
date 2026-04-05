/**
 * @file Settings.cpp
 * @author Saahil
 * @brief Global game settings including color, text, sound, and debug configuration.
 */

#include "pch.h"
#include "Settings.hpp"

#include "InputManager.hpp"
#include "Managers/UIManager.hpp"
#include "Managers/EntityManager.hpp"
#include "Managers/ParticleManager.hpp"
#include "TutorialDungeon.hpp"

namespace Settings {
	//////////////////////
	// Set default values
	//////////////////////

	Color::Color gFillColor = Color::Preset::White;
	Color::Color gStrokeColor = Color::Preset::Black;
	int gStrokeWeight = 1;

	/////////////////
	// Text defaults
	/////////////////
	
	Color::Color gTextFillColor = Color::Preset::Black;
	float gTextSize = 1.0f;
	int gDefaultTextSize = 24;
	Text::TEXT_ALIGN_HORIZONTAL gTextAlignHorizontal = Text::LEFT;
	Text::TEXT_ALIGN_VERTICAL gTextAlignVertical = Text::TOP;
	s8 gCurrentFontId = -1; // No font selected
	std::map<std::string, s8> gFonts;

	///////////
	// Sound
	///////////

	float gMusicVolume = 100.0f;
	float gSFXVolume = 140.0f;
	bool gMuteSFX = false;
	bool gMuteMusic = false;

	///////////////
	// Debug mode
	///////////////
	
	bool gDebugMode = false; // Defaults to off, change it here for now if the toggle doesnt work

	void toggleDebugMode(void) {
		gDebugMode = !gDebugMode;
		printf("Debug mode is now ");
		if (gDebugMode) printf("on\n");
		else printf("off\n");
	}

	std::string currentScreen = "Splash"; // For the actual release, this is what the game will start with

	void drawDebugOverlay(void) {
		std::string output[] = {
			"FPS: " + std::to_string(AEFrameRateControllerGetFrameRate()),
			"Mouse Position: (" + std::to_string(Input::getMouseX()) + ", " + std::to_string(Input::getMouseY()) + ")",
			"UI Elements: " + std::to_string(UIManager::elements.size()),
			"Active Entities: " + std::to_string(EntityManager::entities.size()),
			"Active particles: " + std::to_string(gParticles.getActiveParticleCount()) + "/" + std::to_string(gParticles.getMaxParticles()),
			"Current room: " + currentScreen,
			"Player lives: " + std::to_string(3 - Death::deathCounter) + "/3"
		};
		int counter = 0;
		
		// Save previous text style
		Color::Color oldFill = gFillColor;
		Color::Color oldTextFill = gTextFillColor;
		float oldTextSize = gTextSize;
		Text::TEXT_ALIGN_HORIZONTAL oldAlignH = gTextAlignHorizontal;
		Text::TEXT_ALIGN_VERTICAL oldAlignV = gTextAlignVertical;

		// Draw text
		Color::textFill(Color::Preset::White);
		Text::textSize(20.0f);
		Text::textAlign(Text::LEFT, Text::BOTTOM);
		
		// transparent background for text
		for(auto line : output) {
			Text::text(line.c_str(), -790.0f, 440.0f - (20.0f * counter)); // TODO: make this more dynamic and less hard coded
			counter++;
		}

		// Reset to previous text style
		gFillColor = oldFill;
		gTextFillColor = oldTextFill;
		Text::textSize(oldTextSize);
		Text::textAlign(oldAlignH, oldAlignV);
	}

	///// Testing area
	bool gHoldingCarrot = false;

	void drawLivesLeft(void) {
		// black box top right
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		Color::fill(Color::Preset::Black);
		Shapes::rect(800.0f - 125.0f, 450.0f - 25.0f, -250.0f, -50.0f, Shapes::CENTER);
		std::string livesLeft = "Lives left: ";

		Text::textSize(15.0f);
		for (int i = 0; i < 3; i++) {
			if (i >= Death::deathCounter) livesLeft += "+";
		}

		Color::textFill(Color::Preset::White);
		Text::text(livesLeft.c_str(), 800.0f - 125.0f, 450.0f - 25.0f, Text::CENTER_H, Text::CENTER_V);
	}
}


