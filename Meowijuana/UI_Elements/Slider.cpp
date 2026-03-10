#include <iostream>
#include "UI_Elements.hpp"

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// ProgressBar Static Defaults
	// -------------------------------------------------------------------------
	ElementStyle Slider::defaultStyle = UI_Element::getDefaultStyle();
	TextStyle Slider::defaultTextStyle = UI_Element::getDefaultTextStyle();
	ElementTexture Slider::defaultTexture = UI_Element::getDefaultTexture();

	// Definitions for default setters
	void Slider::setDefaultSliderStyle(ElementStyle newStyle) { defaultStyle = newStyle; }
	void Slider::setDefaultSliderTextStyle(TextStyle newStyle) { defaultTextStyle = newStyle; }
	void Slider::setDefaultSliderTexture(ElementTexture newTexture) { defaultTexture = newTexture; }

	// Definitions for default setters
	ElementStyle Slider::getDefaultSliderStyle(void) {
		UI_Elements::ElementStyle defaultStyle;
		defaultStyle.primaryColor = Color::Preset::defaultStylePrimary;
		defaultStyle.secondaryColor = Color::Preset::defaultStyleSecondary;
		defaultStyle.strokeColor = Color::Preset::defaultStyleStroke;
		defaultStyle.strokeWeight = 1;
		return defaultStyle;
	}

	TextStyle Slider::getDefaultSliderTextStyle(void) {
		UI_Elements::TextStyle defaultTextStyle;
		defaultTextStyle.primaryColor = Color::Preset::defaultTextStylePrimary;
		defaultTextStyle.primaryColor = Color::Preset::defaultTextStyleSecondary;
		defaultTextStyle.fontSize = 10;
		defaultTextStyle.fontName = "default";
		return defaultTextStyle;
	}

	ElementTexture Slider::getDefaultSliderTexture(void) {
		UI_Elements::ElementTexture defaultTexture;
		defaultTexture.primaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/primary.png");
		defaultTexture.secondaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/secondary.png");
		return defaultTexture;
	}

	void Slider::clearDefaultSliderTextures(void) {
		if (defaultTexture.primaryTexture != nullptr) {
			AEGfxTextureUnload(defaultTexture.primaryTexture);
			defaultTexture.primaryTexture = nullptr;
		}

		if (defaultTexture.secondaryTexture != nullptr) {
			AEGfxTextureUnload(defaultTexture.secondaryTexture);
			defaultTexture.secondaryTexture = nullptr;
		}
	}


	// -------------------------------------------------------------------------
	// Slider Implementation
	// -------------------------------------------------------------------------
	
	Slider::Slider(float x, float y, float width, float height, float* value, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), value(value), minValue(minVal), maxValue(maxVal) {

		this->style = defaultStyle;
		this->textStyle = defaultTextStyle;
		this->texture = defaultTexture;
	}

	Slider::Slider(void) : Slider(0.0f, 0.0f, 100.0f, 10.0f, nullptr, 0.0f, 100.0f) {}

	// TODO: ensure that the removal of the default constructor didnt break anything (for slider UI element)
	// Note to anyone reading this in the future: The re ason theres no default constructor is because one of the members is a reference and cannot be a nullpointer
	// You can either:
	// a. have a global float value that this can change which the slider can point to (not recommended) OR
	// b. change the ref member to a pointer so it can be assigned a nullptr at startup. Then disable the functionality that changes the pointed to value if the pointer is null

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
