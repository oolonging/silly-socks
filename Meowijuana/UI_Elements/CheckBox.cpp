#include "../pch.h"
#include "UI_Elements.hpp"


namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Checkbox Implementation
	// -------------------------------------------------------------------------

	Checkbox::Checkbox(float x, float y, float boxSize, char const* label, bool initialState, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, boxSize, boxSize, mode), isChecked(initialState), label(label), boxSize(boxSize), onChange(nullptr) {

		this->style = getDefaultStyle();
		this->textStyle = getDefaultTextStyle();

		// Default textures
		// TODO: maybe add default textures?
		//this->texture.primaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Checkbox/primary.png");
		//this->texture.secondaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Checkbox/secondary.png");
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
		bool hovering = isHovering();
		bool graphicalRender = (this->texture.primaryTexture != nullptr) && (this->texture.secondaryTexture != nullptr);

		// Handle click
		if (hovering && AEInputCheckTriggered(AEVK_LBUTTON)) {
			toggle();
		}

		// Draw checkbox box
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		if (isChecked) Color::fill(style.secondaryColor);
		else Color::fill(style.primaryColor);

		Shapes::rect(x, y, boxSize, boxSize, drawMode);

		// Draw checkmark if checked
		if (isChecked) {
			Color::fill(Color::Preset::Black);
			float centerX = (this->drawMode == Shapes::CORNER) ? (this->x + this->width * 0.5f) : this->x;
			float centerY = (this->drawMode == Shapes::CORNER) ? (this->y - this->height * 0.5f) : this->y;

			rect(centerX, centerY, boxSize * 0.5f, boxSize * 0.5f, Shapes::CENTER);
		}

		// Draw label
		Color::textFill(textStyle.primaryColor);
		Text::textSize(this->textStyle.fontSize);
		float labelX = (drawMode == Shapes::CORNER) ? this->x + this->width : this->x + this->width * 0.5f;
		float labelY = (drawMode == Shapes::CORNER) ? this->y : this->y + (this->height * 0.5f);

		// draw the debug material
		Text::text(label, labelX + 5.0f, labelY - (this->boxSize * 0.5f), Text::LEFT, Text::CENTER_V); // TODO: rewire the text draw so that the align points to the anchor point

		Shapes::debugRect(this->x, this->y, this->width, this->height, this->drawMode);
	}
}


