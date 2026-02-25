#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
// -------------------------------------------------------------------------
// RadioButton Implementation
// -------------------------------------------------------------------------

RadioButton::RadioButton(float x, float y, float circleSize, char const* label, int groupId, bool initialState, Shapes::SHAPE_MODE mode)
	: UI_Element(x, y, circleSize + 100, circleSize, mode), isSelected(initialState), label(label),
	circleSize(circleSize), groupId(groupId), onSelect(nullptr) {
	// Custom default style for radio button
	style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
	style.secondaryColor = Color::createColorRGB(100, 150, 255, 255);
	style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
	style.strokeWeight = 2;

	// Add to radio group
	if (groupId >= 0 && groupId < 10) {
		radioGroups[groupId].push_back(this);
	}
}

RadioButton::RadioButton(void)
	: UI_Element(), isSelected(false), label("Radio"), circleSize(20), groupId(0), onSelect(nullptr) {
	style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
	style.secondaryColor = Color::createColorRGB(100, 150, 255, 255);
	style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
	style.strokeWeight = 2;

	radioGroups[0].push_back(this);
}

RadioButton::~RadioButton() {
	// Remove from radio group
	if (groupId >= 0 && groupId < 10) {
		auto& group = radioGroups[groupId];
		group.erase(std::remove(group.begin(), group.end(), this), group.end());
	}
}

void RadioButton::select() {
	if (!isSelected) {
		// Deselect all other radio buttons in the same group
		if (groupId >= 0 && groupId < 10) {
			for (RadioButton* radio : radioGroups[groupId]) {
				if (radio != this) {
					radio->isSelected = false;
				}
			}
		}

		isSelected = true;
		if (onSelect != nullptr) {
			onSelect();
		}
	}
}

bool RadioButton::getSelected() const {
	return isSelected;
}

void RadioButton::setOnSelect(void (*func)(void)) {
	onSelect = func;
}

void RadioButton::draw(void) {
	// Check if radio button area is hovered (just the circle, not the label)
	int32_t mx = 0, my = 0;
	AEInputGetCursorPosition(&mx, &my);
	int32_t ww = AEGfxGetWindowWidth();
	int32_t wh = AEGfxGetWindowHeight();
	float worldX = mx - ww * 0.5f;
	float worldY = wh * 0.5f - my;

	bool circleHovered = false;
	if (drawMode == Shapes::CORNER) {
		float centerX = x + circleSize / 2;
		float centerY = y - circleSize / 2;
		float dist = sqrtf((worldX - centerX) * (worldX - centerX) +
			(worldY - centerY) * (worldY - centerY));
		circleHovered = (dist <= circleSize / 2);
	}
	else {
		float dist = sqrtf((worldX - x) * (worldX - x) + (worldY - y) * (worldY - y));
		circleHovered = (dist <= circleSize / 2);
	}

	// Handle click
	if (circleHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
		select();
	}

	// Draw radio button circle
	Color::stroke(style.strokeColor);
	Color::strokeWeight(style.strokeWeight);
	Color::fill(style.primaryColor);

	if (drawMode == Shapes::CORNER) {
		Shapes::circle(x + circleSize / 2, y - circleSize / 2, circleSize, Shapes::CENTER);
	}
	else {
		Shapes::circle(x, y, circleSize, Shapes::CENTER);
	}

	// Draw inner circle if selected
	if (isSelected) {
		Color::noStroke();
		Color::fill(style.secondaryColor);
		if (drawMode == Shapes::CORNER) {
			Shapes::circle(x + circleSize / 2, y - circleSize / 2, circleSize * 0.5f, Shapes::CENTER);
		}
		else {
			Shapes::circle(x, y, circleSize * 0.5f, Shapes::CENTER);
		}
	}

	// Draw label
	Color::textFill(0, 0, 0, 255);
	float labelX = (drawMode == Shapes::CORNER) ? (x + circleSize + 10) : (x + circleSize / 2 + 10);
	float labelY = (drawMode == Shapes::CORNER) ? (y - circleSize / 2) : y;
	Text::text(label, labelX, labelY);
}
}
