#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "CreditsScreen.hpp"

//global variables for credits screen
float scrollY;
float scrollSpeed;
const char* credits[] = {
	"Thank you for playing!",
	" ",
	"Pixel Sound Effect #4 | hmmm101 (FreeSound)",
	"Enchanted Valley | ondrosik",
	"Audio",
	" ",
	"Yu Xuan",
	"Saahil",
	"Jun",
	"Andrea",
	"Silly Socks",
};

int creditsCount = sizeof(credits) / sizeof(credits[0]);

void Credits_Load() {

}

void Credits_Initialize() {

	scrollY = -(AEGfxGetWindowHeight() * 0.5f) - (creditsCount * 100.0f);
	scrollSpeed = 100.0f;


}

void Credits_Update() {

	//move upward
	scrollY += scrollSpeed * static_cast<float>(AEFrameRateControllerGetFrameTime());

}

void Credits_Draw() {

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::textSize(20.0f);

	// Draw each line
	for (int i = 0; i < creditsCount; i++) {
		float yPos = scrollY + i * 100; //spacing between lines
		Text::text(credits[i], 0, yPos);
	}
}

void Credits_Free() {

}

void Credits_Unload() {

}
