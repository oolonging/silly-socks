#include "../pch.h"
#include "CreditsScreen.hpp"

#include "../Graphics.hpp"

//global variables for credits screen
float scrollY;
float scrollSpeed;
const char* credits[] = {
	"Thank you for playing!",
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

	scrollY = -AEGfxGetWindowHeight();
	scrollSpeed = 100.0f;


}

void Credits_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });

	//move upward
	scrollY += scrollSpeed * static_cast<float>(AEFrameRateControllerGetFrameTime());

}

void Credits_Draw() {
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


