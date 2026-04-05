/**
 * @file UIManager.hpp
 * @author Saahil
 * @brief Manages creation, retrieval, rendering, and cleanup of all UI elements.
 */

#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include "../UI_Elements/UI_Elements.hpp"

 /**
  * @namespace UIManager
  * @brief Centralized manager for creating, storing, and drawing UI elements by name.
  */
namespace UIManager {
	/// @brief Storage map of all UI elements, keyed by name.
	extern std::unordered_map<std::string, std::unique_ptr<UI_Elements::UI_Element>> elements;

	/**
	 * @brief Creates a UI element of type T and stores it under the given name.
	 * @tparam T The UI element type to create (must derive from UI_Elements::UI_Element).
	 * @tparam Args Constructor argument types.
	 * @param name Unique name to register the element under.
	 * @param args Arguments forwarded to T's constructor.
	 * @return Pointer to the newly created element.
	 */
	template<typename T, typename... Args>
	T* create(const std::string& name, Args&&... args) {
		auto element = std::make_unique<T>(std::forward<Args>(args)...);
		T* ptr = element.get();
		elements[name] = std::move(element);
		return ptr;
	}

	/**
	 * @brief Retrieves a UI element by name.
	 * @param name Name of the element.
	 * @return Pointer to the UI_Element, or nullptr if not found.
	 */
	UI_Elements::UI_Element* get(const std::string& name);

	/**
	 * @brief Retrieves a Button element by name.
	 * @param name Name of the button.
	 * @return Pointer to the Button, or nullptr if not found or wrong type.
	 */
	UI_Elements::Button* getButton(const std::string& name);

	/**
	 * @brief Retrieves a Slider element by name.
	 * @param name Name of the slider.
	 * @return Pointer to the Slider, or nullptr if not found or wrong type.
	 */
	UI_Elements::Slider* getSlider(const std::string& name);

	/**
	 * @brief Retrieves a ProgressBar element by name.
	 * @param name Name of the progress bar.
	 * @return Pointer to the ProgressBar, or nullptr if not found or wrong type.
	 */
	UI_Elements::ProgressBar* getProgressBar(const std::string& name);

	/**
	 * @brief Retrieves a TextBox element by name.
	 * @param name Name of the text box.
	 * @return Pointer to the TextBox, or nullptr if not found or wrong type.
	 */
	UI_Elements::TextBox* getTextBox(const std::string& name);

	/**
	 * @brief Checks whether a UI element with the given name exists.
	 * @param name Name to look up.
	 * @return True if the element exists.
	 */
	bool exists(const std::string& name);

	/**
	 * @brief Removes and destroys a specific UI element by name.
	 * @param name Name of the element to remove.
	 */
	void remove(const std::string& name);

	/**
	 * @brief Draws all registered UI elements.
	 */
	void drawAll(void);

	/**
	 * @brief Removes and destroys all registered UI elements.
	 */
	void clear(void);
}


#endif // UI_MANAGER_HPP