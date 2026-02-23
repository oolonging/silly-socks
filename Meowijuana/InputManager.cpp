#include "InputManager.hpp"
#include "AEEngine.h"
#include "AEInput.h"
#include "Graphics.hpp"

namespace Input {
	// input buffer
	std::deque<unsigned char> keyHistory{};
	static bool sPrevKeyStates[256]{}; // Track prev frame key states to detect keypress (and not hold)

	// custom cursor
	static AEGfxTexture* customCursor = nullptr;

	static float sWorldMouseX{};
	static float sWorldMouseY{};
	static float sScreenMouseX{};
	static float sScreenMouseY{};

	// bounds checker
	// TODO: reconsider the need for this function if collisions does something similar
	bool isMouseInBounds(float x, float y, float width, float height) {
		bool horizontalBounds = (sScreenMouseX >= x && (sScreenMouseX <= x + width));
		bool verticalBounds = (sScreenMouseY >= y && (sScreenMouseY <= y + height));

		return horizontalBounds && verticalBounds;
	}

	// World coordinate functions
	float getMouseX() {
		return sWorldMouseX;
	}

	float getMouseY() {
		return sWorldMouseY;
	}

	float getScreenMouseX() {
		return sScreenMouseX;
	}

	float getScreenMouseY() {
		return sScreenMouseY;
	}

	void recordKey(unsigned char key) {
		keyHistory.push_back(key);

		// keep buffer at maximum size
		if (keyHistory.size() > MAX_BUFFER_SIZE) {
			keyHistory.pop_front();
		}
	}

	bool checkSequence(const std::vector<unsigned char>& sequence) {
		// if the sequence is unset or if in the unlikely event the sequence is longer than the history size
		// TODO: maybe dynamically allocate the memory based on the sequence size, but we'll have to first decide on the sequence at least
		if (sequence.empty() || sequence.size() > keyHistory.size()) {
			return false;
		}

		auto historyStart = keyHistory.end() - sequence.size();
		return std::equal(sequence.begin(), sequence.end(), historyStart); // TODO: a bit shady, but I need a better way to compare since it could be a ooooxxxxxx kind of situation
	}

	void clearHistory() {
		keyHistory.clear();
	}

	// custom cursor functions
	void init(void) {
		customCursor = AEGfxTextureLoad("Assets/Images/Cursors/Pointer.png");
	}

	void exit(void) {
		AEGfxTextureUnload(customCursor);
		customCursor = nullptr;
	}

	void update() {
		s32 screenX{};
		s32 screenY{};

		AEInputGetCursorPosition(&screenX, &screenY);
		sWorldMouseX = screenX - (AEGfxGetWindowWidth() * 0.5f);
		sWorldMouseY = screenY - (AEGfxGetWindowHeight() * 0.5f);
		sScreenMouseX = screenX;
		sScreenMouseY = screenY;

		// and then negate the sWorldMouseY (TODO: not the most elegant solution but it works for now)
		sWorldMouseY = -sWorldMouseY;

		// testing stuff
		bool isMouseInWindow = isMouseInBounds(0, 0, AEGfxGetWindowWidth(), AEGfxGetWindowHeight());

		// if it is then hide the mouse
		if(isMouseInWindow) {
			//AESysShowCursor(false);
			AEInputShowCursor(false);

			// instead of a cursor just show a blue ellipse
			if (customCursor) {
				Graphics::image(sWorldMouseX, sWorldMouseY, 32.0f, 32.0f, customCursor);
			}
			else {
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				Color::fill(255.0f, 0.0f, 0.0f, 255.0f * 0.5f);
				Shapes::ellipse(sWorldMouseX, sWorldMouseY, 30, 30);
				Color::fill(0.0f, 0.0f, 255.0f, 255.0f);
				Shapes::ellipse(sWorldMouseX, sWorldMouseY, 10, 10);
			}
		}
		else {
			AEInputShowCursor(true);
		}

		// track all possible keys for new presses
		// TODO: this tracks mouse inputs as well so that messes with what I had in mind, index the keys and exclude manually later
		for (int key = 0; key < 256; key++) {
			bool currentState = AEInputCheckCurr(static_cast<unsigned char>(key)) != 0;
			bool wasPressed = !sPrevKeyStates[key] && currentState;

			if (wasPressed) {
				recordKey(static_cast<unsigned char>(key));
			}

			sPrevKeyStates[key] = currentState;
		}

		// just to quickly debug
		if (AEInputCheckTriggered(AEVK_LBUTTON)) {
			printMousePosition();
			printKeyHistory();
		}
	}

	// debugging functions
	// TODO: in case something breaks its probably cause no import cstdio
	void printMousePosition(void) {
		printf("Screen: (%.0f, %.0f) | World: (%.2f, %.2f)\n",
			sScreenMouseX, sScreenMouseY, sWorldMouseX, sWorldMouseY);
	}

	void printKeyHistory() {
		printf("Key History: ");
		for (unsigned char key : keyHistory) {
			printf("%c", key);
		}
		printf("\n");
	}
}
