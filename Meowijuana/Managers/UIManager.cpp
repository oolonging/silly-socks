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
		UI_Elements::ElementStyle defaultButtonStyle;
		defaultButtonStyle.primaryColor = Color::Color(Color::Preset::Red);
		defaultButtonStyle.secondaryColor = Color::Color(Color::Preset::Blue);
		defaultButtonStyle.strokeColor = Color::Color(Color::Preset::Black);
		defaultButtonStyle.strokeWeight = 2;

		UI_Elements::TextStyle defaultButtonTextStyle;
		defaultButtonTextStyle.primaryColor = Color::Color(Color::Preset::Blue);
		defaultButtonTextStyle.secondaryColor = Color::Color(Color::Preset::Red);
		defaultButtonTextStyle.fontSize = 10;
		defaultButtonTextStyle.fontName = "default";

		UI_Elements::ElementTexture defaultButtonTexture;
		defaultButtonTexture.primaryTexture = nullptr;
		defaultButtonTexture.secondaryTexture = nullptr;

		UI_Elements::Button::setDefaultStyle(defaultButtonStyle);
		UI_Elements::Button::setDefaultTextStyle(defaultButtonTextStyle);
		UI_Elements::Button::setDefaultTexture(defaultButtonTexture);

	}

	void setDefaultButtonStyle(UI_Elements::ElementStyle style) {
		UI_Elements::Button::setDefaultStyle(style);
	}

	void setDefaultButtonTextStyle(UI_Elements::TextStyle textStyle) {
		UI_Elements::Button::setDefaultTextStyle(textStyle);
	}

	void setDefaultButtonTexture(UI_Elements::ElementTexture texture) {
		UI_Elements::Button::setDefaultTexture(texture);
	}
}