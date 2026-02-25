#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// TextBox Implementation
	// -------------------------------------------------------------------------

	TextBox::TextBox(float x, float y, float width, float height, const std::string& placeholder, size_t maxLen, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), text(""), placeholderText(placeholder), maxLength(maxLen),
		isSelected(false), cursorBlinkTimer(0.0f), showCursor(false) {
		// Custom default style for textbox
		style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
		style.secondaryColor = Color::createColorRGB(200, 230, 255, 255);
		style.strokeColor = Color::createColorRGB(100, 100, 100, 255);
		style.strokeWeight = 2;
	}

	TextBox::TextBox(void)
		: UI_Element(), text(""), placeholderText("Enter text..."), maxLength(50),
		isSelected(false), cursorBlinkTimer(0.0f), showCursor(false) {
		style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
		style.secondaryColor = Color::createColorRGB(200, 230, 255, 255);
		style.strokeColor = Color::createColorRGB(100, 100, 100, 255);
		style.strokeWeight = 2;
	}

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
				displayText += "|";
			}

			Text::text(displayText.c_str(), textX, textY);
		}
	}
}
