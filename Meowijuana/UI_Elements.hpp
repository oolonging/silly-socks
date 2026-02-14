#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include <string>
#include "Graphics.hpp"

namespace UI_Elements {
	typedef struct {
		Color::CL_Color primaryColor;
		Color::CL_Color secondaryColor;
		Color::CL_Color strokeColor;
		float strokeWeight;
	} ElementStyle;

	typedef struct {
		Color::CL_Color primaryColor;
		Color::CL_Color secondaryColor;
		int fontSize;
		std::string fontPath;
	} TextStyle;

	// Base class for all UI elements
	class UI_Element {
	protected:
		float x;
		float y;
		float width;
		float height;
		Shapes::SHAPE_MODE drawMode;
		ElementStyle style;

		// Default style initialization
		static ElementStyle getDefaultStyle();

	public:
		// Common functionality
		bool isHovering(void) const;
		void setStyle(ElementStyle newStyle);
		ElementStyle getStyle(void) const;
		
		// Virtual destructor for proper cleanup
		virtual ~UI_Element() = default;

		// Pure virtual - must be implemented by derived classes
		virtual void draw(void) = 0;

	protected:
		// Constructor for derived classes
		UI_Element(float x, float y, float width, float height, Shapes::SHAPE_MODE mode);
		UI_Element();
	};

	class Button : public UI_Element {
	private:
		char const* msg;
		void (*onClick)(void);

	public:
		void setOnClick(void (*func)(void));
		void draw(void) override;

		// Constructors
		Button(float x, float y, float width, float height, char const* msg, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		Button(void);
	};

	class ProgressBar : public UI_Element {
	private:
		float value;
		float minValue;
		float maxValue;

	public:
		void clampValue(void);
		void setValue(float newValue);
		void draw(void) override;

		// Constructor
		ProgressBar(float x, float y, float width, float height, float valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		ProgressBar(void);
	};

	class Slider : public UI_Element {
	private:
		float& valueRef;
		float minValue;
		float maxValue;

		void updateValue(void);

	public:
		void clampValue(void);
		void draw(void) override;

		// Constructors
		Slider(float x, float y, float width, float height, float& valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		Slider(void);
	};

	class TextBox : public UI_Element {
	private:
		std::string text;
		std::string placeholderText;
		size_t maxLength;
		bool isSelected;
		float cursorBlinkTimer;
		bool showCursor;

		static TextBox* currentlySelected;

		void handleInput();
		void updateCursor(float deltaTime);

	public:
		void draw(void) override;
		void select();
		void deselect();
		bool getIsSelected() const;
		std::string getText() const;
		void setText(const std::string& newText);
		void setPlaceholder(const std::string& placeholder);

		// Constructors
		TextBox(float x, float y, float width, float height, const std::string& placeholder = "", size_t maxLen = 50, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		TextBox(void);
	};
}

#endif // UI_ELEMENTS_HPP