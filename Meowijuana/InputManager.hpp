#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

// TODO: consider the addition of functionality to divorce AE functions from the game files and place them here so that in the event we change engines or whatever we can just have one place to modify the function calls
// for example see the following function
// bool isKeyPressed(unsigned char key) {
//	return AEInputCheckCurr(key) != 0; // only place that knows about AE engine so I wont have to change it in like 100 places
// }


namespace Input {
	// bounds checker
	bool isMouseInBounds(float x, float y, float width, float height);

	// Retrieves mouse position in world coordinates (-800.0f to 800.0f) (-450.0f to 450.0f)
	float getMouseX();
	float getMouseY();

	// Gets the mouse position in screen coordinates (0-1600) (0-900)
	float getScreenMouseX();
	float getScreenMouseY();

	// Cache functions
	void update(void);

	// debugging functions
	void printMousePosition(void);

	// TODO: mousewheeldelta (for scrolling and also another delta function for mouseMovement to get direction)
}

#endif // INPUT_MANAGER_HPP
