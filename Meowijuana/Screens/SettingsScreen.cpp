#include "../pch.h"
#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SettingsScreen.hpp"
#include "../UI_Elements/UI_Elements.hpp"
#include "../AudioManager.hpp"
#include "../../Managers/UIManager.hpp"



//global object for audio management
AudioManager::Audio audio;

UI_Elements::Slider* bgVolumeSlider;
UI_Elements::Slider* sfxVolumeSlider;
UI_Elements::Checkbox* bgmMuteCheckbox;
UI_Elements::Checkbox* sfxMuteCheckbox;

float bgVolume = 50.0f;
float sfxVolume = 20.0f;

void Settings_Load() {
	audio.init();
	audio.loadBGM("Assets/testaudio.mp3");
	audio.loadSFX("Assets/testsfx.mp3");
}

void Settings_Initialize() {

	//create sliders
	bgVolumeSlider = UIManager::create<UI_Elements::Slider>("bgmVolumeSlider", -200.0f, 200.0f, 400.0f, 50.0f, &bgVolume, 0.0f, 255.0f);
	sfxVolumeSlider = UIManager::create<UI_Elements::Slider>("sfxVolumeSlider", -200.0f, 100.0f, 400.0f, 50.0f, &sfxVolume, 0.0f, 255.0f);

	//create checkboxes
	bgmMuteCheckbox = UIManager::create<UI_Elements::Checkbox>("bgmMuteCheckbox", 250.0f, 200.0f, 50.0f, "MUTE", false);
	sfxMuteCheckbox = UIManager::create<UI_Elements::Checkbox>("sfxMuteCheckbox", 250.0f, 100.0f, 50.0f, "MUTE", false);

	//set text style for checkbox for white text
	UI_Elements::TextStyle checkboxTextStyle = bgmMuteCheckbox->getTextStyle();
	checkboxTextStyle.primaryColor = Color::createColorRGB(255, 255, 255, 255);
	bgmMuteCheckbox->setTextStyle(checkboxTextStyle);
	sfxMuteCheckbox->setTextStyle(checkboxTextStyle);

	//mute bgm when checkbox is toggled
	bgmMuteCheckbox->setOnChange([](bool isMuted) {
		if (isMuted) audio.setBGMVolume(0.0f);
		else audio.setBGMVolume(bgVolume / 100.0f);
		});

	//mute sfx when checkbox is toggled
	sfxMuteCheckbox->setOnChange([](bool isMuted) {
		if (isMuted) audio.setSFXVolume(0.0f);
		else audio.setSFXVolume(sfxVolume / 100.0f);
		});

	//play bgm with initial volume
	audio.playBGM(bgVolume / 100.0f, true);

}

void Settings_Update() {
	// Nothing to init just a plain background and some text
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(255, 255, 255);
	Color::background({ 0, 0, 100, 255 });
	Text::text("BGM", -200.0f, 220.0f);
	Text::text("SFX", -200.0f, 120.0f);
	Text::text("Settings", 0.0f, 350.0f);

	//bgm slider update
	float newbgVolume = bgVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	if (!bgmMuteCheckbox->getChecked()) {
		audio.setBGMVolume(bgVolume / 100.0f);
	}

	//play sfx when left click
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		audio.playSFX(sfxVolume / 100.0f);
	}

	//sfx slider update
	float newsfxVolume = sfxVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	if (!sfxMuteCheckbox->getChecked()) {
		audio.setSFXVolume(sfxVolume / 100.0f);
	}


}

void Settings_Draw() {

	UIManager::drawAll();

}

void Settings_Free() {
}

void Settings_Unload() {
	audio.exit();
}


