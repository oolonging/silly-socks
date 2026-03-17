#include "../pch.h"
#include "UI_Elements.hpp"

#include "../InputManager.hpp"

GameData gameData;

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Static Member Initialization
	// -------------------------------------------------------------------------

	// For text box
	TextBox* TextBox::currentlySelected = nullptr;

	// For slider
	Slider* Slider::currentlySelected = nullptr;
	
	// For radio button
	std::vector<RadioButton*> RadioButton::radioGroups[10];

	// -------------------------------------------------------------------------
	// UI_Element Base Class Implementation
	// -------------------------------------------------------------------------

	ElementStyle UI_Element::getDefaultStyle() {
		ElementStyle defaultStyle;
		defaultStyle.primaryColor = Color::Preset::defaultStylePrimary;
		defaultStyle.secondaryColor = Color::Preset::defaultStyleSecondary;
		defaultStyle.strokeColor = Color::Preset::defaultStyleStroke;
		defaultStyle.strokeWeight = 1;
		return defaultStyle;
	}

	TextStyle UI_Element::getDefaultTextStyle() {
		TextStyle defaultStyle;
		defaultStyle.primaryColor = Color::Preset::Black;
		defaultStyle.secondaryColor = Color::Preset::White;
		defaultStyle.fontSize = 10;
		defaultStyle.fontName = "default";
		return defaultStyle;
	}

	ElementTexture UI_Element::getDefaultTexture() {
		// Wont have a texture by default
		ElementTexture defaultTexture;
		defaultTexture.primaryTexture = nullptr;
		defaultTexture.secondaryTexture = nullptr;
		return defaultTexture;
	}

	bool UI_Element::isHovering(void) const {
		bool xOverlap{};
		bool yOverlap{};

		float mouseX = Input::getMouseX();
		float mouseY = Input::getMouseY();

		float topLeftX = (drawMode == Shapes::CORNER) ? this->x : (this->x - this->width * 0.5f);
		float topLeftY = (drawMode == Shapes::CORNER) ? this->y : (this->y + this->height * 0.5f);

		// check if width is negative
		if (this->width >= 0) xOverlap = (mouseX >= topLeftX) && (mouseX <= topLeftX + this->width);
		else xOverlap = (mouseX <= topLeftX) && (mouseX >= topLeftX + this->width);

		// chcek if height is negative
		if (this->height >= 0) yOverlap = (mouseY <= topLeftY) && (mouseY >= topLeftY - this->height);
		else yOverlap = (mouseY >= topLeftY) && (mouseY <= topLeftY - this->height);

		return xOverlap && yOverlap;
	}

	// clear textures
	void UI_Element::clearTextures(void) {
		if (this->texture.primaryTexture != nullptr) {
			AEGfxTextureUnload(this->texture.primaryTexture);
			this->texture.primaryTexture = nullptr;
		}

		if (this->texture.secondaryTexture != nullptr) {
			AEGfxTextureUnload(this->texture.secondaryTexture);
			this->texture.secondaryTexture = nullptr;
		}
	}

	// Ctors
	UI_Element::UI_Element(float x, float y, float width, float height, Shapes::SHAPE_MODE mode)
		: x(x), y(y), width(width), height(height), drawMode(mode) {
		
		this->textStyle = getDefaultTextStyle();

		printf("UI Element constructor called\n");
	}

	UI_Element::UI_Element(void)
		: UI_Element(0.0f, 0.0f, 100.0f, 50.0f) {}
}
