#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "Graphics.hpp"

namespace Settings {
	// Debug
	static bool debugMode = false;

	// colors
	static Color::CL_Color gFillColor{};
	static Color::CL_Color gStrokeColor{};

	// World
	static int gridSize = 50;
}


#endif // SETTINGS_HPP