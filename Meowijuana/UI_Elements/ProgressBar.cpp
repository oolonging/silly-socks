#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// ProgressBar Static Defaults
	// -------------------------------------------------------------------------
	ElementStyle ProgressBar::defaultstyle = UI_Element::getDefaultStyle();
	TextStyle ProgressBar::defaultTextStyle = UI_Element::getDefaultTextStyle();
	ElementTexture ProgressBar::defaultTexture = UI_Element::getDefaultTexture();

	// Definitions for default setters
	void ProgressBar::setDefaultProgressBarStyle(ElementStyle newStyle) { defaultstyle = newStyle; }
	void ProgressBar::setDefaultProgressBarTextStyle(TextStyle newStyle) { defaultTextStyle = newStyle; }
	void ProgressBar::setDefaultProgressBarTexxture(ElementTexture newTexture) { defaultTexture = newTexture; }

	// Definitions for default getters
	ElementStyle ProgressBar::getDefaultProgressBarStyle(void) {
		UI_Elements::ElementStyle defaultProgressBarStyle;
		// TODO: When color palette is done
		return defaultProgressBarStyle;
	}

	TextStyle ProgressBar::getDefaultProgressBarTextStyle(void) {
		UI_Elements::TextStyle defaultTextStyle;
		// TODO: when color palette is done
		return defaultTextStyle;
	}

	ElementTexture ProgressBar::getDefaultProgressBarTexture(void) {
		UI_Elements::ElementTexture defaultTexture;
		// TODO: when default textures for slider is done

		defaultTexture.primaryTexture = nullptr;
		defaultTexture.secondaryTexture = nullptr;

		return defaultTexture;
	}



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
