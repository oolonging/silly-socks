
#include "AEEngine.h"
#include "PauseMenu.hpp"
#include "../../GameStateManager.hpp"
#include "../../Managers/UIManager.hpp"


bool isPaused = false;

//buttons
static UI_Elements::Button resumeButton;
static UI_Elements::Button settingsButton;
static UI_Elements::Button quitButton;

//button functions
static void navigateToPrev(void) { isPaused = false; }
static void navigateToSettings(void) { next = GS_SETTINGS; }
static void navigateToMainMenu(void) { next = GS_MAIN_MENU; }


void PauseMenu_Load() {

}

void PauseMenu_Initialize() {

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    //create buttons
    //testing for level 1, using create pointer will cause error, creating manually first
    resumeButton = UI_Elements::Button(
        0.0f, 130.0f, 200.0f, 100.0f, "Resume", Shapes::CENTER
    );
    settingsButton = UI_Elements::Button(
        0.0f, 0.0f, 200.0f, 100.0f, "Settings", Shapes::CENTER
    );
    quitButton = UI_Elements::Button(
        0.0f, -130.0f, 200.0f, 100.0f, "Quit", Shapes::CENTER
    );

    //text not showing
    UI_Elements::TextStyle buttonTextStyle = resumeButton.getTextStyle();
    buttonTextStyle.primaryColor = Color::createColorRGB(0, 0, 0, 255);
    resumeButton.setTextStyle(buttonTextStyle);

    //manual text first
    Color::textFill(0, 0, 0);
    Text::text("Resume", 0.0f, 130.0f);
    Text::text("Settings", 0.0f, 0.0f);
    Text::text("Quit", 0.0f, -130.0f);
    
    //button functions
    resumeButton.setOnClick(navigateToPrev);
    settingsButton.setOnClick(navigateToSettings);
    quitButton.setOnClick(navigateToMainMenu);

}

void PauseMenu_Update() {
	

}

void PauseMenu_Draw() {

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);

    //dim bg
    Color::fill(0.0f, 0.0f, 0.0f, 50.0f);
    Color::noStroke();
    Shapes::rect(0.0f, 0.0f, (float)AEGfxGetWindowWidth(), (float)AEGfxGetWindowHeight(), Shapes::CENTER);

    //panel
    Color::fill(255.0f, 197.0f, 121.0f, 255.0f);
    Color::stroke(255.0f, 255.0f, 255.0f, 255.0f);
    Shapes::rect(0.0f, 0.0f, 400.0f, 500.0f, Shapes::CENTER);

    //draw manually
    resumeButton.draw();
    settingsButton.draw();
    quitButton.draw();
}

void PauseMenu_Free() {

}

void PauseMenu_Unload() {

}