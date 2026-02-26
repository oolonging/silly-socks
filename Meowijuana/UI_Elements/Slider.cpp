#include "UI_Elements.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Slider Implementation
	// -------------------------------------------------------------------------

	Slider::Slider(float x, float y, float width, float height, float& valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), valueRef(valRef), minValue(minVal), maxValue(maxVal) {
	}

	// TODO: ensure that the removal of the default constructor didnt break anything (for slider UI element)
	// Note to anyone reading this in the future: The re ason theres no default constructor is because one of the members is a reference and cannot be a nullpointer
	// You can either:
	// a. have a global float value that this can change which the slider can point to (not recommended) OR
	// b. change the ref member to a pointer so it can be assigned a nullptr at startup. Then disable the functionality that changes the pointed to value if the pointer is null

	void Slider::clampValue(void) {
		if (valueRef < minValue) valueRef = minValue;
		if (valueRef > maxValue) valueRef = maxValue;
	}

	void Slider::updateValue(void) {
		float worldX = Input::getMouseX();
		float worldY = Input::getMouseY();

		float relativeX = (drawMode == Shapes::CENTER) ?
			(worldX - this->x + (this->width / 2)) :
			(worldX - this->x);

		float newValue = (relativeX / this->width) * (maxValue - minValue) + minValue;
		this->valueRef = newValue;
	}

	void Slider::select(void) {
		// Deselects previously selected slider (if any)
		if (currentlySelected != nullptr && currentlySelected != this) {
			currentlySelected->deselect();
		}

		this->isSelected = true;
		currentlySelected = this;
	}

	void Slider::deselect(void) {
		this->isSelected = false;
		if (currentlySelected == this) {
			currentlySelected = nullptr;
		}
	}


	void Slider::draw(void) {
		float filledWidth = ((valueRef - minValue) / (maxValue - minValue)) * width;

		// Apply stroke
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		// Draw Background
		Color::fill(Color::Preset::Black);
		Shapes::rect(x, y, width, height, drawMode);


		if (this->isSelected) {
			updateValue();
			clampValue();

			// set color to signify selected
			Color::fill(this->style.secondaryColor);

			if (AEInputCheckReleased(AEVK_LBUTTON)) {
				this->deselect();
			}
		}
		else {
			// set color to signify unselected
			Color::fill(this->style.primaryColor);

			if (this->isHovering() && AEInputCheckCurr(AEVK_LBUTTON) && currentlySelected == nullptr) {
				this->select();
			}
		}

		// Draw Foreground
		Shapes::rect(x, y, filledWidth, height, drawMode);
	}
}
