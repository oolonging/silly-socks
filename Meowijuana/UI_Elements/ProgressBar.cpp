#include "UI_Elements.hpp"
#include "AEEngine.h"
#include <algorithm>
#include <vector>

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// ProgressBar Static Defaults
	// -------------------------------------------------------------------------
	ElementStyle ProgressBar::defaultstyle = UI_Element::getDefaultStyle();
	TextStyle ProgressBar::defaultTextStyle = UI_Element::getDefaultTextStyle();
	ElementTexture ProgressBar::defaultTexture = UI_Element::getDefaultTexture();

	// Definitions for default setters
	void ProgressBar::setDefaultProgressBarStyle(ElementStyle newStyle) { defaultstyle = newStyle; }
	void ProgressBar::setDefaultProgressBarTextStyle(TextStyle newStyle) { defaultTextStyle = newStyle; }
	void ProgressBar::setDefaultProgressBarTexture(ElementTexture newTexture) { defaultTexture = newTexture; }

	// Definitions for default getters
	ElementStyle ProgressBar::getDefaultProgressBarStyle(void) {
		UI_Elements::ElementStyle defaultStyle;
		defaultStyle.primaryColor = Color::Preset::defaultStylePrimary;
		defaultStyle.secondaryColor = Color::Preset::defaultStyleSecondary;
		defaultStyle.strokeColor = Color::Preset::defaultStyleStroke;
		defaultStyle.strokeWeight = 0;
		return defaultStyle;
	}

	TextStyle ProgressBar::getDefaultProgressBarTextStyle(void) {
		UI_Elements::TextStyle defaultTextStyle;
		defaultTextStyle.primaryColor = Color::Preset::defaultTextStylePrimary;
		defaultTextStyle.primaryColor = Color::Preset::defaultTextStyleSecondary;
		defaultTextStyle.fontSize = 10;
		defaultTextStyle.fontName = "default";
		return defaultTextStyle;
	}

	ElementTexture ProgressBar::getDefaultProgressBarTexture(void) {
		UI_Elements::ElementTexture defaultTexture;
		defaultTexture.primaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Progressbar/primary.png");
		defaultTexture.secondaryTexture = AEGfxTextureLoad("Assets/Images/UI_Elements/Progressbar/secondary.png");
		return defaultTexture;
	}

	// Definition to clear default textures
	void ProgressBar::clearDefaultProgressBarTextures(void) {
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
	// ProgressBar Implementation
	// -------------------------------------------------------------------------

	ProgressBar::ProgressBar(float x, float y, float width, float height, float* valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode)
		: UI_Element(x, y, width, height, mode), value(valRef), minValue(minVal), maxValue(maxVal) {

		this->style = defaultstyle;
		this->textStyle = defaultTextStyle;
		this->texture = defaultTexture;

	}

	//ProgressBar::ProgressBar(void) :
	//	UI_Element(), value(0.0f), minValue(0.0f), maxValue(100.0f) {}

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
