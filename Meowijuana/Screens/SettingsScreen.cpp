#include "../pch.h"
#include "AEEngine.h"
#include "../../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "SettingsScreen.hpp"
#include "../UI_Elements/UI_Elements.hpp"
#include "../AudioManager.hpp"
#include "../../Managers/UIManager.hpp"
#include "../PauseMenu.hpp"



static UI_Elements::Slider* bgVolumeSlider;
static UI_Elements::Slider* sfxVolumeSlider;
static UI_Elements::Checkbox* bgmMuteCheckbox;
static UI_Elements::Checkbox* sfxMuteCheckbox;
static UI_Elements::Button* backButton;

static void navigateToPrev(void) { next = GS_BACK; }

using namespace AudioManager;

void Settings_Load() {

}

void Settings_Initialize() {

	//create sliders
	bgVolumeSlider = UIManager::create<UI_Elements::Slider>("bgmVolumeSlider", -200.0f, 200.0f, 400.0f, 50.0f, &bgVolume, 0.0f, 255.0f);
	sfxVolumeSlider = UIManager::create<UI_Elements::Slider>("sfxVolumeSlider", -200.0f, 100.0f, 400.0f, 50.0f, &sfxVolume, 0.0f, 255.0f);

	//create checkboxes
	bgmMuteCheckbox = UIManager::create<UI_Elements::Checkbox>("bgmMuteCheckbox", 250.0f, 200.0f, 50.0f, "MUTE", false);
	sfxMuteCheckbox = UIManager::create<UI_Elements::Checkbox>("sfxMuteCheckbox", 250.0f, 100.0f, 50.0f, "MUTE", false);

	//set element style for checkbox
	UI_Elements::ElementStyle checkboxElementStyle = bgmMuteCheckbox->getStyle();
	checkboxElementStyle.primaryColor = Color::createColorRGB(100, 200, 100, 255);
	checkboxElementStyle.secondaryColor = Color::createColorRGB(200, 100, 50, 255);
	checkboxElementStyle.strokeWeight = 5.0f;
	bgmMuteCheckbox->setStyle(checkboxElementStyle);
	sfxMuteCheckbox->setStyle(checkboxElementStyle);

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

	//back button
	backButton = UIManager::create<UI_Elements::Button>(
		"backButton", -300.0f, 300.0f, 50.0f, 50.0f, " ", Shapes::CENTER
	);

	//button function
	backButton->setOnClick(navigateToPrev);

}

void Settings_Update() {

	//bgm slider update
	float newbgVolume = bgVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	if (!bgmMuteCheckbox->getChecked()) {
		audio.setBGMVolume(bgVolume / 100.0f);
	}

	//sfx slider update
	float newsfxVolume = sfxVolume / 100.0f; // Map slider value (0-100) to volume range (0.0-1.0)
	if (!sfxMuteCheckbox->getChecked()) {
		audio.setSFXVolume(sfxVolume / 100.0f);
	}


}

void Settings_Draw() {

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	Color::textFill(0, 0, 0);
	Color::background({ 162, 209, 213, 255 });

	Text::textSize(30.0f);
	Text::text("Settings", 0.0f, 350.0f);

	Text::textSize(20.0f);
	Text::text("BGM", -200.0f, 220.0f);
	Text::text("SFX", -200.0f, 120.0f);
	Text::text("Back", -380.0f, 300.0f);

	UIManager::drawAll();
}

void Settings_Free() {
}

void Settings_Unload() {

}


