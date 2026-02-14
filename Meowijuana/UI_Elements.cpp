#include "UI_Elements.hpp"
#include "AEEngine.h"

namespace UI_Elements {

	// -------------------------------------------------------------------------
	// UI_Element Base Class Implementation
	// -------------------------------------------------------------------------

	ElementStyle UI_Element::getDefaultStyle() {
		ElementStyle defaultStyle;
		defaultStyle.primaryColor = Color::CL_Color_Create(200, 200, 200, 255);
		defaultStyle.secondaryColor = Color::CL_Color_Create(100, 150, 255, 255);
		defaultStyle.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
		defaultStyle.strokeWeight = 2.0f;
		return defaultStyle;
	}

	UI_Element::UI_Element(float x, float y, float width, float height, Shapes::SHAPE_MODE mode)
		: x(x), y(y), width(width), height(height), drawMode(mode), style(getDefaultStyle()) {
	}

	UI_Element::UI_Element()
		: x(0), y(0), width(100), height(50), drawMode(Shapes::CORNER), style(getDefaultStyle()) {
	}

	bool UI_Element::isHovering(void) const {
		int32_t mx = 0, my = 0;
		AEInputGetCursorPosition(&mx, &my);

		int32_t ww = AEGfxGetWindowWidth();
		int32_t wh = AEGfxGetWindowHeight();
		float worldX = mx - ww * 0.5f;
		float worldY = wh * 0.5f - my;

		bool xOverlap{}, yOverlap{};

		if (drawMode == Shapes::CORNER) {
			xOverlap = ((worldX >= x) && (worldX <= (x + width)));
			yOverlap = ((worldY <= y) && (worldY >= (y - height)));
		}
		else if (drawMode == Shapes::CENTER) {
			xOverlap = ((worldX >= (x - width / 2)) && (worldX <= (x + width / 2)));
			yOverlap = ((worldY >= (y - height / 2)) && (worldY <= (y + height / 2)));
		}

		return (xOverlap && yOverlap);
	}

	void UI_Element::setStyle(ElementStyle newStyle) {
		style = newStyle;
	}

	ElementStyle UI_Element::getStyle(void) const {
		return style;
	}

	// -------------------------------------------------------------------------
	// Button Implementation
	// -------------------------------------------------------------------------

	Button::Button(float x, float y, float width, float height, char const* msg, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), msg(msg), onClick(nullptr) {
	}

	Button::Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode)
		: UI_Element(quad.position.x, quad.position.y, quad.width, quad.height, mode), msg(msg), onClick(nullptr) {
	}

	Button::Button(void)
		: UI_Element(), msg("Button"), onClick(nullptr) {
	}

	void Button::setOnClick(void (*func)(void)) {
		onClick = func;
	}

	void Button::draw(void) {
		bool hovering = isHovering();

		// Apply stroke
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		// Handle click
		if (hovering) {
			if (AEInputCheckTriggered(AEVK_LBUTTON) && onClick != nullptr) {
				onClick();
			}
			Color::fill(style.secondaryColor);
		}
		else {
			Color::fill(style.primaryColor);
		}

		Shapes::rect(x, y, width, height, drawMode);

		// Draw text
		Color::textFill(0, 0, 0);

		float textX = (drawMode == Shapes::CORNER) ? (x + width / 2) : x;
		float textY = (drawMode == Shapes::CORNER) ? (y - height / 2) : y;

		Text::text(msg, textX, textY, Text::ALIGN_CENTER);
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

	// -------------------------------------------------------------------------
	// Slider Implementation
	// -------------------------------------------------------------------------

	Slider::Slider(float x, float y, float width, float height, float& valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), valueRef(valRef), minValue(minVal), maxValue(maxVal) {
	}

	Slider::Slider(void)
		: UI_Element(), valueRef(*(new float(0))), minValue(0), maxValue(100) {
	}

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
			Color::fill(Color::CL_Color_Create(0, 255, 0, 255));
		}
		else {
			Color::fill(style.secondaryColor);
		}
		Shapes::rect(x, y, filledWidth, height, drawMode);
	}
}