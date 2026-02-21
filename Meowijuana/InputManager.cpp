#include "InputManager.hpp"
#include "AEEngine.h"

namespace Input {
	static float sWorldMouseX{};
	static float sWorldMouseY{};
	static float sScreenMouseX{};
	static float sScreenMouseY{};

	// bounds checker
	// TODO: reconsider the need for this function if collisions does something similar
	bool isMouseInBounds(float x, float y, float width, float height) {
		bool horizontalBounds = (sWorldMouseX >= x && sWorldMouseX <= x + width);
		bool verticalBounds = (sWorldMouseY >= y - height && sWorldMouseY <= y);

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
	}

	// debugging functions
	// TODO: in case something breaks its probably cause no import cstdio
	void printMousePosition(void) {
		printf("Screen: (%.0f, %.0f) | World: (%.2f, %.2f)\n",
			sScreenMouseX, sScreenMouseY, sWorldMouseX, sWorldMouseY);
	}
}
