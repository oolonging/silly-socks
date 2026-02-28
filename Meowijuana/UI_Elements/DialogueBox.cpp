#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
// -------------------------------------------------------------------------
// Dialogue Box Implementation
// -------------------------------------------------------------------------

DialogueBox::DialogueBox(float x, float y, float width, float height, char const* speakerName,
	const std::string& message, AEGfxTexture* sprite, Shapes::SHAPE_MODE mode)
	: UI_Element(x, y, width, height, mode), message(message), speakerName(speakerName),
	characterSprite(sprite), showSprite(sprite != nullptr), isActive(false),
	spriteSize(120.0f), textPadding(20.0f), nameBoxHeight(40.0f), onDismiss(nullptr) {
	// Custom default style for dialogue box
	style.primaryColor = Color::createColorRGB(240, 240, 240, 255);      // Light gray background
	style.secondaryColor = Color::createColorRGB(60, 60, 80, 255);       // Dark blue-gray for name box
	style.strokeColor = Color::createColorRGB(40, 40, 50, 255);          // Dark border
	style.strokeWeight = 3;
}

// TODO: set up the position properly, add easter egg message here too, add graphical rendering if assets are available
DialogueBox::DialogueBox(void)
	: UI_Element(0, 0, 800, 200, Shapes::CORNER), message("You found a hidden easter egg"), speakerName(nullptr),
	characterSprite(nullptr), showSprite(false), isActive(false),
	spriteSize(120.0f), textPadding(20.0f), nameBoxHeight(40.0f), onDismiss(nullptr) {
	style.primaryColor = Color::createColorRGB(240, 240, 240, 255);
	style.secondaryColor = Color::createColorRGB(60, 60, 80, 255);
	style.strokeColor = Color::createColorRGB(40, 40, 50, 255);
	style.strokeWeight = 3;
}

void DialogueBox::setText(const std::string& text) {
	message = text;
}

void DialogueBox::setSpeaker(char const* name) {
	speakerName = name;
}

void DialogueBox::setCharacterSprite(AEGfxTexture* sprite) {
	characterSprite = sprite;
	if (sprite != nullptr) {
		showSprite = true;
	}
}

void DialogueBox::setShowSprite(bool show) {
	showSprite = show && (characterSprite != nullptr);
}

void DialogueBox::activate() {
	isActive = true;
}

void DialogueBox::dismiss() {
	isActive = false;
	if (onDismiss != nullptr) {
		onDismiss();
	}
}

bool DialogueBox::getIsActive() const {
	return isActive;
}

void DialogueBox::setOnDismiss(void (*func)(void)) {
	onDismiss = func;
}

std::vector<std::string> DialogueBox::wrapText(const std::string& text, float maxWidth) {
	std::vector<std::string> lines;
	std::string currentLine = "";
	std::string word = "";

	for (size_t i = 0; i < text.length(); ++i) {
		char c = text[i];

		if (c == ' ' || c == '\n' || i == text.length() - 1) {
			// Add last character if end of string
			if (i == text.length() - 1 && c != ' ' && c != '\n') {
				word += c;
			}

			// Check if adding this word exceeds the line width
			std::string testLine = currentLine.empty() ? word : currentLine + " " + word;

			// Simple character count approximation (you may want to use actual text measurement)
			// Assuming average character width is ~10 pixels at default size
			float estimatedWidth = testLine.length() * 10.0f;

			if (estimatedWidth > maxWidth || c == '\n') {
				// Start new line
				if (!currentLine.empty()) {
					lines.push_back(currentLine);
					currentLine = word;
				}
				else {
					lines.push_back(word);
					currentLine = "";
				}
			}
			else {
				currentLine = testLine;
			}

			word = "";
		}
		else {
			word += c;
		}
	}

	// Add the last line
	if (!currentLine.empty()) {
		lines.push_back(currentLine);
	}

	return lines;
}

void DialogueBox::draw(void) {
	if (!isActive) return;
	// set render mode to color for shapes and text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// set text align
	Text::textAlign(Text::LEFT, Text::TOP);

	// Handle click to dismiss (anywhere on screen)
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		dismiss();
		return;
	}

	// Calculate positions based on draw mode
	float boxX = x;
	float boxY = y;

	if (drawMode == Shapes::CENTER) {
		boxX = x - width / 2;
		boxY = y + height / 2;
	}

	// Draw main dialogue box background
	Color::stroke(style.strokeColor);
	Color::strokeWeight(style.strokeWeight);
	Color::fill(style.primaryColor);
	Shapes::rect(boxX, boxY, width, height, Shapes::CORNER);

	// Draw speaker name box if speaker exists
	if (speakerName != nullptr) {
		Color::fill(style.secondaryColor);
		Shapes::rect(boxX, boxY, width * 0.3f, nameBoxHeight, Shapes::CORNER);

		// Draw speaker name
		Color::textFill(255, 255, 255, 255); // White text
		Text::text(speakerName, boxX + textPadding, boxY - nameBoxHeight / 2);
	}

	// Draw character sprite if enabled
	float textStartX = boxX + textPadding;
	if (showSprite && characterSprite != nullptr) {
		float spriteX = boxX + textPadding;
		float spriteY = boxY - nameBoxHeight - textPadding;

		// Draw sprite box background
		Color::noStroke();
		Color::fill(255, 255, 255, 255);
		Shapes::rect(spriteX, spriteY, spriteSize, spriteSize, Shapes::CORNER);

		// Draw sprite
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		Graphics::image(spriteX, spriteY, spriteSize, spriteSize, characterSprite, Shapes::CORNER);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// Adjust text start position to account for sprite
		textStartX = spriteX + spriteSize + textPadding;
	}

	// Calculate text area
	float textAreaWidth = width - (textStartX - boxX) - textPadding;
	float textStartY = boxY - nameBoxHeight - textPadding;

	// Wrap and draw message text
	std::vector<std::string> wrappedLines = wrapText(message, textAreaWidth);

	Color::textFill(0, 0, 0, 255); // Black text
	float lineHeight = 25.0f; // Adjust based on your font size

	for (size_t i = 0; i < wrappedLines.size(); ++i) {
		float lineY = textStartY - (i * lineHeight);
		Text::text(wrappedLines[i].c_str(), textStartX, lineY);
	}

	// Draw "Click to continue" hint at bottom right
	Color::textFill(120, 120, 120, 255); // Gray text
	float hintX = boxX + width - textPadding - 150;
	float hintY = boxY - height + textPadding + 10;
	Text::text("[Click to continue]", hintX, hintY);
}
}
