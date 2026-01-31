#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Graphics.hpp"

namespace Settings {
	// Debug
	static bool debugMode = false;

	// colors
	static Color::CL_Color gFillColor{};
	static Color::CL_Color gStrokeColor{};
	static Color::CL_Color textFillColor{};

	// Text
	static int8_t pCurrentFont = 0;

	// World
	static int gridSize = 50;

	// Switches
	void toggleDebugMode(void);
}


#endif // SETTINGS_HPP