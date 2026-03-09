#include "UIManager.hpp"

namespace UIManager {
	std::unordered_map<std::string, std::unique_ptr<UI_Elements::UI_Element>> elements;

	UI_Elements::UI_Element* get(const std::string& name) {
		auto it = elements.find(name);
		return (it != elements.end()) ? it->second.get() : nullptr;
	}

	UI_Elements::Button* getButton(const std::string& name) {
		return dynamic_cast<UI_Elements::Button*>(get(name));
	}

	UI_Elements::Slider* getSlider(const std::string& name) {
		return dynamic_cast<UI_Elements::Slider*>(get(name));
	}

	UI_Elements::ProgressBar* getProgressBar(const std::string& name) {
		return dynamic_cast<UI_Elements::ProgressBar*>(get(name));
	}

	UI_Elements::TextBox* getTextBox(const std::string& name) {
		return dynamic_cast<UI_Elements::TextBox*>(get(name));
	}

	bool exists(const std::string& name) {
		return elements.find(name) != elements.end();
	}

	void remove(const std::string& name) {
		auto it = elements.find(name);
		if (it != elements.end()) {
			// Clean up textures before removing
			it->second->clearTextures();
			elements.erase(it);
		}
	}

	void drawAll(void) {
		for (auto& pair : elements) {
			pair.second->draw();
		}
	}

	void clear(void) {
		// Clean up all textures before clearing
		for (auto& pair : elements) {
			pair.second->clearTextures();
		}
		elements.clear();
	}

	void initDefaults(void) {
		// TODO: change this when we have a viable color palette
		// Initialize default button styles
		UI_Elements::Button::setDefaultButtonStyle(UI_Elements::Button::getDefaultButtonStyle());
		UI_Elements::Button::setDefaultButtonTextStyle(UI_Elements::Button::getDefaultButtonTextStyle());
		UI_Elements::Button::setDefaultButtonTexture(UI_Elements::Button::getDefaultButtonTexture());

		// Initialize default progress bar styles
		UI_Elements::ProgressBar::setDefaultProgressBarStyle(UI_Elements::ProgressBar::getDefaultProgressBarStyle());
		UI_Elements::ProgressBar::setDefaultProgressBarTextStyle(UI_Elements::ProgressBar::getDefaultProgressBarTextStyle());
		UI_Elements::ProgressBar::setDefaultProgressBarTexture(UI_Elements::ProgressBar::getDefaultProgressBarTexture());

		// Initialize default text box styles
		UI_Elements::TextBox::setDefaultTextBoxStyle(UI_Elements::TextBox::getDefaultTextBoxStyle());
		UI_Elements::TextBox::setDefaultTextBoxTextStyle(UI_Elements::TextBox::getDefaultTextBoxTextStyle());
		UI_Elements::TextBox::setDefaultTextBoxTexture(UI_Elements::TextBox::getDefaultTextBoxTexture());

	}

	void clearDefaults(void) {
		// clear button defaults
		UI_Elements::Button::clearDefaultButtonTextures();

		// clear progress bar defaults
		UI_Elements::ProgressBar::clearDefaultProgressBarTextures();

		// clear slider defaults

		// clear text box defaults
		UI_Elements::TextBox::clearDefaultTextBoxTextures();
	}
}