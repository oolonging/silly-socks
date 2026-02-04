#include "GameStateManager.hpp"
#include "AEEngine.h"

#include <iostream>
#include "Graphics.hpp"
#include "UI_Elements.hpp"
#include "Entity.hpp"
#include "World.hpp"
#include "Attack.h"

// -----------------------------------------------------------------------------
// Level-scope state (persists while Level1 is active)

bool drawGrid = false;

//UI_Elements::Button testButton;
//UI_Elements::Button testButton1;
//UI_Elements::Button thirdtest;

Entity::Player testPlayer;
Entity::Enemy  testEnemy;

UI_Elements::Slider testbar;
UI_Elements::Slider testbar1;

Weapon::Sword weapon;

// -----------------------------------------------------------------------------
// Load: file IO, textures, assets

void Level1_Load()
{
    return;
}

// Initialize some stuff
void Level1_Initialize()
{
    drawGrid = false;

    // Initialize player & enemy (NO redeclaration)
    testPlayer = Entity::Player(
        -400.0f, -400.0f,
        50.0f, 50.0f,
        100.0f, 5.0f, 0.0f
    );

    testEnemy = Entity::Enemy(
        000.0f, 0.0f,
        50.0f, 50.0f,
        100.0f, 2.0f, 0.0f
    );

    // Buttons
    /*testButton = UI_Elements::Button(
        Shapes::Quad{ {0.0f, 0.0f}, 200.0f, 100.0f },
        "corner tada",
        Shapes::CORNER
    );

    testButton1 = UI_Elements::Button(
        Shapes::Quad{ {-150.0f, 0.0f}, 200.0f, 100.0f },
        "center tada",
        Shapes::CENTER
    );

    thirdtest = UI_Elements::Button(
        Shapes::Quad{ {-300.0f, -200.0f}, 200.0f, 100.0f },
        "just in case tada",
        Shapes::CORNER
    );*/

    // Sliders
    testbar = UI_Elements::Slider(
        Shapes::Quad{ {-100.0f, 200.0f}, 300.0f, 50.0f },
        Shapes::CORNER
    );

    testbar1 = UI_Elements::Slider(
        Shapes::Quad{ {200.0f, 200.0f}, 300.0f, 50.0f },
        Shapes::CORNER
    );
    
    //test
    weapon = Weapon::Sword(
        testPlayer.getX() + 20, testPlayer.getY(),
        100.0f, 20.0f,
        25.0f, 0.0f, 45.0f,
        Shapes::CORNER
	);
}

// Initialize (update per frame?)
void Level1_Update()
{
    // Toggle grid
    /*if (AEInputCheckTriggered(AEVK_F3))
    {
        drawGrid = !drawGrid;
    }*/


	// Weapon update
    // theres a better way to do this but this is for visuals tomorrow lol
    if (AEInputCheckCurr(AEVK_A)) {
        weapon.setPosition(testPlayer.getX() - 130, testPlayer.getY());
    }
    if (AEInputCheckCurr(AEVK_D)) {
        weapon.setPosition(testPlayer.getX() + 30, testPlayer.getY());
	}
    if (AEInputCheckCurr(AEVK_W)) {
		weapon.setPosition(testPlayer.getX(), testPlayer.getY() + 50);
        }
	if (AEInputCheckCurr(AEVK_S)) {
        weapon.setPosition(testPlayer.getX(), testPlayer.getY() - 30);
    }

	testbar1.minValue = -1000;

	// Attack enemy
	if (AEInputCheckTriggered(AEVK_LBUTTON) && testEnemy.getHp() > 0) {

        if (weapon.attack(testEnemy)) {
            // Slider update
            testbar1.fgUpdate(
                4500.f,
                static_cast<float>(AEFrameRateControllerGetFrameTime())
            );
        }

		std::cout << "Enemy HP: " << testEnemy.getHp() << "\n";
        if (testEnemy.getHp() < 0) {
            testEnemy.setHp(0);
		}
        if (testEnemy.getHp() == 0) {
            std::cout << "Enemy defeated!\n";
		}
    }

    if (testPlayer.getX() > (AEGfxGetWindowWidth() / 2))
    {
        next += 1;
    }
	
}

// 
void Level1_Draw()
{

 /*   if (drawGrid)*/
        
    World::drawGrid();

    testPlayer.draw();

	//enemy disappears when dead
	bool enemyAlive = (testEnemy.getHp() > 0);
    if (enemyAlive) {
		testEnemy.draw(testPlayer);
    }

    Color::fill(0, 255, 0);
    testbar.bgDraw();
	Color::fill(255, 0, 0);
    testbar1.fgDraw();

    weapon.draw();
}


//
void Level1_Free()
{
    std::cout << "Level1:Free\n";
}

//
void Level1_Unload()
{
    return;
}
