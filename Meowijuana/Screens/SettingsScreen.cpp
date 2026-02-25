#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SettingsScreen.hpp"
#include "../UI_Elements/UI_Elements.hpp"
#include "../AudioManager.hpp"

//testing audio manager
//global object for audio management
AudioManager::Audio audio;


float bgVolume = 50.0f;
UI_Elements::Slider bgVolumeSlider = UI_Elements::Slider(
	-300.0f, 200.0f,
	300.0f, 30.0f,
	bgVolume,
	0.0f, 100.0f,
	Shapes::CORNER
);

void Settings_Load() {

	audio.init();
	audio.loadBGM("Assets/testaudio.mp3");
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

	audio.playBGM(bgVolume / 100.0f, true);

}

void Settings_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 0, 255 });
	Text::text("Settings screen", 0, 0);


	float newVolume = bgVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	audio.setBGMVolume(newVolume);
}

void Settings_Draw() {

	bgVolumeSlider.draw();

}

void Settings_Free() {

}

void Settings_Unload() {

	audio.exit();
}
