#include "UI_Elements.hpp"
#include "../Settings.hpp"


namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Checkbox Implementation
	// -------------------------------------------------------------------------

	Checkbox::Checkbox(float x, float y, float boxSize, char const* label, bool initialState, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, boxSize, boxSize, mode), isChecked(initialState), label(label), boxSize(boxSize), onChange(nullptr) {

		// Custom default style for checkbox
		style.primaryColor = Color::Preset::White;
		style.secondaryColor = Color::createColorRGB(100, 200, 100, 255);
		style.strokeColor = Color::createColorRGB(0, 0, 0, 255);
		style.strokeWeight = 2;
		textStyle = getDefaultTextStyle();
	}

	Checkbox::Checkbox(void)
		: Checkbox(0.0f, 0.0f, 10.0f, "Default checkbox") {}

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

		// force render mode at the start
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		// Check if checkbox area is hovered (just the box, not the label)
		bool boxHovered = isHovering();

		// Handle click
		if (boxHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
			toggle();
		}

		// Draw checkbox box
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		if (isChecked) Color::fill(style.secondaryColor);
		else Color::fill(style.primaryColor);

		Shapes::rect(x, y, boxSize, boxSize, drawMode);

		// Draw checkmark if checked
		if (isChecked) {
			Color::noStroke();
			Color::fill(Color::Preset::Black);

			float checkX = (drawMode == Shapes::CORNER) ? (x + (this->boxSize * 0.5f)) : x;
			float checkY = (drawMode == Shapes::CORNER) ? (y - (this->boxSize * 0.5f)) : y;
			Shapes::rect(checkX - boxSize * 0.15f, checkY, boxSize * 0.3f, boxSize * 0.3f, Shapes::CENTER);
		}

		// Draw label
		Color::textFill(textStyle.primaryColor);
		float labelX = (drawMode == Shapes::CORNER) ? this->x + this->width : this->x + this->width * 0.5f;
		float labelY = (drawMode == Shapes::CORNER) ? this->y : this->y + (this->height * 0.5f);

		// draw the debug material

		Text::text(label, labelX + 5.0f, labelY - (this->boxSize * 0.5f), Text::LEFT, Text::CENTER_V); // TODO: rewire the text draw so that the align points to the anchor point


	}
}
