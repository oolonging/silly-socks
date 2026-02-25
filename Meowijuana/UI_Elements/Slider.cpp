#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
// -------------------------------------------------------------------------
// Slider Implementation
// -------------------------------------------------------------------------

Slider::Slider(float x, float y, float width, float height, float& valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
	: UI_Element(x, y, width, height, mode), valueRef(valRef), minValue(minVal), maxValue(maxVal) {
}

// TODO: ensure that the removal of the default constructor didnt break anything (for slider UI element)

void Slider::clampValue(void) {
	if (valueRef < minValue) valueRef = minValue;
	if (valueRef > maxValue) valueRef = maxValue;
}

void Slider::updateValue(void) {
	if (isHovering() && AEInputCheckCurr(AEVK_LBUTTON)) {
		int32_t mx = 0, my = 0;
		AEInputGetCursorPosition(&mx, &my);
		int32_t ww = AEGfxGetWindowWidth();
		float worldX = mx - ww * 0.5f;

		float relativeX = (drawMode == Shapes::CENTER)
			? (worldX - x + width / 2)
			: (worldX - x);

		float newValue = (relativeX / width) * (maxValue - minValue) + minValue;
		valueRef = newValue;
	}
}

void Slider::draw(void) {
	updateValue();
	clampValue();

	float filledWidth = ((valueRef - minValue) / (maxValue - minValue)) * width;

	// Apply stroke
	Color::stroke(style.strokeColor);
	Color::strokeWeight(style.strokeWeight);

	// Background
	Color::fill(style.primaryColor);
	Shapes::rect(x, y, width, height, drawMode);

	// Foreground
	if (isHovering() && AEInputCheckCurr(AEVK_LBUTTON)) {
		Color::fill(Color::createColorRGB(0, 255, 0, 255));
	}
	else {
		Color::fill(style.secondaryColor);
	}
	Shapes::rect(x, y, filledWidth, height, drawMode);
}
}
