#include "UI_Elements.hpp"
#include "../InputManager.hpp"

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

		if (drawMode == Shapes::CORNER) {
			// check if width is negative
			if (this->width < 0.0f) { xOverlap = (mouseX < this->x && mouseX >= (this->x + this->width)); }
			else { xOverlap = (mouseX > this->x && mouseX <= (this->x + this->width)); }

			// check if height is negative
			if (this->height < 0.0f) { yOverlap = (mouseY > this->y && mouseY <= (this->y - this->height)); }
			else { yOverlap = (mouseY < this->y && mouseY >= (this->y - this->height)); }
		}
		else if (drawMode == Shapes::CENTER) {
			// check if width is negative
			if (this->width < 0.0f) { xOverlap = (mouseX < (this->x + this->width / 2) && mouseX >= (this->x - (this->width / 2))); } 
			else { xOverlap = (mouseX > (this->x - this->width / 2) && mouseX <= (this->x + (this->width / 2))); }

			// check if height is negative
			if (this->height < 0.0f) { yOverlap = (mouseY > (this->y - this->height / 2) && mouseY <= (this->y + (this->height / 2))); }
			else { yOverlap = (mouseY < (this->y + this->height / 2) && mouseY >= (this->y - (this->height / 2))); }
		}

		return xOverlap && yOverlap;
	}

	// Getters and Setters
	void UI_Element::setStyle(ElementStyle newStyle) {
		this->style = newStyle;
	}

	void UI_Element::setTextStyle(TextStyle newStyle) {
		this->textStyle = newStyle;
	}

	void UI_Element::setTexture(ElementTexture newTexture) {
		this->texture = newTexture;
	}

	ElementStyle UI_Element::getStyle(void) const {
		return this->style;
	}

	TextStyle UI_Element::getTextStyle(void) const {
		return this->textStyle;
	}

	ElementTexture UI_Element::getTexture(void) const {
		return this->texture;
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
