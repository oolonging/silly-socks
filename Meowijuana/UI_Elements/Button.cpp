#include "UI_Elements.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Button Implementation
	// -------------------------------------------------------------------------

	void Button::setOnClick(void (*func)(void)) {
		this->onClick = func;
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

		if (drawMode == Shapes::CORNER) {
			Text::text(msg, x + (width / 2), y - (height / 2), Text::CENTER_H, Text::CENTER_V);
		}
		else if (drawMode == Shapes::CENTER) {
			Text::text(msg, x, y, Text::CENTER_H, Text::CENTER_V);
		}
	}

	// Ctors
	Button::Button(float x, float y, float width, float height, char const* msg, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), msg(msg), onClick(nullptr) {
	}

	Button::Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode)
		: UI_Element(quad.position.x, quad.position.y, quad.width, quad.height, mode), msg(msg), onClick(nullptr) {
	}

	Button::Button(void)
		: UI_Element(), msg("Button"), onClick(nullptr) {
	}
}
