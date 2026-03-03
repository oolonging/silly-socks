#include "UI_Elements.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Button Static Defaults
	// -------------------------------------------------------------------------
	ElementStyle Button::defaultStyle = UI_Element::getDefaultStyle();
	TextStyle Button::defaultTextStyle = UI_Element::getDefaultTextStyle();
	ElementTexture Button::defaultTexture = UI_Element::getDefaultTexture();

	// Definitions for default setters
	void Button::setDefaultButtonStyle(ElementStyle newStyle) { defaultStyle = newStyle; }
	void Button::setDefaultButtonTextStyle(TextStyle newStyle) { defaultTextStyle = newStyle; }
	void Button::setDefaultButtonTexture(ElementTexture newTexture) { defaultTexture = newTexture; }

	// Definitions for default getters
	ElementStyle Button::getDefaultButtonStyle(void) {
		UI_Elements::ElementStyle defaultButtonStyle;
		defaultButtonStyle.primaryColor = Color::Color(Color::Preset::Red);
		defaultButtonStyle.secondaryColor = Color::Color(Color::Preset::Blue);
		defaultButtonStyle.strokeColor = Color::Color(Color::Preset::Black);
		defaultButtonStyle.strokeWeight = 2;
		return defaultButtonStyle;
	}

	TextStyle Button::getDefaultButtonTextStyle(void) {
		UI_Elements::TextStyle defaultButtonTextStyle;
		defaultButtonTextStyle.primaryColor = Color::Color(Color::Preset::Blue);
		defaultButtonTextStyle.secondaryColor = Color::Color(Color::Preset::Red);
		defaultButtonTextStyle.fontSize = 10;
		defaultButtonTextStyle.fontName = "default";
		return defaultButtonTextStyle;
	}

	ElementTexture Button::getDefaultButtonTexture(void) {
		UI_Elements::ElementTexture defaultButtonTexture;
		defaultButtonTexture.primaryTexture = nullptr;
		defaultButtonTexture.secondaryTexture = nullptr;
		return defaultButtonTexture;
	}

	// -------------------------------------------------------------------------
	// Button Implementation
	// -------------------------------------------------------------------------

	void Button::setOnClick(void (*func)(void)) {
		this->onClick = func;
	}

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
				Color::textFill(this->textStyle.primaryColor);
			}
			else {
				Color::fill(this->style.secondaryColor);
				Color::textFill(this->textStyle.secondaryColor);
			}

			// draw the rectangle
			Shapes::rect(this->x, this->y, this->width, this->height, this->drawMode);
		}

		// set text size
		Text::setFont(this->textStyle.fontName);
		Text::textSize(this->textStyle.fontSize);

		// Draw the label
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

		// Use the initialized defaults
		this->style = defaultStyle;
		this->textStyle = defaultTextStyle;
		this->texture = defaultTexture;

		printf("Main constructor called\n");
	}

	//Button::Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode)
	//	: UI_Element(quad.position.x, quad.position.y, quad.width, quad.height, mode), msg(msg), onClick(nullptr) {
	//}

	Button::Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode)
		: Button(quad.position.x, quad.position.y, quad.width, quad.height, msg, mode) {
		printf("Quad constructor called\n");
	}

	//Button::Button(void)
	//	: UI_Element(), msg("Button"), onClick(nullptr) {
	//}

	Button::Button(void)
		: Button(0.0f, 0.0f, 100.0f, 50.0f, "Button", Shapes::CORNER) {
		printf("Default constructor called\n");
	}
}
