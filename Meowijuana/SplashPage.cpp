#pragma once

#include "SplashPage.hpp"
#include "AEEngine.h"
#include "UI_Elements.hpp"

extern int current, previous, next;

UI_Elements::Button testButton;
UI_Elements::Button testButton1;
UI_Elements::Button thirdtest;

AEGfxTexture* SplashScreen = nullptr;
AEGfxVertexList* splashMesh = nullptr;



void Splash_Load()
{
	if (!SplashScreen)
		SplashScreen = AEGfxTextureLoad("Assets/Cat.png");
}

void Splash_Initialize()
{
	testButton = UI_Elements::Button(Shapes::Quad{ {-300.0f, -200.0f}, 200.0f, 100.0f }, "Credits", Shapes::CENTER);
	testButton1 = UI_Elements::Button(Shapes::Quad{ {0.0f, -200.0f}, 200.0f, 100.0f }, "Play", Shapes::CENTER);
	thirdtest = UI_Elements::Button(Shapes::Quad{ {300.0f, -200.0f}, 200.0f, 100.0f }, "Settings", Shapes::CENTER);

	AEGfxMeshStart();

	// Unit quad centered at origin
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f
	);

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f
	);

	splashMesh = AEGfxMeshEnd();
}

void Go_play()
{
	next += 1;
}

void Splash_Update()
{
// If button 1 is pressed, change to level 1 instead
		testButton1.setOnClick(Go_play);

}
void Splash_Draw()
{

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(2.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	AEGfxTextureSet(SplashScreen, 0, 0);

	float winW = static_cast<float>(AEGfxGetWindowWidth());
	float winH = static_cast<float>(AEGfxGetWindowHeight());

	AEMtx33 scale, translate, transform;
	AEMtx33Scale(&scale, winW, winH);
	AEMtx33Trans(&translate, 0.0f, 0.0f);
	AEMtx33Concat(&transform, &scale, &translate);

	AEGfxSetTransform(transform.m);

	AEGfxMeshDraw(splashMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxTextureMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);
	testButton.draw();
	testButton1.draw();
	thirdtest.draw();
}

void Splash_Free()
{
	testButton = UI_Elements::Button{};
	testButton1 = UI_Elements::Button{};
	thirdtest = UI_Elements::Button{};
}

void Splash_Unload()
{
	AEGfxTextureUnload(SplashScreen);
	AEGfxMeshFree(splashMesh);
}
