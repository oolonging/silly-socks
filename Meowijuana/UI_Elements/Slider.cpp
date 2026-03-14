#include <iostream>
#include "UI_Elements.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Slider Implementation
	// -------------------------------------------------------------------------
	
	Slider::Slider(float x, float y, float width, float height, float* value, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), value(value), minValue(minVal), maxValue(maxVal) {

		this->style = getDefaultStyle();
		this->textStyle = getDefaultTextStyle();
		
		// Default texture
		this->texture.primaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/primary.png");
		this->texture.secondaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/secondary.png");
	}

	Slider::Slider(void) : Slider(0.0f, 0.0f, 100.0f, 10.0f, nullptr, 0.0f, 100.0f) {}

	void Slider::clampValue(void) {
		if (*value < minValue) *value = minValue;
		if (*value > maxValue) *value = maxValue;
	}

	void Slider::updateValue(void) {
		float worldX = Input::getMouseX();
		float worldY = Input::getMouseY();

		float relativeX = (drawMode == Shapes::CENTER) ?
			(worldX - this->x + (this->width / 2)) :
			(worldX - this->x);

		float newValue = (relativeX / this->width) * (maxValue - minValue) + minValue;
		*(this->value) = newValue;
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
		// Reminder: primary texture is for the handle, secondary texture is for the bar
		float filledWidth = ((*value - minValue) / (maxValue - minValue)) * width;
		bool graphicalRender = (this->texture.primaryTexture != nullptr && this->texture.secondaryTexture != nullptr);

		// Draw Background
		if (graphicalRender) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 0.0f);
			AEGfxTextureSet(this->texture.secondaryTexture, 0, 0);
			Color::noStroke();
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Color::fill(this->style.secondaryColor);
			Color::stroke(style.strokeColor);
			Color::strokeWeight(style.strokeWeight);
		}
		Shapes::rect(x, y, width, height, drawMode);

		// variables for handle
		float handleX = (drawMode == Shapes::CENTER) ? (x - width / 2 + filledWidth) : (x + filledWidth);
		float handleY = (drawMode == Shapes::CENTER) ? y : (y - height / 2);
		float handleSize = height * 1.1f; // Handle size can be adjusted as needed


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

			// Controls when the slider is selected
			//if (this->isHovering() && AEInputCheckCurr(AEVK_LBUTTON) && currentlySelected == nullptr) {
			//	this->select();
			//}
			
			float dx = Input::getMouseX() - handleX;
			float dy = Input::getMouseY() - handleY;
			float dist = sqrtf(dx * dx + dy * dy);
			// Instead of ishovering, it checks if the mouse is inside the handle
			if (
				(dist <= (handleSize / 2)) &&
				AEInputCheckCurr(AEVK_LBUTTON)&&
				currentlySelected == nullptr
				) {
				this->select();
			}
		}

		if (graphicalRender) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_NONE);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
			Color::fill(Color::Preset::White);
			AEGfxTextureSet(this->texture.primaryTexture, 0, 0);
			Color::noStroke();
			Graphics::image(handleX, handleY, handleSize, handleSize, texture.primaryTexture, Shapes::CENTER);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Color::fill(this->style.primaryColor);
			Shapes::ellipse(handleX, handleY, handleSize, handleSize, Shapes::CENTER);
		}
	}
}
