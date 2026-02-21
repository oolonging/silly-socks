#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "AETypes.h"
#include <map>
#include <string>
#include "Graphics.hpp"

namespace Settings {
	// Color settings
	extern Color::Color gFillColor;
	extern Color::Color gStrokeColor;
	extern int gStrokeWeight;

	// Text settings
	extern Color::Color gTextFillColor;
	extern float gTextSize;
	extern Text::TEXT_ALIGN_HORIZONTAL gTextAlignHorizontal;
	extern Text::TEXT_ALIGN_VERTICAL gTextAlignVertical;
	extern s8 gCurrentFontId;

	// Font storage (fontName -> fontId)
	extern std::map<std::string, s8> gFonts;
}

#endif // SETTINGS_HPP

//TODO: missing
// debugmode toggle and bool
// grid size for world
// some other default values like sprites
