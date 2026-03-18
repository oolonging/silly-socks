#include "../pch.h"
#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SettingsScreen.hpp"
#include "../UI_Elements/UI_Elements.hpp"
#include "../AudioManager.hpp"
#include "../../Managers/UIManager.hpp"
#include "../PauseMenu.hpp"
#include "../Settings.hpp"


static void navigateToPrev(void) { next = GS_BACK; }

namespace {
	struct SettingsState {
		UI_Elements::Slider* bgVolumeSlider;
		UI_Elements::Slider* sfxVolumeSlider;
		UI_Elements::Checkbox* bgmMuteCheckbox;
		UI_Elements::Checkbox* sfxMuteCheckbox;
		UI_Elements::Button* backButton;
	};

	// Unique ptr
	std::unique_ptr<SettingsState> state;
}

void Settings_Load() {
	state = std::make_unique<SettingsState>();
}

void Settings_Initialize() {

	// Initialize sliders
	state->bgVolumeSlider = UIManager::create<UI_Elements::Slider>("bgmVolumeSlider", -200.0f, 200.0f, 400.0f, 50.0f, &bgVolume, 0.0f, 255.0f);
	state->sfxVolumeSlider = UIManager::create<UI_Elements::Slider>("sfxVolumeSlider", -200.0f, 100.0f, 400.0f, 50.0f, &sfxVolume, 0.0f, 255.0f);

	// Initialize checkboxes
	state->bgmMuteCheckbox = UIManager::create<UI_Elements::Checkbox>("bgmMuteCheckbox", 250.0f, 200.0f, 50.0f, "MUTE", Settings::gMuteMusic);
	state->sfxMuteCheckbox = UIManager::create<UI_Elements::Checkbox>("sfxMuteCheckbox", 250.0f, 100.0f, 50.0f, "MUTE", Settings::gMuteSFX);


	//mute bgm when checkbox is toggled
	state->bgmMuteCheckbox->setOnChange([](bool isMuted) {
		Settings::gMuteMusic = isMuted;
		if (isMuted) {
			AudioManager::audio.setBGMVolume(0.0f);
		}
		else {
			AudioManager::audio.setBGMVolume(bgVolume / 100.0f);
		}
	});

	//mute sfx when checkbox is toggled
	state->sfxMuteCheckbox->setOnChange([](bool isMuted) {
		if (isMuted) AudioManager::audio.setSFXVolume(0.0f);
		else AudioManager::audio.setSFXVolume(sfxVolume / 100.0f);
		});

	//back button
	state->backButton = UIManager::create<UI_Elements::Button>(
		"backButton", -300.0f, 300.0f, 100.0f, 50.0f, "Back", Shapes::CENTER
	);

	//button function
	state->backButton->setOnClick(navigateToPrev);

}

void Settings_Update() {

	//bgm slider update
	float newbgVolume = bgVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	if (!state->bgmMuteCheckbox->getChecked()) {
		AudioManager::audio.setBGMVolume(bgVolume / 100.0f);
	}

	//sfx slider update
	float newsfxVolume = sfxVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	if (!state->sfxMuteCheckbox->getChecked()) {
		AudioManager::audio.setSFXVolume(sfxVolume / 100.0f);
	}


}

void Settings_Draw() {

	Text::textAlign(Text::CENTER_H, Text::CENTER_V);
	Text::setFont("default");

	Color::textFill(Color::Preset::Black);
	Color::background({ 162, 209, 213, 255 });

	Text::textSize(30.0f);
	Text::text("Settings", 0.0f, 350.0f);

	Text::textSize(20.0f);
	Text::text("BGM", -200.0f, 220.0f);
	Text::text("SFX", -200.0f, 120.0f);

	UIManager::drawAll();
}

void Settings_Free() {}

void Settings_Unload() {
	if (state) {

		// Nothing else to unload yet
		state.reset();
	}

	UIManager::clear();
}


