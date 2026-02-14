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
		defaultStyle.primaryColor = Color::CL_Color_Create(200, 200, 200, 255);
		defaultStyle.secondaryColor = Color::CL_Color_Create(100, 150, 255, 255);
		defaultStyle.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
		defaultStyle.strokeWeight = 2.0f;
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

		float textX = (drawMode == Shapes::CORNER) ? (x + width / 2) : x;
		float textY = (drawMode == Shapes::CORNER) ? (y - height / 2) : y;

		Text::text(msg, textX, textY, Text::ALIGN_CENTER);
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

	Slider::Slider(void)
		: UI_Element(), valueRef(*(new float(0))), minValue(0), maxValue(100) {
	}

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
			Color::fill(Color::CL_Color_Create(0, 255, 0, 255));
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
		style.primaryColor = Color::CL_Color_Create(255, 255, 255, 255);
		style.secondaryColor = Color::CL_Color_Create(200, 230, 255, 255);
		style.strokeColor = Color::CL_Color_Create(100, 100, 100, 255);
		style.strokeWeight = 2.0f;
	}

	TextBox::TextBox(void)
		: UI_Element(), text(""), placeholderText("Enter text..."), maxLength(50),
		  isSelected(false), cursorBlinkTimer(0.0f), showCursor(false) {
		style.primaryColor = Color::CL_Color_Create(255, 255, 255, 255);
		style.secondaryColor = Color::CL_Color_Create(200, 230, 255, 255);
		style.strokeColor = Color::CL_Color_Create(100, 100, 100, 255);
		style.strokeWeight = 2.0f;
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
			Color::stroke(Color::CL_Color_Create(0, 120, 255, 255)); // Blue when selected
			Color::strokeWeight(style.strokeWeight + 1.0f);
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
			Text::text(placeholderText.c_str(), textX, textY, Text::ALIGN_LEFT);
		}
		else {
			// Draw actual text in black
			Color::textFill(0, 0, 0, 255);
			std::string displayText = text;
			
			// Add cursor if selected
			if (isSelected && showCursor) {
				displayText += "|";
			}
			
			Text::text(displayText.c_str(), textX, textY, Text::ALIGN_LEFT);
		}
	}

	// -------------------------------------------------------------------------
	// Checkbox Implementation
	// -------------------------------------------------------------------------

	Checkbox::Checkbox(float x, float y, float boxSize, char const* label, bool initialState, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, boxSize + 100, boxSize, mode), isChecked(initialState), label(label), boxSize(boxSize), onChange(nullptr) {
		// Custom default style for checkbox
		style.primaryColor = Color::CL_Color_Create(255, 255, 255, 255);
		style.secondaryColor = Color::CL_Color_Create(100, 200, 100, 255);
		style.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
		style.strokeWeight = 2.0f;
	}

	Checkbox::Checkbox(void)
		: UI_Element(), isChecked(false), label("Checkbox"), boxSize(20), onChange(nullptr) {
		style.primaryColor = Color::CL_Color_Create(255, 255, 255, 255);
		style.secondaryColor = Color::CL_Color_Create(100, 200, 100, 255);
		style.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
		style.strokeWeight = 2.0f;
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
		Text::text(label, labelX, labelY, Text::ALIGN_LEFT);
	}

	// -------------------------------------------------------------------------
	// RadioButton Implementation
	// -------------------------------------------------------------------------

	RadioButton::RadioButton(float x, float y, float circleSize, char const* label, int groupId, bool initialState, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, circleSize + 100, circleSize, mode), isSelected(initialState), label(label), 
		  circleSize(circleSize), groupId(groupId), onSelect(nullptr) {
		// Custom default style for radio button
		style.primaryColor = Color::CL_Color_Create(255, 255, 255, 255);
		style.secondaryColor = Color::CL_Color_Create(100, 150, 255, 255);
		style.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
		style.strokeWeight = 2.0f;

		// Add to radio group
		if (groupId >= 0 && groupId < 10) {
			radioGroups[groupId].push_back(this);
		}
	}

	RadioButton::RadioButton(void)
		: UI_Element(), isSelected(false), label("Radio"), circleSize(20), groupId(0), onSelect(nullptr) {
		style.primaryColor = Color::CL_Color_Create(255, 255, 255, 255);
		style.secondaryColor = Color::CL_Color_Create(100, 150, 255, 255);
		style.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
		style.strokeWeight = 2.0f;

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
		Text::text(label, labelX, labelY, Text::ALIGN_LEFT);
	}
}

