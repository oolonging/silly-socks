#include "GameStateManager.hpp"
#include "AEEngine.h"

#include <iostream>
#include "Graphics.hpp"
#include "UI_Elements.hpp"
#include "Entity.hpp"
#include "World.hpp"

// -----------------------------------------------------------------------------
// Level-scope state (persists while Level1 is active)

bool drawGrid = false;

UI_Elements::Button testButton;
UI_Elements::Button testButton1;
UI_Elements::Button thirdtest;

Entity::Player testPlayer;
Entity::Enemy  testEnemy;

UI_Elements::Slider testbar;
UI_Elements::Slider testbar1;

// -----------------------------------------------------------------------------
// Load: file IO, textures, assets

void Level1_Load()
{
    std::cout << "Level1:Load\n";
}

// Initialize some stuff
void Level1_Initialize()
{
    drawGrid = false;

    // Initialize player & enemy (NO redeclaration)
    testPlayer = Entity::Player(
        0.0f, 0.0f,
        50.0f, 50.0f,
        100.0f, 5.0f, 0.0f
    );

    testEnemy = Entity::Enemy(
        200.0f, 200.0f,
        50.0f, 50.0f,
        100.0f, 5.0f, 0.0f
    );

    // Buttons
    testButton = UI_Elements::Button(
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
    );

    // Sliders
    testbar = UI_Elements::Slider(
        Shapes::Quad{ {200.0f, 200.0f}, 300.0f, 50.0f },
        Shapes::CORNER
    );

    testbar1 = UI_Elements::Slider(
        Shapes::Quad{ {200.0f, 200.0f}, 300.0f, 50.0f },
        Shapes::CORNER
    );
    
}

// Initialize (update per frame?)
void Level1_Update()
{
    // Toggle grid
    if (AEInputCheckTriggered(AEVK_F3))
    {
        drawGrid = !drawGrid;
    }

    // Slider update
    testbar1.fgUpdate(
        static_cast<float>(AEFrameRateControllerGetFrameTime())
    );
}

// 
void Level1_Draw()
{
    if (drawGrid)
        World::drawGrid();

    testPlayer.draw();
    testEnemy.draw(testPlayer);

    testbar.bgDraw();
    testbar1.fgDraw();
}


//
void Level1_Free()
{
    std::cout << "Level1:Free\n";
}

//
void Level1_Unload()
{
    std::cout << "Level1:Unload\n";
}
