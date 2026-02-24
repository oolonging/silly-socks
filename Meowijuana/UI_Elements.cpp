#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

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

	UI_Element::UI_Element(float x, float y, float width, float height, Shapes::SHAPE_MODE mode)
		: x(x), y(y), width(width), height(height), drawMode(mode), style(getDefaultStyle()) {
	}

	UI_Element::UI_Element()
		: x(0), y(0), width(100), height(50), drawMode(Shapes::CORNER), style(getDefaultStyle()) {
	}

	bool UI_Element::isHovering(void) const {
		int32_t mx = 0, my = 0;
		AEInputGetCursorPosition(&mx, &my);

		int32_t ww = AEGfxGetWindowWidth();
		int32_t wh = AEGfxGetWindowHeight();
		float worldX = mx - ww * 0.5f;
		float worldY = wh * 0.5f - my;

		bool xOverlap{}, yOverlap{};

		if (drawMode == Shapes::CORNER) {
			xOverlap = ((worldX >= x) && (worldX <= (x + width)));
			yOverlap = ((worldY <= y) && (worldY >= (y - height)));
		}
		else if (drawMode == Shapes::CENTER) {
			xOverlap = ((worldX >= (x - width / 2)) && (worldX <= (x + width / 2)));
			yOverlap = ((worldY >= (y - height / 2)) && (worldY <= (y + height / 2)));
		}

		return (xOverlap && yOverlap);
	}

	void UI_Element::setStyle(ElementStyle newStyle) {
		style = newStyle;
	}

	ElementStyle UI_Element::getStyle(void) const {
		return style;
	}

	// -------------------------------------------------------------------------
	// Button Implementation
	// -------------------------------------------------------------------------

	Button::Button(float x, float y, float width, float height, char const* msg, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), msg(msg), onClick(nullptr) {
	}

	Button::Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode)
		: UI_Element(quad.position.x, quad.position.y, quad.width, quad.height, mode), msg(msg), onClick(nullptr) {
	}

	Button::Button(void)
		: UI_Element(), msg("Button"), onClick(nullptr) {
	}

	void Button::setOnClick(void (*func)(void)) {
		onClick = func;
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

	// -------------------------------------------------------------------------
	// ProgressBar Implementation
	// -------------------------------------------------------------------------

	ProgressBar::ProgressBar(float x, float y, float width, float height, float valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), value(valRef), minValue(minVal), maxValue(maxVal) {
	}

	ProgressBar::ProgressBar(void)
		: UI_Element(), value(0), minValue(0), maxValue(100) {
	}

	void ProgressBar::clampValue(void) {
		if (value < minValue) value = minValue;
		if (value > maxValue) value = maxValue;
	}

	void ProgressBar::setValue(float newValue) {
		value = newValue;
	}

	void ProgressBar::draw(void) {
		float filledWidth = (value / maxValue) * width;

		// Apply stroke
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		// Background
		Color::fill(style.primaryColor);
		Shapes::rect(x, y, width, height, drawMode);

		// Foreground
		Color::fill(style.secondaryColor);
		Shapes::rect(x, y, filledWidth, height, drawMode);
	}

	// -------------------------------------------------------------------------
	// Slider Implementation
	// -------------------------------------------------------------------------

	Slider::Slider(float x, float y, float width, float height, float& valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), valueRef(valRef), minValue(minVal), maxValue(maxVal) {
	}

	// TODO: ensure that the removal of the default constructor didnt break anything (for slider UI element)

	void Slider::clampValue(void) {
		if (valueRef < minValue) valueRef = minValue;
		if (valueRef > maxValue) valueRef = maxValue;
	}

	void Slider::updateValue(void) {
		if (isHovering() && AEInputCheckCurr(AEVK_LBUTTON)) {
			int32_t mx = 0, my = 0;
			AEInputGetCursorPosition(&mx, &my);
			int32_t ww = AEGfxGetWindowWidth();
			float worldX = mx - ww * 0.5f;

			float relativeX = (drawMode == Shapes::CENTER)
				? (worldX - x + width / 2)
				: (worldX - x);

			float newValue = (relativeX / width) * (maxValue - minValue) + minValue;
			valueRef = newValue;
		}
	}

	void Slider::draw(void) {
		updateValue();
		clampValue();

		float filledWidth = ((valueRef - minValue) / (maxValue - minValue)) * width;

		// Apply stroke
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		// Background
		Color::fill(style.primaryColor);
		Shapes::rect(x, y, width, height, drawMode);

		// Foreground
		if (isHovering() && AEInputCheckCurr(AEVK_LBUTTON)) {
			Color::fill(Color::createColorRGB(0, 255, 0, 255));
		}
		else {
			Color::fill(style.secondaryColor);
		}
		Shapes::rect(x, y, filledWidth, height, drawMode);
	}

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
		float textX = (drawMode == Shapes::CORNER) ? (x + 10) : (x - width / 2 + 10);
		float textY = (drawMode == Shapes::CORNER) ? (y - height / 2) : y;

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

	// -------------------------------------------------------------------------
	// Checkbox Implementation
	// -------------------------------------------------------------------------

	Checkbox::Checkbox(float x, float y, float boxSize, char const* label, bool initialState, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, boxSize + 100, boxSize, mode), isChecked(initialState), label(label), boxSize(boxSize), onChange(nullptr) {
		// Custom default style for checkbox
		style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
		style.secondaryColor = Color::createColorRGB(100, 200, 100, 255);
		style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
		style.strokeWeight = 2;
	}

	Checkbox::Checkbox(void)
		: UI_Element(), isChecked(false), label("Checkbox"), boxSize(20), onChange(nullptr) {
		style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
		style.secondaryColor = Color::createColorRGB(100, 200, 100, 255);
		style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
		style.strokeWeight = 2;
	}

	void Checkbox::toggle() {
		isChecked = !isChecked;
		if (onChange != nullptr) {
			onChange(isChecked);
		}
	}

	bool Checkbox::getChecked() const {
		return isChecked;
	}

	void Checkbox::setChecked(bool checked) {
		if (isChecked != checked) {
			isChecked = checked;
			if (onChange != nullptr) {
				onChange(isChecked);
			}
		}
	}

	void Checkbox::setOnChange(void (*func)(bool)) {
		onChange = func;
	}

	void Checkbox::draw(void) {
		// Check if checkbox area is hovered (just the box, not the label)
		int32_t mx = 0, my = 0;
		AEInputGetCursorPosition(&mx, &my);
		int32_t ww = AEGfxGetWindowWidth();
		int32_t wh = AEGfxGetWindowHeight();
		float worldX = mx - ww * 0.5f;
		float worldY = wh * 0.5f - my;

		bool boxHovered = false;
		if (drawMode == Shapes::CORNER) {
			boxHovered = ((worldX >= x) && (worldX <= (x + boxSize)) &&
				(worldY <= y) && (worldY >= (y - boxSize)));
		}
		else {
			boxHovered = ((worldX >= (x - boxSize / 2)) && (worldX <= (x + boxSize / 2)) &&
				(worldY >= (y - boxSize / 2)) && (worldY <= (y + boxSize / 2)));
		}

		// Handle click
		if (boxHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
			toggle();
		}

		// Draw checkbox box
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		if (isChecked) {
			Color::fill(style.secondaryColor);
		}
		else {
			Color::fill(style.primaryColor);
		}

		Shapes::rect(x, y, boxSize, boxSize, drawMode);

		// Draw checkmark if checked
		if (isChecked) {
			Color::noStroke();
			Color::fill(0, 0, 0, 255);
			float checkX = (drawMode == Shapes::CORNER) ? (x + boxSize / 2) : x;
			float checkY = (drawMode == Shapes::CORNER) ? (y - boxSize / 2) : y;
			Shapes::rect(checkX - boxSize * 0.15f, checkY, boxSize * 0.3f, boxSize * 0.3f, Shapes::CENTER);
		}

		// Draw label
		Color::textFill(0, 0, 0, 255);
		float labelX = (drawMode == Shapes::CORNER) ? (x + boxSize + 10) : (x + boxSize / 2 + 10);
		float labelY = (drawMode == Shapes::CORNER) ? (y - boxSize / 2) : y;
		Text::text(label, labelX, labelY);
	}

	// -------------------------------------------------------------------------
	// RadioButton Implementation
	// -------------------------------------------------------------------------

	RadioButton::RadioButton(float x, float y, float circleSize, char const* label, int groupId, bool initialState, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, circleSize + 100, circleSize, mode), isSelected(initialState), label(label),
		circleSize(circleSize), groupId(groupId), onSelect(nullptr) {
		// Custom default style for radio button
		style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
		style.secondaryColor = Color::createColorRGB(100, 150, 255, 255);
		style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
		style.strokeWeight = 2;

		// Add to radio group
		if (groupId >= 0 && groupId < 10) {
			radioGroups[groupId].push_back(this);
		}
	}

	RadioButton::RadioButton(void)
		: UI_Element(), isSelected(false), label("Radio"), circleSize(20), groupId(0), onSelect(nullptr) {
		style.primaryColor = Color::createColorRGB(255, 255, 255, 255);
		style.secondaryColor = Color::createColorRGB(100, 150, 255, 255);
		style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
		style.strokeWeight = 2;

		radioGroups[0].push_back(this);
	}

	RadioButton::~RadioButton() {
		// Remove from radio group
		if (groupId >= 0 && groupId < 10) {
			auto& group = radioGroups[groupId];
			group.erase(std::remove(group.begin(), group.end(), this), group.end());
		}
	}

	void RadioButton::select() {
		if (!isSelected) {
			// Deselect all other radio buttons in the same group
			if (groupId >= 0 && groupId < 10) {
				for (RadioButton* radio : radioGroups[groupId]) {
					if (radio != this) {
						radio->isSelected = false;
					}
				}
			}

			isSelected = true;
			if (onSelect != nullptr) {
				onSelect();
			}
		}
	}

	bool RadioButton::getSelected() const {
		return isSelected;
	}

	void RadioButton::setOnSelect(void (*func)(void)) {
		onSelect = func;
	}

	void RadioButton::draw(void) {
		// Check if radio button area is hovered (just the circle, not the label)
		int32_t mx = 0, my = 0;
		AEInputGetCursorPosition(&mx, &my);
		int32_t ww = AEGfxGetWindowWidth();
		int32_t wh = AEGfxGetWindowHeight();
		float worldX = mx - ww * 0.5f;
		float worldY = wh * 0.5f - my;

		bool circleHovered = false;
		if (drawMode == Shapes::CORNER) {
			float centerX = x + circleSize / 2;
			float centerY = y - circleSize / 2;
			float dist = sqrtf((worldX - centerX) * (worldX - centerX) +
				(worldY - centerY) * (worldY - centerY));
			circleHovered = (dist <= circleSize / 2);
		}
		else {
			float dist = sqrtf((worldX - x) * (worldX - x) + (worldY - y) * (worldY - y));
			circleHovered = (dist <= circleSize / 2);
		}

		// Handle click
		if (circleHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
			select();
		}

		// Draw radio button circle
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);
		Color::fill(style.primaryColor);

		if (drawMode == Shapes::CORNER) {
			Shapes::circle(x + circleSize / 2, y - circleSize / 2, circleSize, Shapes::CENTER);
		}
		else {
			Shapes::circle(x, y, circleSize, Shapes::CENTER);
		}

		// Draw inner circle if selected
		if (isSelected) {
			Color::noStroke();
			Color::fill(style.secondaryColor);
			if (drawMode == Shapes::CORNER) {
				Shapes::circle(x + circleSize / 2, y - circleSize / 2, circleSize * 0.5f, Shapes::CENTER);
			}
			else {
				Shapes::circle(x, y, circleSize * 0.5f, Shapes::CENTER);
			}
		}

		// Draw label
		Color::textFill(0, 0, 0, 255);
		float labelX = (drawMode == Shapes::CORNER) ? (x + circleSize + 10) : (x + circleSize / 2 + 10);
		float labelY = (drawMode == Shapes::CORNER) ? (y - circleSize / 2) : y;
		Text::text(label, labelX, labelY);
	}

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
		: UI_Element(0, 0, 800, 200, Shapes::CORNER), message(""), speakerName(nullptr),
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

		// set text align
		Text::textAlign(Text::LEFT, Text::TOP);
		// TODO: consider putting the alignment in the text draw function or making it an overload

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
