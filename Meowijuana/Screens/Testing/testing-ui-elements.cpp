//#include "testing-ui-elements.hpp"
//#include "AEEngine.h"
//#include "../../UI_Elements/UI_Elements.hpp"
//#include "../../GameStateManager.hpp"
//#include "../../Managers/UIManager.hpp"
//
//// Global
//bool renderGraphicalUI = false;
//
//void switchRenderMode(void) {
//	// When this function is called, it will toggle whether the page renders the UI elements with Graphics or not
//	// Currently bound to the G key
//	if (AEInputCheckTriggered(AEVK_G)) {
//		renderGraphicalUI = !renderGraphicalUI;
//	}
//}
//
//////////////////////////////////////
//// Declare all the UI elements here
//////////////////////////////////////
//
//// Button
//UI_Elements::Button myButton;
//
//// ProgressBar
//UI_Elements::ProgressBar myProgressBar;
//
//// Slider
//float cashewSliderValue = 0.5f; // Value for slider to control (red component)
//UI_Elements::Slider mySlider = UI_Elements::Slider(-AEGfxGetWindowWidth() * 0.35f, AEGfxGetWindowHeight() * 0.1f, 300.0f, 40.0f, cashewSliderValue, 0.0f, 255.0f, Shapes::CORNER);
//
//// TextBox
//UI_Elements::TextBox myTextBox;
//
//// Checkbox
//UI_Elements::Checkbox myCheckbox;
//
//// RadioButtons
//UI_Elements::RadioButton radioButton1;
//UI_Elements::RadioButton radioButton2;
//UI_Elements::RadioButton radioButton3;
//
//// DialogueBox
//UI_Elements::DialogueBox myDialogue;
//
//// Additional variables for testing
//float mouseProgressValue = 0.0f;
//float backgroundRed = 255.0f;
//bool showDialogue = false;
//
//
//
//
//// Helper function for button
//void meow(void) {
//	printf("Meow\n");
//}
//
//// Helper function for checkbox
//void onCheckboxChange(bool isChecked) {
//	if (isChecked) {
//		printf("Checkbox is now CHECKED\n");
//	}
//	else {
//		printf("Checkbox is now UNCHECKED\n");
//	}
//}
//
//// Helper functions for radio buttons
//void onRadioOption1(void) {
//	printf("Radio Option 1 selected\n");
//	backgroundRed = 100.0f;
//}
//
//void onRadioOption2(void) {
//	printf("Radio Option 2 selected\n");
//	backgroundRed = 180.0f;
//}
//
//void onRadioOption3(void) {
//	printf("Radio Option 3 selected\n");
//	backgroundRed = 255.0f;
//}
//
//// Helper function for dialogue
//void onDialogueDismiss(void) {
//	printf("Dialogue dismissed\n");
//	showDialogue = false;
//}
//
//// Helper function to show dialogue
//void showDialogueBox(void) {
//	printf("Showing dialogue box\n");
//	showDialogue = true;
//	myDialogue.activate();
//}
//
//void testingUI_Load() {
//
//}
//
//void testingUI_Initialize() {
//	// Get screen dimensions
//	float screenWidth = static_cast<float>(AEGfxGetWindowWidth());
//	float screenHeight = static_cast<float>(AEGfxGetWindowHeight());
//
//	// Initialize Button (top-left area)
//	myButton = UI_Elements::Button(-screenWidth * 0.35f, screenHeight * 0.35f, 200.0f, 60.0f, "MEOW!", Shapes::CORNER);
//	myButton.setOnClick(meow);
//
//	// Initialize ProgressBar (full width at top, tracks mouse X position)
//	mouseProgressValue = screenWidth * 0.5f; // Start at middle
//	myProgressBar = UI_Elements::ProgressBar(-screenWidth * 0.5f, screenHeight * 0.45f, screenWidth, 40.0f, mouseProgressValue, 0.0f, screenWidth, Shapes::CORNER);
//
//	// Initialize Slider (middle-left, controls red component of background)
//	cashewSliderValue = 128.0f; // Start at middle
//
//	// Initialize TextBox (center area)
//	myTextBox = UI_Elements::TextBox(-100.0f, -screenHeight * 0.1f, 300.0f, 50.0f, "Type something...", 100, Shapes::CORNER);
//
//	// Initialize Checkbox (bottom-left)
//	myCheckbox = UI_Elements::Checkbox(-screenWidth * 0.35f, -screenHeight * 0.25f, 30.0f, "Enable Feature", false, Shapes::CORNER);
//	myCheckbox.setOnChange(onCheckboxChange);
//
//	// Initialize RadioButtons (bottom-center area, same group)
//	radioButton1 = UI_Elements::RadioButton(-screenWidth * 0.15f, -screenHeight * 0.35f, 25.0f, "Low Red", 0, false, Shapes::CORNER);
//	radioButton1.setOnSelect(onRadioOption1);
//
//	radioButton2 = UI_Elements::RadioButton(0.0f, -screenHeight * 0.35f, 25.0f, "Mid Red", 0, true, Shapes::CORNER);
//	radioButton2.setOnSelect(onRadioOption2);
//
//	radioButton3 = UI_Elements::RadioButton(screenWidth * 0.15f, -screenHeight * 0.35f, 25.0f, "High Red", 0, false, Shapes::CORNER);
//	radioButton3.setOnSelect(onRadioOption3);
//
//	// Initialize DialogueBox (bottom of screen)
//	myDialogue = UI_Elements::DialogueBox(-screenWidth * 0.4f, -screenHeight * 0.45f, screenWidth * 0.8f, 150.0f);
//	myDialogue.setText("Hello! This is a test dialogue box. Click anywhere to dismiss.");
//	myDialogue.setSpeaker("Test Character");
//	myDialogue.setOnDismiss(onDialogueDismiss);
//	myDialogue.setShowSprite(false); // No sprite for testing
//
//	// Initialize Button to show dialogue (top-right)
//	UI_Elements::Button dialogueButton = UI_Elements::Button(screenWidth * 0.2f, screenHeight * 0.35f, 220.0f, 60.0f, "Show Dialogue", Shapes::CORNER);
//	dialogueButton.setOnClick(showDialogueBox);
//
//	printf("Testing UI initialized with all elements\n");
//}
//
//void testingUI_Update() {
//	switchRenderMode();
//
//	// Update mouse progress bar - track mouse X position
//	s32 mouseX, mouseY;
//	AEInputGetCursorPosition(&mouseX, &mouseY);
//
//	float screenWidth = static_cast<float>(AEGfxGetWindowWidth());
//	mouseProgressValue = static_cast<float>(mouseX);
//	mouseProgressValue = (mouseProgressValue < 0.0f) ? 0.0f : (mouseProgressValue > screenWidth) ? screenWidth : mouseProgressValue;
//	myProgressBar.setValue(mouseProgressValue);
//
//	// Update background red component from slider
//	backgroundRed = cashewSliderValue;
//}
//
//void testingUI_Draw() {
//	// Set background using slider-controlled red component
//	Color::background({ backgroundRed, 255.0f, 255.0f, 255.0f });
//
//	// Draw title text
//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
//	Color::fill(0.0f, 0.0f, 0.0f, 255.0f);
//
//	float screenWidth = static_cast<float>(AEGfxGetWindowWidth());
//	float screenHeight = static_cast<float>(AEGfxGetWindowHeight());
//
//	// Instructions
//	//Graphics::textCentered(0.0f, screenHeight * 0.48f, 16, "Press G to toggle graphical UI mode | All UI Elements Test Page");
//	//Graphics::textCentered(-screenWidth * 0.35f + 150.0f, screenHeight * 0.28f, 14, "Button Test:");
//	//Graphics::textCentered(0.0f, screenHeight * 0.48f - 60.0f, 14, "Progress Bar (tracks mouse X):");
//	//Graphics::textCentered(-screenWidth * 0.35f + 150.0f, screenHeight * 0.15f, 14, "Slider (controls background red):");
//	//Graphics::textCentered(50.0f, -screenHeight * 0.05f, 14, "TextBox:");
//	//Graphics::textCentered(-screenWidth * 0.35f + 100.0f, -screenHeight * 0.2f, 14, "Checkbox:");
//	//Graphics::textCentered(0.0f, -screenHeight * 0.3f, 14, "Radio Buttons (change background):");
//
//
//	// Draw all UI elements
//	myButton.draw();
//	myProgressBar.draw();
//	mySlider.draw();
//	myTextBox.draw();
//	myCheckbox.draw();
//	radioButton1.draw();
//	radioButton2.draw();
//	radioButton3.draw();
//
//	// Draw dialogue button
//	UI_Elements::Button dialogueButton = UI_Elements::Button(screenWidth * 0.2f, screenHeight * 0.35f, 220.0f, 60.0f, "Show Dialogue", Shapes::CORNER);
//	dialogueButton.setOnClick(showDialogueBox);
//	dialogueButton.draw();
//
//	// Draw dialogue box if active
//	if (showDialogue) {
//		myDialogue.draw();
//	}
//
//	// Display current values
//	char infoText[256];
//	sprintf_s(infoText, "Slider Value (Red): %.0f | Mouse X: %.0f | Checkbox: %s",
//		cashewSliderValue, mouseProgressValue, myCheckbox.getChecked() ? "ON" : "OFF");
//	Text::text(infoText, 0.0f, -screenHeight * 0.48f);
//}
//
//void testingUI_Free() {
//
//}
//
//void testingUI_Unload() {
//
//}