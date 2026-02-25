#include <iostream>

#include "AEEngine.h"
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "../UI_Elements/UI_Elements.hpp"
#include "TestingScreen.hpp"

// stuff i need
#include "../Entity.hpp"

// import other testing pages here
#include "Testing/testing-andrea.hpp"
#include "Testing/testing-jun.hpp"
#include "Testing/testing-saahil.hpp"
#include "Testing/testing-xuan.hpp"

// temporary routing to individual testing pages
void rerouteTesting(void) {
	
	// Routes to saahils testing page
	if (AEInputCheckTriggered(AEVK_NUMPAD1)) {
		fpLoad = Saahil_Load;
		fpInitialize = Saahil_Initialize;
		fpUpdate = Saahil_Update;
		fpDraw = Saahil_Draw;
		fpFree = Saahil_Free;
		fpUnload = Saahil_Unload;
	}

	// Routes to xuans testing page
	if(AEInputCheckTriggered(AEVK_NUMPAD2)) {
		fpLoad = Xuan_Load;
		fpInitialize = Xuan_Initialize;
		fpUpdate = Xuan_Update;
		fpDraw = Xuan_Draw;
		fpFree = Xuan_Free;
		fpUnload = Xuan_Unload;
	}

	// Routes to andreas testing page
	if(AEInputCheckTriggered(AEVK_NUMPAD3)) {
		fpLoad = Andrea_Load;
		fpInitialize = Andrea_Initialize;
		fpUpdate = Andrea_Update;
		fpDraw = Andrea_Draw;
		fpFree = Andrea_Free;
		fpUnload = Andrea_Unload;
	}

	// Routes to juns testing page
	if(AEInputCheckTriggered(AEVK_NUMPAD4)) {
		fpLoad = Jun_Load;
		fpInitialize = Jun_Initialize;
		fpUpdate = Jun_Update;
		fpDraw = Jun_Draw;
		fpFree = Jun_Free;
		fpUnload = Jun_Unload;
	}
}

namespace Cashew {
	// Player object
	Entity::Player player;

	// heal and hurt functions
	void heal() {
		player.setHp(player.getHp() + 5.0f);
	}

	void hurt() {
		player.setHp(player.getHp() - 5.0f);
	}

	void toggleGrid(bool) {
		std::cout << "Grid draw toggled" << std::endl;
		//return !drawGrid.getChecked();
	}

	// UI Elements

	// Button
	UI_Elements::Button healButton = UI_Elements::Button(300.0f, 300.0f, 100.0f, 50.0f, "Heal", Shapes::CORNER);
	UI_Elements::Button hurtButton = UI_Elements::Button(450.0f, 300.0f, 100.0f, 50.0f, "Hurt", Shapes::CORNER);

	// Slider
	float sliderValue = 5.0f;
	UI_Elements::Slider speedSlider = UI_Elements::Slider(300.0f, 200.0f, 250.0f, 50.0f, sliderValue, 0.0f, 10.0f, Shapes::CORNER);

	// Checkbox
	UI_Elements::Checkbox drawGrid{};

	// Text box
	std::string myText{};
	UI_Elements::TextBox chatBox;


}

void Testing_Load() {

}

void Testing_Initialize() {
	// Init player
	Cashew::player.setPosition(0.0f, 0.0f);
	Cashew::player.setSpeed(5.0f);

	// Init buttons
	Cashew::healButton.setOnClick(Cashew::heal);
	Cashew::hurtButton.setOnClick(Cashew::hurt);

	UI_Elements::ElementTexture buttonTexture = {
		AEGfxTextureLoad("Assets/Images/UI_Elements/Button/button_normal.png"),
		AEGfxTextureLoad("Assets/Images/UI_Elements/Button/button_pressed.png")
	};
	Cashew::healButton.setTexture(buttonTexture);

	// Init checkbox
	Cashew::drawGrid = UI_Elements::Checkbox(600.0f, -400.0f, 15.0f, "Draw Grid", false, Shapes::CENTER); // TODO: test the other draw mode
	// TODO: The default draw Mode doesnt work
	// 

	Cashew::drawGrid.setOnChange(Cashew::toggleGrid);

	// Init text box
	Cashew::chatBox = UI_Elements::TextBox(-750.0f, -350.0f, 200.0f, 50.0f, Cashew::myText); // TODO: change maxlen and also test the other draw mode
	//TODO: change parameter name placeholder so its less ambiguous
}

void Testing_Update() {
	// reroute testing
	rerouteTesting();

	// Update player position
	Cashew::player.update();

	// Update player speed;
	Cashew::player.setSpeed(Cashew::sliderValue);
}

void Testing_Draw() {
	// reset background
	Color::background(Color::Preset::Orange);

	// Draw player
	Cashew::player.draw();

	// Draw text
	Color::textFill(Color::Preset::Blue);
	Text::text("UI Elements Playground", 0.0f, 400.0f, Text::CENTER_H, Text::CENTER_V);

	// UI Elements

	// Draw buttons
	Cashew::hurtButton.draw();
	Cashew::healButton.draw();

	// Draw slider
	Cashew::speedSlider.draw();

	// Draw checkbox
	Cashew::drawGrid.draw();

	// Draw text box
	Cashew::chatBox.draw();
}

void Testing_Free() {
}

void Testing_Unload() {
}
