#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include "../UI_Elements/UI_Elements.hpp"

namespace UIManager {
	// storage for all UI Elements
	extern std::unordered_map<std::string, std::unique_ptr<UI_Elements::UI_Element>> elements;

	// Template function to create UI elements
	template<typename T, typename... Args>
	T* create(const std::string& name, Args&&... args) {
		auto element = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = element.get();
		elements[name] = std::move(element);
		return ptr;
	}

	// Retrieval functions (just the basics for now, expand later if really necessary)
	UI_Elements::UI_Element* get(const std::string& name);
	UI_Elements::Button* getButton(const std::string& name);
	UI_Elements::Slider* getSlider(const std::string& name);
	UI_Elements::ProgressBar* getProgressBar(const std::string& name);
	UI_Elements::TextBox* getTextBox(const std::string& name);

	// Check if element exists
	bool exists(const std::string& name);

	// remove specific element
	void remove(const std::string& name);

	// Batch draw all elements
	void drawAll(void);

	// Clear all elements
	void clear(void);
}


#endif // UI_MANAGER_HPP
