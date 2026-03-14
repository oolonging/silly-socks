#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// ProgressBar Implementation
	// -------------------------------------------------------------------------

	ProgressBar::ProgressBar(float x, float y, float width, float height, float* valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), value(valRef), minValue(minVal), maxValue(maxVal) {

		this->style = getDefaultStyle();
		this->textStyle = getDefaultTextStyle();
		
		// Default textures
		this->texture.primaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/primary.png");
		this->texture.secondaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/secondary.png");
	}

	ProgressBar::ProgressBar(void)
		: ProgressBar(0.0f, 0.0f, 100.0f, 10.0f, nullptr, 0.0f, 100.0f) {}

	void ProgressBar::clampValue(void) {
		if (*value < minValue) *value = minValue;
		if (*value > maxValue) *value = maxValue;
	}

	void ProgressBar::setValue(float newValue) {
		*value = newValue;
	}

	//void ProgressBar::draw(void) {
	//	// clamp value
	//	clampValue();

	//	float filledWidth = (this->value / maxValue) * width;

	//	// Apply stroke
	//	Color::stroke(style.strokeColor);
	//	Color::strokeWeight(style.strokeWeight);

	//	// Background
	//	Color::fill(style.primaryColor);
	//	Shapes::rect(x, y, width, height, drawMode);

	//	// Foreground
	//	Color::fill(style.secondaryColor);
	//	Shapes::rect(x, y, filledWidth, height, drawMode);
	//}

	// Improved draw with graphical render
	void ProgressBar::draw(void) {
		clampValue();
		bool graphicalRender = (this->texture.primaryTexture != nullptr && this->texture.secondaryTexture != nullptr);

		float filledWidth = (*value / this->maxValue) * this->width;


		// Apply styles (Stroke)
		Color::stroke(style.strokeColor);
		Color::strokeWeight(style.strokeWeight);

		// Draw background
		if (graphicalRender) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxTextureSet(this->texture.secondaryTexture, 0, 0);
			Shapes::rect(x, y, width, height, drawMode);
			AEGfxTextureSet(this->texture.primaryTexture, 0, 0);
			Shapes::rect(x, y, filledWidth, height, drawMode);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Color::fill(this->style.secondaryColor);
			Shapes::rect(x, y, width, height, drawMode);
			Color::fill(this->style.primaryColor);
			Shapes::rect(x, y, filledWidth, height, drawMode);
		}
	}

}
