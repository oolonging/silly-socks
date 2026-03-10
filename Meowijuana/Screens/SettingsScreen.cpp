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
float sfxVolume = 20.0f;
UI_Elements::Slider bgVolumeSlider = UI_Elements::Slider(
	-300.0f, 200.0f,
	300.0f, 30.0f,
	&bgVolume,
	0.0f, 100.0f,
	Shapes::CORNER
);
UI_Elements::Slider sfxVolumeSlider = UI_Elements::Slider(
	-300.0f, 100.0f,
	300.0f, 30.0f,
	&sfxVolume,
	0.0f, 100.0f,
	Shapes::CORNER
);
static UI_Elements::Button bgmButton;
static UI_Elements::Button sfxButton;


void Settings_Load() {
	audio.init();
	audio.loadBGM("Assets/testaudio.mp3");
	audio.loadSFX("Assets/testsfx.mp3");
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
	sfxVolumeSlider.setStyle(sliderStyle);

	// manually set texture, ill need some help with the default texture since it saves memory but i dont know why its not workign yet
	bgVolumeSlider.setTexture({
		AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/slider_handle.png"),
		AEGfxTextureLoad("Assets/Images/UI_Elements/Slider/slider_bar.png")
	});

	bgmButton = UI_Elements::Button(Shapes::Quad{ {200.0f, 200.0f}, 70.0f, 70.0f }, "BGM", Shapes::CENTER);
	sfxButton = UI_Elements::Button(Shapes::Quad{ {200.0f, 100.0f}, 70.0f, 70.0f }, "SFX", Shapes::CENTER);

	//play bgm with initial volume
	audio.playBGM(bgVolume / 100.0f, true);

}

void Settings_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 100, 255 });
	Text::text("BGM", -300.0f, 220.0f);
	Text::text("SFX", -300.0f, 120.0f);

	//bgm slider update
	float newbgVolume = bgVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	audio.setBGMVolume(newbgVolume);

	//play sfx to test volume
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		audio.playSFX(sfxVolume / 100.0f);
	}

	//sfx slider update
	float newsfxVolume = sfxVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	audio.setSFXVolume(newsfxVolume);

	//button updates
	bgmButton.setOnClick([]() {
		audio.toggleBGMMuted();
	});
	sfxButton.setOnClick([]() {
		audio.toggleSFXMuted();
	});
}

void Settings_Draw() {

	bgVolumeSlider.draw();
	sfxVolumeSlider.draw();
	bgmButton.draw();
	sfxButton.draw();

}

void Settings_Free() {
}

void Settings_Unload() {
	audio.exit();
}
