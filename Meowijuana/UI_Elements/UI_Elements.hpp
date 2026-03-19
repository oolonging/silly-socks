#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include "../Graphics.hpp"

struct GameData {
	std::vector<std::pair<int, int>> inventory;
	// (itemID, count)
};

namespace Entity {
	class Player;
}

namespace UI_Elements {
	// -------------------------------------------------------------------------
	// Struct definitions
	// -------------------------------------------------------------------------
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
		std::string fontName;
	} TextStyle;
	typedef struct {
		AEGfxTexture* primaryTexture;
		AEGfxTexture* secondaryTexture;
	} ElementTexture;

	// -------------------------------------------------------------------------
	// Base UI class
	// -------------------------------------------------------------------------
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
		static TextStyle getDefaultTextStyle();
		static ElementTexture getDefaultTexture();

	public:
		// Common functionality
		bool isHovering(void) const;
		void setStyle(ElementStyle newStyle) { this->style = newStyle; }
		void setTextStyle(TextStyle newStyle) { this->textStyle = newStyle; }
		void setTexture(ElementTexture newTexture) { this->texture = newTexture; }

		ElementStyle getStyle(void) const { return this->style; }
		TextStyle getTextStyle(void) const { return this->textStyle; }
		ElementTexture getTexture(void) const { return this->texture; }

		// to clean textures
		void clearTextures(void);

		// Virtual destructor for proper cleanup
		virtual ~UI_Element() = default;

		// Pure virtual - must be implemented by derived classes
		virtual void draw(void) = 0;

	protected:
		// Constructor for derived classes
		UI_Element(float x, float y, float width, float height, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		UI_Element();
	};


	// -------------------------------------------------------------------------
	// Button Class
	// -------------------------------------------------------------------------
	class Button : public UI_Element {
	private:
		char const* msg;				// The text drawn within the button
		void (*onClick)(void);			// What the button does when clicked

	public:
		void setOnClick(void (*func)(void));		// Set what to do when clicked

		// Draw override
		void draw(void) override;					// Render the button

		// Ctors
		Button(float x, float y, float width, float height, char const* msg, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		Button(Shapes::Quad quad, char const* msg, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		Button(void);
	};

	// -------------------------------------------------------------------------
	// Progress Bar class
	// -------------------------------------------------------------------------
	// TODO: CENTER render broken
	class ProgressBar : public UI_Element {
	private:
		float* value;
		float minValue;
		float maxValue;

	public:
		void clampValue(void);
		void setValue(float newValue);

		// Draw override
		void draw(void) override;

		// Ctors
		ProgressBar(float x, float y, float width, float height, float* valRef, float minVal, float maxVal, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		ProgressBar(void);
	};


	// -------------------------------------------------------------------------
	// Slider class
	// -------------------------------------------------------------------------
	// TODO: corner render is broken
	class Slider : public UI_Element {
	private:
		float* value;
		float minValue;
		float maxValue;
		bool isSelected = false;

		// slider static member
		static Slider* currentlySelected;

		void updateValue(void);

	public:
		void select();
		void deselect();

		void clampValue(void);
		void draw(void) override;

		// Ctors
		Slider(float x, float y, float width, float height, float* value, float minVal, float maxVal, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		Slider(void);
	};


	// -------------------------------------------------------------------------
	// TextBox class
	// -------------------------------------------------------------------------
	class TextBox : public UI_Element {
	private:
		std::string text;
		std::string placeholderText;
		size_t maxLength;
		bool isSelected = false; // not selected by default upon creation
		float cursorBlinkTimer;
		bool showCursor;

		// textbox static member
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


	// -------------------------------------------------------------------------
	// Checkbox class
	// -------------------------------------------------------------------------
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


	// -------------------------------------------------------------------------
	// RadioButton class
	// -------------------------------------------------------------------------
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


	// -------------------------------------------------------------------------
	// DialogueBox class
	// -------------------------------------------------------------------------
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
		DialogueBox(float x, float y, float width, float height, char const* speakerName = "speaker",
			const std::string& message = "You arent supposed to be seeing this but if you are, congrats. You broke something",
			AEGfxTexture* sprite = nullptr, Shapes::SHAPE_MODE drawMode = Shapes::CORNER);
		DialogueBox(void);
	};


	// -------------------------------------------------------------------------
	// Player UI Elements classes
	// -------------------------------------------------------------------------
	class PlayerInventory : public UI_Element {
	private:
		Entity::Player* playerRef; // Reference to player object
		int slotCount;
		float slotSize;
		float slotSpacing;
		int selectedSlot; // 0-8 for the 9 slots

		// Visual feedback
		Color::Color selectedColor;
		float selectedBorderThickness;

		void handleKeyInput();
		void drawSlot(int slotIndex, float slotX, float slotY);
		void drawSelectedIndicator(float slotX, float slotY);

	public:
		void draw(void) override;
		void update(); // Call this each frame to handle input

		// Getters/Setters
		int getSelectedSlot() const;
		float getSlotSize() const { return slotSize; } // Getting slot size to the positioning can be slightly better
		void setSelectedSlot(int slot);
		void setPlayer(Entity::Player* player);
		void setPosition(float newX, float newY); // Soz just making it so that it looks a lil nicer 

		/*void giveSeeds(Entity::Player& player);*/
		bool findItem(Entity::Player& player, int check);
		bool isEmpty(Entity::Player& player);
		void giveItem(Entity::Player& player, int itemID, int itemCount);

		void saveInventory(Entity::Player* player, GameData& gameData);
		void loadInventory(Entity::Player* player, GameData& gameData);

		// Constructors
		PlayerInventory(float x, float y, float slotSize, float slotSpacing, Entity::Player* player, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		PlayerInventory(void);

		// Destructor
		~PlayerInventory();
	};

	// -------------------------------------------------------------------------
	// PopupBox class
	// -------------------------------------------------------------------------
	class PopupBox : public UI_Element {
	private:
		bool isVisible;
		char const* title;
		char const* message;
		char const* message2;
		float width;
		float height;
		void (*onDismiss)(void);
		Button dismissButton;

	public:
		void draw(void) override;
		void show();
		void hide();
		bool getVisible() const;
		void setVisible(bool visible);
		void setTitle(char const* title);
		void setMessage(char const* message);
		void setOnDismiss(void (*func)(void));

		// Constructors
		PopupBox(float x, float y, float width, float height, char const* title, char const* message, char const* message2, bool initialState = false, Shapes::SHAPE_MODE mode = Shapes::CORNER);
		PopupBox(void);
	};
}

#endif // UI_ELEMENTS_HPP
