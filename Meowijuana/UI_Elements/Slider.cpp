#include <iostream>
#include "UI_Elements.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Slider Implementation
	// -------------------------------------------------------------------------
	
	// Initialize staic members
	AEGfxTexture* Slider::defaultHandleTexture = nullptr; // Note the destination for these are in : "Assets/Images/UI_Elements/Slider"
	AEGfxTexture* Slider::defaultBarTexture = nullptr;

	// static function to manage default textures
	void Slider::loadDefaultTextures() {
		if (defaultHandleTexture == nullptr) {
			defaultHandleTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/slider_handle.png");
		}
		if (defaultBarTexture == nullptr) {
			defaultBarTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/slider_bar.png");
		}
	}

	void Slider::unloadDefaultTextures() {
		if (defaultHandleTexture != nullptr) {
			AEGfxTextureUnload(defaultHandleTexture);
			defaultHandleTexture = nullptr;
		}
		if (defaultBarTexture != nullptr) {
			AEGfxTextureUnload(defaultBarTexture);
			defaultBarTexture = nullptr;
		}
	}

	Slider::Slider(float x, float y, float width, float height, float& valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), valueRef(valRef), minValue(minVal), maxValue(maxVal) {

		// use default textures
		if(texture.primaryTexture == nullptr) {
			texture.primaryTexture = defaultHandleTexture;
		}
		if (texture.secondaryTexture == nullptr) {
			texture.secondaryTexture = defaultBarTexture;
		}

		if(texture.primaryTexture == nullptr || texture.secondaryTexture == nullptr) {
			std::cout << "Textures for slider havent loaded" << std::endl;
		}
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
		// Reminder: primary texture is for the handle, secondary texture is for the bar
		float filledWidth = ((valueRef - minValue) / (maxValue - minValue)) * width;
		bool graphicalRender = (this->texture.primaryTexture != nullptr && this->texture.secondaryTexture != nullptr);

		// Apply stroke
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		// Draw Background
		if (graphicalRender) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(this->texture.secondaryTexture, 0, 0);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Color::fill(this->style.secondaryColor);
		}
		Shapes::rect(x, y, width, height, drawMode);

		// variables for handle
		float handleX = (drawMode == Shapes::CENTER) ? (x - width / 2 + filledWidth) : (x + filledWidth);
		float handleY = (drawMode == Shapes::CENTER) ? y : (y - height / 2);
		float handleSize = height * 1.5f; // Handle size can be adjusted as needed


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
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f); // Comment this out if you want the highlight when selected effect
			AEGfxTextureSet(this->texture.primaryTexture, 0, 0);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Color::fill(this->style.primaryColor);
		}
		Shapes::ellipse(handleX, handleY, handleSize, handleSize, Shapes::CENTER);
	}
}
