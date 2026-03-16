#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Graphics.hpp"

namespace Settings {
	///////////////////
	// Color settings
	///////////////////
	
	extern Color::Color gFillColor;
	extern Color::Color gStrokeColor;
	extern int gStrokeWeight;

	//////////////////
	// Text settings
	//////////////////
	
	extern Color::Color gTextFillColor;
	extern float gTextSize;
	extern int gDefaultTextSize;
	extern Text::TEXT_ALIGN_HORIZONTAL gTextAlignHorizontal;
	extern Text::TEXT_ALIGN_VERTICAL gTextAlignVertical;
	extern s8 gCurrentFontId;
	extern std::map<std::string, s8> gFonts;

	///////////
	// Sound
	///////////


	extern float gMusicVolume;
	extern float gSFXVolume;
	extern bool gMuteSFX;
	extern bool gMuteMusic;

	//////////////
	// Debug mode
	//////////////
	
	// Controls if debug menus and debug draws are rendered
	extern bool gDebugMode;

	// Call to switch debug mode on or off
	void toggleDebugMode(void);

	// Draw debug overlay
	void drawDebugOverlay(void);
}

#endif // SETTINGS_HPP

//TODO: missing
// grid size for world
// some other default values like sprites
