/**
 * @file Settings.hpp
 * @author Saahil
 * @brief Global game settings including color, text, sound, and debug configuration.
 */

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Graphics.hpp"
#include "UI_Elements/UI_Elements.hpp"

 /**
  * @namespace Settings
  * @brief Contains all global game settings and configuration variables.
  */
namespace Settings {
	///////////////////
	// Color settings
	///////////////////

	extern Color::Color gFillColor;			///< Global fill color.
	extern Color::Color gStrokeColor;		///< Global stroke (outline) color.
	extern int gStrokeWeight;				///< Global stroke weight in pixels.

	//////////////////
	// Text settings
	//////////////////

	extern Color::Color gTextFillColor;								///< Global text fill color.
	extern float gTextSize;											///< Current text size.
	extern int gDefaultTextSize;									///< Default text size.
	extern Text::TEXT_ALIGN_HORIZONTAL gTextAlignHorizontal;		///< Current horizontal text alignment.
	extern Text::TEXT_ALIGN_VERTICAL gTextAlignVertical;			///< Current vertical text alignment.
	extern s8 gCurrentFontId;										///< Currently active font ID.
	extern std::map<std::string, s8> gFonts;						///< Map of font names to font IDs.

	///////////
	// Sound
	///////////

	extern float gMusicVolume;		///< Global music volume (0.0f - 1.0f).
	extern float gSFXVolume;		///< Global SFX volume (0.0f - 1.0f).
	extern bool gMuteSFX;			///< Whether SFX is muted.
	extern bool gMuteMusic;			///< Whether music is muted.

	//////////////
	// Debug mode
	//////////////

	/// @brief Controls if debug menus and debug draws are rendered.
	extern bool gDebugMode;

	/**
	 * @brief Toggles debug mode on or off.
	 */
	void toggleDebugMode(void);

	/// @brief Name of the currently active screen.
	extern std::string currentScreen;

	/**
	 * @brief Draws the debug overlay with diagnostic information.
	 */
	void drawDebugOverlay(void);

	////////////////
	// Testing area
	////////////////

	extern bool gHoldingCarrot;		///< Whether the player is currently holding a carrot.

	/**
	 * @brief Draws the remaining lives indicator on screen.
	 */
	void drawLivesLeft(void);
}

#endif // SETTINGS_HPP
