#include "UI_Elements.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Button Implementation
	// -------------------------------------------------------------------------

	void Button::setOnClick(void (*func)(void)) {
		this->onClick = func;
	}

	//void Button::draw(void) {
	//	bool hovering = this->isHovering();

	//	// Apply stroke
	//	Color::stroke(style.strokeColor);
	//	Color::strokeWeight(style.strokeWeight);

	//	// Handle click
	//	if (hovering) {
	//		if (AEInputCheckTriggered(AEVK_LBUTTON) && onClick != nullptr) {
	//			onClick();
	//		}
	//		Color::fill(style.secondaryColor);
	//	}
	//	else {
	//		Color::fill(style.primaryColor);
	//	}

	//	if (this->texture.primaryTexture != nullptr) {
	//		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//		Graphics::image(x, y, width, height, this->texture.primaryTexture, drawMode);
	//	}
	//	else {
	//		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//		Shapes::rect(x, y, width, height, drawMode);
	//	}

	//	// Draw text
	//	Color::textFill(0, 0, 0);

	//	if (drawMode == Shapes::CORNER) {
	//		Text::text(msg, x + (width / 2), y - (height / 2), Text::CENTER_H, Text::CENTER_V);
	//	}
	//	else if (drawMode == Shapes::CENTER) {
	//		Text::text(msg, x, y, Text::CENTER_H, Text::CENTER_V);
	//	}

	//	// temporary balm to soothe the sting of being a moron
	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//}

	// New draw function that can handle graphics
	void Button::draw(void) {
		bool hovering = this->isHovering();
		bool graphicalDraw = (this->texture.primaryTexture != nullptr) && (this->texture.secondaryTexture != nullptr);
		bool drawStroke = (this->style.strokeColor.alpha > 0.0f) && (this->style.strokeWeight >= 0);

		// Draw the button
		if (graphicalDraw) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			Graphics::image(this->x, this->y, this->width, this->height, (hovering ? this->texture.secondaryTexture : this->texture.primaryTexture), this->drawMode);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			if (hovering) {
				Color::fill(this->style.primaryColor);
			}
			else {
				Color::fill(this->style.secondaryColor);
			}

			// draw the rectangle
			Shapes::rect(this->x, this->y, this->width, this->height, this->drawMode);
		}

		// Draw the label
		Color::textFill(this->textStyle.primaryColor);
		if (drawMode == Shapes::CORNER) {
			Text::text(msg, x + (width / 2), y - (height / 2), Text::CENTER_H, Text::CENTER_V);
		}
		else if (drawMode == Shapes::CENTER) {
			Text::text(msg, x, y, Text::CENTER_H, Text::CENTER_V);
		}

		// Completely forgot to handle clicks
		if (hovering && AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (this->onClick != nullptr) this->onClick();
		}

		// Temporary balm to soothe the sting of being a dumbass
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
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
