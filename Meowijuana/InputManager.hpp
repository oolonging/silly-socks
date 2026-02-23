#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

// TODO: consider the addition of functionality to divorce AE functions from the game files and place them here so that in the event we change engines or whatever we can just have one place to modify the function calls
// for example see the following function
// bool isKeyPressed(unsigned char key) {
//	return AEInputCheckCurr(key) != 0; // only place that knows about AE engine so I wont have to change it in like 100 places
// }

#include <deque>
#include <vector>


namespace Input {
	// input buffer
	constexpr size_t MAX_BUFFER_SIZE = 10;
	extern std::deque<unsigned char> keyHistory;

	// bounds checker
	bool isMouseInBounds(float x, float y, float width, float height);

	// Retrieves mouse position in world coordinates (-800.0f to 800.0f) (-450.0f to 450.0f)
	float getMouseX();
	float getMouseY();

	// Gets the mouse position in screen coordinates (0-1600) (0-900)
	float getScreenMouseX();
	float getScreenMouseY();

	// custom cursor functions
	void init(void);
	void exit(void);

	// Cache functions
	void update(void);

	// Key sequence tracking
	// TODO: nothing, just noticed char key sounds like car key if you say it incorrectly
	void recordKey(unsigned char key);
	bool checkSequence(const std::vector<unsigned char>& sequence); // TODO: actually create the sequences to dump through here
	void clearHistory(); // Me when I end up in the emergency room

	// debugging functions
	void printMousePosition(void);
	void printKeyHistory(void);

	// TODO: mousewheeldelta (for scrolling and also another delta function for mouseMovement to get direction)
}

#endif // INPUT_MANAGER_HPP
