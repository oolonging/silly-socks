#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// ProgressBar Implementation
	// -------------------------------------------------------------------------

	ProgressBar::ProgressBar(float x, float y, float width, float height, float valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), value(valRef), minValue(minVal), maxValue(maxVal) {
	}

	ProgressBar::ProgressBar(void)
		: UI_Element(), value(0), minValue(0), maxValue(100) {
	}

	void ProgressBar::clampValue(void) {
		if (value < minValue) value = minValue;
		if (value > maxValue) value = maxValue;
	}

	void ProgressBar::setValue(float newValue) {
		value = newValue;
	}

	void ProgressBar::draw(void) {
		float filledWidth = (value / maxValue) * width;

		// Apply stroke
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		// Background
		Color::fill(style.primaryColor);
		Shapes::rect(x, y, width, height, drawMode);

		// Foreground
		Color::fill(style.secondaryColor);
		Shapes::rect(x, y, filledWidth, height, drawMode);
	}
}
