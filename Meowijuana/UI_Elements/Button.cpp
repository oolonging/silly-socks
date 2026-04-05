#include "../pch.h"
#include "UI_Elements.hpp"
#include "../InputManager.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Button Implementation
	// -------------------------------------------------------------------------

	void Button::setOnClick(void (*func)(void)) {
		this->onClick = func;
	}

	void Button::draw(void) {
		bool hovering = this->isHovering();
		bool graphicalDraw = (this->texture.primaryTexture != nullptr) && (this->texture.secondaryTexture != nullptr);

		if (graphicalDraw) {
			// Prepare to draw graphics
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.0f);
			Color::noStroke();
			AEGfxTextureSet((hovering)? this->texture.secondaryTexture : this->texture.primaryTexture , 0, 0);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Color::stroke(this->style.strokeColor);
			Color::strokeWeight(this->style.strokeWeight);
		}
		
		if (hovering) {
			// change custom cursor to pointer
			Input::gCursorType = Input::CLICKABLE;

			Color::fill(this->style.secondaryColor);
			Color::textFill(this->textStyle.secondaryColor);
			Text::textSize(this->textStyle.fontSize * 1.5f);
		}
		else {
			Color::fill(this->style.primaryColor);
			Color::textFill(this->textStyle.primaryColor);
			Text::textSize(this->textStyle.fontSize);
		}

		// Draw button
		Shapes::rect(this->x, this->y, this->width, this->height, this->drawMode);

		// set font
		Text::setFont(this->textStyle.fontName);
		Text::textSize(17.0f);

		// Draw the label
		if (drawMode == Shapes::CORNER) {
			Text::text(msg, x + (width / 2), y - (height / 2), Text::CENTER_H, Text::CENTER_V);
		}
		else if (drawMode == Shapes::CENTER) {
			Text::text(msg, x, y, Text::CENTER_H, Text::CENTER_V);
		}

		// Handle click events
		if (hovering && AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (this->onClick != nullptr) this->onClick();
		}

		// Debug draw test
		Shapes::debugRect(this->x, this->y, this->width, this->height, this->drawMode);
	}

	// Ctors
	Button::Button(float x, float y, float width, float height, char const* msg, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), msg(msg), onClick(nullptr) {

		this->style = getDefaultStyle();
		this->textStyle = getDefaultTextStyle();

		// Default textuers
		this->texture.primaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Button/primary.png");
		this->texture.secondaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Button/secondary.png");
	}

	Button::Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode)
		: Button(quad.position.x, quad.position.y, quad.width, quad.height, msg, mode) {}

	Button::Button(void)
		: Button(0.0f, 0.0f, 100.0f, 50.0f, "Button", Shapes::CORNER) {}

	void Button::setPosition(float newX, float newY) {
		x = newX;
		y = newY;
	}
}


