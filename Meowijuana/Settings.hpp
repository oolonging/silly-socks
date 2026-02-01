#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Graphics.hpp"

namespace Settings {
	// Debug
	static bool debugMode = false;

	// colors
	static Color::CL_Color gFillColor{255, 255, 255, 255};
	static Color::CL_Color textFillColor{0, 0, 0, 255};

	static Color::CL_Color gStrokeColor{0, 0, 0, 255};
	static float gStrokeWeight = 5.0f;

	// Text
	static int8_t pCurrentFont = 0;

	// World
	static int gridSize = 50;

	// Switches
	void toggleDebugMode(void);
}


#endif // SETTINGS_HPP