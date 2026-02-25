#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
// -------------------------------------------------------------------------
// Checkbox Implementation
// -------------------------------------------------------------------------

Checkbox::Checkbox(float x, float y, float boxSize, char const* label, bool initialState, Shapes::SHAPE_MODE mode)
	: UI_Element(x, y, boxSize + 100, boxSize, mode), isChecked(initialState), label(label), boxSize(boxSize), onChange(nullptr) {
	// Custom default style for checkbox
	style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
	style.secondaryColor = Color::createColorRGB(100, 200, 100, 255);
	style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
	style.strokeWeight = 2;
}

Checkbox::Checkbox(void)
	: UI_Element(), isChecked(false), label("Checkbox"), boxSize(20), onChange(nullptr) {
	style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
	style.secondaryColor = Color::createColorRGB(100, 200, 100, 255);
	style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
	style.strokeWeight = 2;
}

void Checkbox::toggle() {
	isChecked = !isChecked;
	if (onChange != nullptr) {
		onChange(isChecked);
	}
}

bool Checkbox::getChecked() const {
	return isChecked;
}

void Checkbox::setChecked(bool checked) {
	if (isChecked != checked) {
		isChecked = checked;
		if (onChange != nullptr) {
			onChange(isChecked);
		}
	}
}

void Checkbox::setOnChange(void (*func)(bool)) {
	onChange = func;
}

void Checkbox::draw(void) {
	// Check if checkbox area is hovered (just the box, not the label)
	int32_t mx = 0, my = 0;
	AEInputGetCursorPosition(&mx, &my);
	int32_t ww = AEGfxGetWindowWidth();
	int32_t wh = AEGfxGetWindowHeight();
	float worldX = mx - ww * 0.5f;
	float worldY = wh * 0.5f - my;

	bool boxHovered = false;
	if (drawMode == Shapes::CORNER) {
		boxHovered = ((worldX >= x) && (worldX <= (x + boxSize)) &&
			(worldY <= y) && (worldY >= (y - boxSize)));
	}
	else {
		boxHovered = ((worldX >= (x - boxSize / 2)) && (worldX <= (x + boxSize / 2)) &&
			(worldY >= (y - boxSize / 2)) && (worldY <= (y + boxSize / 2)));
	}

	// Handle click
	if (boxHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
		toggle();
	}

	// Draw checkbox box
	Color::stroke(style.strokeColor);
	Color::strokeWeight(style.strokeWeight);

	if (isChecked) {
		Color::fill(style.secondaryColor);
	}
	else {
		Color::fill(style.primaryColor);
	}

	Shapes::rect(x, y, boxSize, boxSize, drawMode);

	// Draw checkmark if checked
	if (isChecked) {
		Color::noStroke();
		Color::fill(0, 0, 0, 255);
		float checkX = (drawMode == Shapes::CORNER) ? (x + boxSize / 2) : x;
		float checkY = (drawMode == Shapes::CORNER) ? (y - boxSize / 2) : y;
		Shapes::rect(checkX - boxSize * 0.15f, checkY, boxSize * 0.3f, boxSize * 0.3f, Shapes::CENTER);
	}

	// Draw label
	Color::textFill(0, 0, 0, 255);
	float labelX = (drawMode == Shapes::CORNER) ? (x + boxSize + 10) : (x + boxSize / 2 + 10);
	float labelY = (drawMode == Shapes::CORNER) ? (y - boxSize / 2) : y;
	Text::text(label, labelX, labelY);
}
}
