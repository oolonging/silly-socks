#include "UI_Elements.hpp"
#include "../InputManager.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Static Member Initialization
	// -------------------------------------------------------------------------

	TextBox* TextBox::currentlySelected = nullptr;
	std::vector<RadioButton*> RadioButton::radioGroups[10];

	// -------------------------------------------------------------------------
	// UI_Element Base Class Implementation
	// -------------------------------------------------------------------------

	ElementStyle UI_Element::getDefaultStyle() {
		ElementStyle defaultStyle;
		defaultStyle.primaryColor = { 200.0f, 200.0f, 200.0f, 255.0f };
		defaultStyle.secondaryColor = { 100.0f, 150.0f, 255.0f, 255.0f };
		defaultStyle.strokeColor = { 0.0f, 0.0f, 0.0f, 255.0f };
		defaultStyle.strokeWeight = 2;
		return defaultStyle;
	}

	ElementTexture UI_Element::getDefaultTexture() {
		ElementTexture defaultTexture;
		defaultTexture.primaryTexture = nullptr; // TODO: add texture when it gets added
		defaultTexture.secondaryTexture = nullptr; // TODO: add texture when it gets added
		return defaultTexture;
	}

	TextStyle UI_Element::getDefaultTextStyle() {
		TextStyle defaultStyle;
		defaultStyle.primaryColor = Color::Preset::Black;
		defaultStyle.secondaryColor = Color::Preset::White;
		defaultStyle.fontSize = 24;
		defaultStyle.fontPath = "default"; // TODO: change to fontName
		return defaultStyle;
	}

	// New implementation of isHover using input manager
	bool UI_Element::isHovering(void) const {
		bool xOverlap{};
		bool yOverlap{};

		float mouseX = Input::getMouseX();
		float mouseY = Input::getMouseY();

		if (drawMode == Shapes::CORNER) {
			xOverlap = (mouseX > this->x && mouseX <= (this->x + this->width));
			yOverlap = (mouseY < this->y && mouseY >= (this->y - this->height));
		}
		else if (drawMode == Shapes::CENTER) {
			xOverlap = (mouseX > (this->x - this->width / 2) && mouseX <= (this->x + (this->width / 2)));
			yOverlap = (mouseY < (this->y + this->height / 2) && mouseY >= (this->y - (this->height / 2)));
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


	// Ctors
	UI_Element::UI_Element(float x, float y, float width, float height, Shapes::SHAPE_MODE mode)
		: x(x), y(y), width(width), height(height), drawMode(mode), style(getDefaultStyle()) {
	}

	UI_Element::UI_Element()
		: x(0), y(0), width(100), height(50), drawMode(Shapes::CORNER), style(getDefaultStyle()) {
	}
}
