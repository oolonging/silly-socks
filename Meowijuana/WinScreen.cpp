#include "pch.h"
#include "WinScreen.hpp"
#include "GameStateManager.hpp"
#include "AudioManager.hpp"

namespace WinScreen {

	AEGfxTexture* winScreen;
	AEGfxVertexList* mesh;
	AudioManager::Audio winAudio;
}


void Win_Load() {

	// load win screen texture
	WinScreen::winScreen = AEGfxTextureLoad("Assets/Images/Backgrounds/Win Screen.png");

	// Load win sound
	WinScreen::winAudio.init();
	WinScreen::winAudio.loadSFX("Assets/Sounds/win.mp3");
}

void Win_Initialize() {
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);
	AEGfxTriAdd(
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	WinScreen::mesh = AEGfxMeshEnd();

	//play win sound
	WinScreen::winAudio.playSFX(1.0f);
}

void Win_Update() {

}

void Win_Draw() {

	AEMtx33 scale, trans, transform;

	AEMtx33Scale(&scale, (f32)AEGfxGetWindowWidth(), (f32)AEGfxGetWindowHeight());
	AEMtx33Trans(&trans, 0.0f, 0.0f);
	AEMtx33Concat(&transform, &trans, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(WinScreen::winScreen, 0.0f, 0.0f);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(WinScreen::mesh, AE_GFX_MDM_TRIANGLES);

	// Click anywhere to return to main menu
	if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_E)) {
		next = GS_MAIN_MENU;
	}
}

void Win_Free() {

	AEGfxMeshFree(WinScreen::mesh);
}

void Win_Unload() {

	AEGfxTextureUnload(WinScreen::winScreen);
	WinScreen::winAudio.exit();
}
