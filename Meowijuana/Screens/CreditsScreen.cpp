#include "../pch.h"
#include "CreditsScreen.hpp"

#include "../Graphics.hpp"
#include "../Settings.hpp"
#include "../GameStateManager.hpp"

//global variables for credits screen
float scrollY;
float scrollSpeed;

s32 mouseWheelDelta{};

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
	Settings::currentScreen = "CreditsScreen.cpp";
}

void Credits_Initialize() {
	scrollY = -(AEGfxGetWindowHeight() * 0.5f) - (creditsCount * 100.0f);
	scrollSpeed = 100.0f;
}

void Credits_Update() {
	//move upward
	scrollY += scrollSpeed * static_cast<float>(AEFrameRateControllerGetFrameTime());

	// if the mouse wheel is being scrolled, change the speed
	if (mouseWheelDelta > 0) scrollSpeed = -400.0f;
	if (mouseWheelDelta < 0) scrollSpeed = 400.0f;
	if (mouseWheelDelta == 0) scrollSpeed = 100.0f;

	// get mousewheel delta
	AEInputMouseWheelDelta(&mouseWheelDelta);

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

	// Click anywhere to return to main menu
	if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_E)) {
		next = GS_MAIN_MENU;
	}
}

// No assets to clear out yet. TODO: potentially add some images
void Credits_Free() {}
void Credits_Unload() {}


