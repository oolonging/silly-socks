#include "Settings.hpp"

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

	void drawDebugOverlay(void) {
		// TODO: implement
	}
}
