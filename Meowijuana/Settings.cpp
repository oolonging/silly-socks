#include "Settings.hpp"

namespace Settings {
	// set default values

	// Color
	Color::Color gFillColor = { 255.0f, 255.0f, 255.0f, 255.0f };
	Color::Color gStrokeColor = { 0.0f, 0.0f, 0.0f, 255.0f };
	int gStrokeWeight = 1.0f;

	// Text
	Color::Color gTextFillColor = { 0.0f, 0.0f, 0.0f, 255.0f };
	float gTextSize = 1.0f;
	Text::TEXT_ALIGN_HORIZONTAL gTextAlignHorizontal = Text::LEFT;
	Text::TEXT_ALIGN_VERTICAL gTextAlignVertical = Text::TOP;
	s8 gCurrentFontId = -1; // No font selected

	std::map<std::string, s8> gFonts;
}
