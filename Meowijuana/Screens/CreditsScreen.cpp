/*
 * @file CreditsScreen.cpp
 * @author Jun
 * @brief Credit Screen
 */

#include "../pch.h"
#include "CreditsScreen.hpp"

#include "../Graphics.hpp"
#include "../Settings.hpp"
#include "../GameStateManager.hpp"

//global variables for credits screen
float scrollY;
float scrollSpeed;

s32 mouseWheelDelta{};

AEGfxTexture* creditsLogo = nullptr;
AEGfxVertexList* creditsMesh = nullptr;

const char* credits[] = {
	"Thank you for playing!",
	" ",
	"All available at https://www.kenney.nl/",
	"pixel-platformer  || KENNEY",
	"tiny-dungeon      || KENNEY",
	"tiny-town         || KENNEY",
	"Graphics",
	" ",
	"piglevelwin2 | Tuudurt (FreeSound)",
	"Pixel Sound Effect #4 | hmmm101 (FreeSound)",
	"Enchanted Valley | ondrosik",
	"Audio",
	" ",
	"Tommy",
	"Soroor",
	"Gerald",
	"Instructors",
	" ",
	"Claude Comair",
	"President",
	" ",
	"Yu Xuan",
	"Saahil",
	"Jun",
	"Andrea",
	"Silly Socks Team Members",
};

int creditsCount = sizeof(credits) / sizeof(credits[0]);

void Credits_Load() {

	creditsLogo = AEGfxTextureLoad("Assets/Images/Logos/Digipen_White.png");

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00FFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0x00FFFFFF, 0.0f, 0.0f
	);
	AEGfxTriAdd(
		0.5f, -0.5f, 0x00FFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0x00FFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0x00FFFFFF, 0.0f, 0.0f
	);
	creditsMesh = AEGfxMeshEnd();

}

void Credits_Initialize() {
	Settings::currentScreen = "CreditsScreen.cpp";

	scrollY = -(AEGfxGetWindowHeight() * 0.5f) - (creditsCount * 100.0f) - 300.0f;
	scrollSpeed = 100.0f;
}

void Credits_Update() {

	//move upward
	scrollY += scrollSpeed * static_cast<float>(AEFrameRateControllerGetFrameTime());

	//if the mouse wheel is being scrolled, change the speed
	if (mouseWheelDelta > 0) scrollSpeed = -400.0f;
	if (mouseWheelDelta < 0) scrollSpeed = 400.0f;
	if (mouseWheelDelta == 0) scrollSpeed = 100.0f;

	//get mousewheel delta
	AEInputMouseWheelDelta(&mouseWheelDelta);

}

void Credits_Draw() {

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//draw logo
	if (creditsLogo) {
		float imageWidth = 700.0f;
		float imageHeight = 200.0f;
		float imageY = scrollY + (creditsCount * 100.0f) + 150.0f;

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(creditsLogo, 0, 0);
		AEGfxSetTransparency(1.0f);
		AEGfxSetBlendColor(0.0f, 0.0f, 0.0f, 0.0f);

		AEMtx33 transform;
		AEMtx33Scale(&transform, imageWidth, imageHeight);
		AEMtx33TransApply(&transform, &transform, 0, imageY);
		AEGfxSetTransform(transform.m);

		AEGfxMeshDraw(creditsMesh, AE_GFX_MDM_TRIANGLES);

		// Restore color mode for text
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		Color::textFill(255, 255, 255);
	}

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::textSize(20.0f);

	// Draw each line
	for (int i = 0; i < creditsCount; i++) {
		float yPos = scrollY + i * 100; //spacing between lines
		Text::text(credits[i], 0, yPos, Text::CENTER_H, Text::CENTER_V);
	}

	// Click anywhere to return to main menu
	if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_E)) {
		next = GS_MAIN_MENU;
	}
}

void Credits_Free() {
	if (creditsLogo) {
		AEGfxTextureUnload(creditsLogo);
		creditsLogo = nullptr;
	}
	if (creditsMesh) {
		AEGfxMeshFree(creditsMesh);
		creditsMesh = nullptr;
	}
}

void Credits_Unload() {}


