#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include <string>
#include <vector>
#include "Graphics.hpp"

namespace UI_Elements {
	typedef struct {
		Color::Color primaryColor;
		Color::Color secondaryColor;
		Color::Color strokeColor;
		int strokeWeight;
	} ElementStyle;

	typedef struct {
		Color::Color primaryColor;
		Color::Color secondaryColor;
		int fontSize;
		std::string fontPath;
	} TextStyle;

	typedef struct {
		AEGfxTexture* primaryTexture;
		AEGfxTexture* secondaryTexture;
	} ElementTexture;

	// Base class for all UI elements
	class UI_Element {
	protected:
		float x;
		float y;
		float width;
		float height;
		Shapes::SHAPE_MODE drawMode;
		ElementStyle style;
		ElementTexture texture;
		TextStyle textStyle;

		// Default style initialization
		static ElementStyle getDefaultStyle();
		static ElementTexture getDefaultTexture(); // TODO: define
		static TextStyle getDefaultTextStyle(); // TODO: define

	public:
		// Common functionality
		bool isHovering(void) const;
		void setStyle(ElementStyle newStyle);
		ElementStyle getStyle(void) const;
		ElementTexture getTexture(void) const;

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
		TextStyle textStyle;

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

	// TODO: Corner render is broken
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

	class Checkbox : public UI_Element {
	private:
		bool isChecked;
		char const* label;
		float boxSize;
		void (*onChange)(bool);

	public:
		void draw(void) override;
		void toggle();
		bool getChecked() const;
		void setChecked(bool checked);
		void setOnChange(void (*func)(bool));

		// Constructors
		Checkbox(float x, float y, float boxSize, char const* label, bool initialState = false, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		Checkbox(void);
	};

	class RadioButton : public UI_Element {
	private:
		bool isSelected;
		char const* label;
		float circleSize;
		int groupId;
		void (*onSelect)(void);

		static std::vector<RadioButton*> radioGroups[10]; // Support up to 10 groups

	public:
		void draw(void) override;
		void select();
		bool getSelected() const;
		void setOnSelect(void (*func)(void));

		// Constructors
		RadioButton(float x, float y, float circleSize, char const* label, int groupId = 0, bool initialState = false, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		RadioButton(void);
		~RadioButton();
	};

	class DialogueBox : public UI_Element {
		private:
			std::string message;

			// Entity stuff
			char const* speakerName;
			AEGfxTexture* characterSprite;

			bool showSprite; // The sprite should be optional in case nobody is speaking
			bool isActive; // TODO: potentially remove this, im including this just in case

			// Misc designer stuff
			float spriteSize;
			float textPadding;
			float nameBoxHeight;

			// When the person clicks 
			void (*onDismiss)(void);

			// Temporary wrapping helper
			std::vector<std::string> wrapText(const std::string& text, float maxWidth);

	public:
		void draw(void) override;
		void setText(const std::string& text);
		void setSpeaker(char const* name);
		void setCharacterSprite(AEGfxTexture* pSprite);
		void setShowSprite(bool showSprite);
		void activate();
		void dismiss();
		bool getIsActive() const;
		void setOnDismiss(void(*func)(void));

		// Ctors
		DialogueBox(float x, float y, float width, float height, char const* speakerName = nullptr,
			const std::string& message = "You arent supposed to be seeing this but if you are, congrats. You broke something",
			AEGfxTexture* sprite = nullptr, Shapes::SHAPE_MODE drawMode = Shapes::CORNER);
		DialogueBox(void);
	};
}

#endif // UI_ELEMENTS_HPP




