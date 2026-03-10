#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// TextBox Implementation
	// -------------------------------------------------------------------------
	ElementStyle TextBox::defaultStyle = UI_Element::getDefaultStyle();
	TextStyle TextBox::defaultTextStyle = UI_Element::getDefaultTextStyle();
	ElementTexture TextBox::defaultTexture = UI_Element::getDefaultTexture();

	// definitions for default setters
	void TextBox::setDefaultTextBoxStyle(ElementStyle newStyle) { defaultStyle = newStyle; }
	void TextBox::setDefaultTextBoxTextStyle(TextStyle newStyle) { defaultTextStyle = newStyle; }
	void TextBox::setDefaultTextBoxTexture(ElementTexture newTexture) { defaultTexture = newTexture; }

	// definitions for default getters
	ElementStyle TextBox::getDefaultTextBoxStyle(void) {
		ElementStyle defaultStyle;
		defaultStyle.primaryColor = Color::Preset::White;
		defaultStyle.secondaryColor = Color::createColorHex(0xFFC0C0C0);
		defaultStyle.strokeColor = Color::createColorHex(0xFF696969);
		defaultStyle.strokeWeight = 1;
		return defaultStyle;
	}

	TextStyle TextBox::getDefaultTextBoxTextStyle(void) {
		TextStyle defaultTextStyle;
		defaultTextStyle.primaryColor = Color::Preset::Black;
		defaultTextStyle.secondaryColor = Color::Preset::DarkGray;
		defaultTextStyle.fontSize = 10;
		defaultTextStyle.fontName = "default";
		return defaultTextStyle;
	}

	ElementTexture TextBox::getDefaultTextBoxTexture(void) {
		ElementTexture defaultTexture;
		defaultTexture.primaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Textbox/primary.png");
		defaultTexture.secondaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Textbox/secondary.png");
		return defaultTexture;
	}

	// clear default textures
	void TextBox::clearDefaultTextBoxTextures(void) {
		if (defaultTexture.primaryTexture != nullptr) {
			AEGfxTextureUnload(defaultTexture.primaryTexture);
			defaultTexture.primaryTexture = nullptr;
		}

		if (defaultTexture.primaryTexture != nullptr) {
			AEGfxTextureUnload(defaultTexture.secondaryTexture);
			defaultTexture.secondaryTexture = nullptr;
		}
	}

	// -------------------------------------------------------------------------
	// TextBox Implementation
	// -------------------------------------------------------------------------

	TextBox::TextBox(float x, float y, float width, float height, const std::string& placeholder, size_t maxLen, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), text(""), placeholderText(placeholder), maxLength(maxLen),
		isSelected(false), cursorBlinkTimer(0.0f), showCursor(false) {
		// Custom default style for textbox
		this->style = defaultStyle;
		this->textStyle = defaultTextStyle;
		this->texture = defaultTexture;
	}

	TextBox::TextBox(void) 
		: TextBox(0.0f, 0.0f, 100.0f, 100.0f, "Enter text...") {}

	void TextBox::select() {
		// Deselect previously selected textbox
		if (currentlySelected != nullptr && currentlySelected != this) {
			currentlySelected->deselect();
		}
		isSelected = true;
		currentlySelected = this;
		cursorBlinkTimer = 0.0f;
		showCursor = true;
	}

	void TextBox::deselect() {
		isSelected = false;
		if (currentlySelected == this) {
			currentlySelected = nullptr;
		}
		showCursor = false;
	}

	bool TextBox::getIsSelected() const {
		return isSelected;
	}

	std::string TextBox::getText() const {
		return text;
	}

	void TextBox::setText(const std::string& newText) {
		if (newText.length() <= maxLength) {
			text = newText;
		}
		else {
			text = newText.substr(0, maxLength);
		}
	}

	void TextBox::setPlaceholder(const std::string& placeholder) {
		placeholderText = placeholder;
	}

	void TextBox::handleInput() {
		if (!isSelected) return;

		// Handle backspace
		if (AEInputCheckTriggered(AEVK_BACK) && !text.empty()) {
			text.pop_back();
		}

		// Handle alphanumeric and common punctuation
		if (text.length() < maxLength) {
			// Letters A-Z
			for (char c = 'A'; c <= 'Z'; ++c) {
				if (AEInputCheckTriggered(static_cast<unsigned char>(c))) {
					bool shift = AEInputCheckCurr(AEVK_LSHIFT) || AEInputCheckCurr(AEVK_RSHIFT);
					text += shift ? c : (c + 32); // lowercase if no shift
				}
			}

			// Numbers 0-9
			for (char c = '0'; c <= '9'; ++c) {
				if (AEInputCheckTriggered(static_cast<unsigned char>(c))) {
					text += c;
				}
			}

			// Space
			if (AEInputCheckTriggered(AEVK_SPACE)) {
				text += ' ';
			}

			// Common punctuation
			if (AEInputCheckTriggered(AEVK_PERIOD)) text += '.';
			if (AEInputCheckTriggered(AEVK_COMMA)) text += ',';
			if (AEInputCheckTriggered(static_cast<unsigned char>('1')) && AEInputCheckCurr(AEVK_LSHIFT) || AEInputCheckCurr(AEVK_RSHIFT)) text += '!';
			if (AEInputCheckTriggered(static_cast<unsigned char>('2')) && AEInputCheckCurr(AEVK_LSHIFT) || AEInputCheckCurr(AEVK_RSHIFT)) text += '@';
		}
	}

	void TextBox::updateCursor(float deltaTime) {
		if (!isSelected) return;

		cursorBlinkTimer += deltaTime;
		if (cursorBlinkTimer >= 0.5f) {
			showCursor = !showCursor;
			cursorBlinkTimer = 0.0f;
		}
	}

	void TextBox::draw(void) {
		// Handle selection
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			if (isHovering()) {
				select();
			}
			else if (isSelected) {
				deselect();
			}
		}

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// Handle keyboard input
		handleInput();

		// Update cursor blink
		updateCursor(static_cast<float>(AEFrameRateControllerGetFrameTime()));

		// Apply stroke (highlight if selected)
		if (isSelected) {
			Color::stroke(Color::createColorRGB(0, 120, 255, 255)); // Blue when selected
			Color::strokeWeight(style.strokeWeight + 1);
			Color::fill(style.secondaryColor);
		}
		else {
			Color::stroke(style.strokeColor);
			Color::strokeWeight(style.strokeWeight);
			Color::fill(style.primaryColor);
		}

		// Draw background
		Shapes::rect(x, y, width, height, drawMode);

		// Draw text or placeholder
		float textX = (drawMode == Shapes::CORNER) ? this->x + (this->width * 0.5f) : this->x;
		float textY = (drawMode == Shapes::CORNER) ? this->y - (this->height * 0.5f) : this->y;

		if (text.empty()) {
			// Draw placeholder in gray
			Color::textFill(150, 150, 150, 255);
			Text::text(placeholderText.c_str(), textX, textY);
		}
		else {
			// Draw actual text in black
			Color::textFill(0, 0, 0, 255);
			std::string displayText = text;

			// Add cursor if selected
			if (isSelected && showCursor) {
				displayText += "<";
			}

			Text::text(displayText.c_str(), textX, textY);
		}
	}
}
