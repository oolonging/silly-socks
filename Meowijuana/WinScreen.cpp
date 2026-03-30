#include "pch.h"
#include "WinScreen.hpp"

#include "Managers/ParticleManager.hpp"
#include "GameStateManager.hpp"
#include "AudioManager.hpp"
#include "Settings.hpp"
#include "World.hpp"

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
	Settings::currentScreen = "WinScreen.cpp";

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

	int screenWidth = AEGfxGetWindowWidth();
	int screenHeight = AEGfxGetWindowHeight();

	AEMtx33 scale, trans, transform;

	AEMtx33Scale(&scale, (f32)screenWidth, (f32)screenHeight);
	AEMtx33Trans(&trans, 0.0f, 0.0f);
	AEMtx33Concat(&transform, &trans, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(WinScreen::winScreen, 0.0f, 0.0f);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(WinScreen::mesh, AE_GFX_MDM_TRIANGLES);

	int particleCount = 1 + rand() % 3; // random between 1 and 4

	for (int i = 0; i < particleCount; i++)
	{
		float x = (rand() % screenWidth) - screenWidth * 0.5f; 
		float y = (rand() % screenHeight) - screenHeight * 0.5f;
		gParticles.spawnExplosion(x, y, 50);
	}

	// Click anywhere to return to main menu
	if (AEInputCheckTriggered(AEVK_LBUTTON) || AEInputCheckTriggered(AEVK_E)) {
		next = GS_MAIN_MENU;
		World::ResetAllGameState();
	}
}

void Win_Free() {

	AEGfxMeshFree(WinScreen::mesh);
}

void Win_Unload() {

	AEGfxTextureUnload(WinScreen::winScreen);
	WinScreen::winAudio.exit();
}
