#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SettingsScreen.hpp"
#include "../UI_Elements.hpp"

//just testing audio for settings screen, should we make an audio manager file?
AEAudio bgMusic;
AEAudioGroup bgm;

float bgVolume = 50.0f;
UI_Elements::Slider bgVolumeSlider = UI_Elements::Slider(
	-300.0f, 200.0f,
	300.0f, 30.0f,
	bgVolume,
	0.0f, 100.0f,
	Shapes::CORNER
);

void Settings_Load() {

	//load bg music
	bgMusic = AEAudioLoadMusic("Assets/testaudio.mp3");

	// Creates an audio group named 'bgm'
	bgm = AEAudioCreateGroup();

}

void Settings_Initialize() {

	// Slider is already initialized at global scope with sliderValue reference
	// Just set custom style
	UI_Elements::ElementStyle sliderStyle;
	sliderStyle.primaryColor = Color::createColorRGB(150, 150, 150, 255);
	sliderStyle.secondaryColor = Color::createColorRGB(255, 150, 0, 255);
	sliderStyle.strokeColor = Color::createColorRGB(0, 0, 0, 255);
	sliderStyle.strokeWeight = 2.0f;
	bgVolumeSlider.setStyle(sliderStyle);

	// play bg music
	AEAudioPlay(bgMusic, bgm, 0.5f, 1.f, -1);

}

void Settings_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Settings screen", 0, 0);


	float newVolume = bgVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	AEAudioSetGroupVolume(bgm, newVolume);
}

void Settings_Draw() {

	bgVolumeSlider.draw();

}

void Settings_Free() {

}

void Settings_Unload() {

	AEAudioUnloadAudio(bgMusic);
	AEAudioUnloadAudioGroup(bgm);

}
