#include "../GameStateManager.hpp"
#include "AEEngine.h"

#include <iostream>
#include "../Graphics.hpp"
#include "../UI_Elements/UI_Elements.hpp"
#include "../Entity.hpp"
#include "../World.hpp"
#include "Level1.hpp"

// -----------------------------------------------------------------------------
// Level-scope state (persists while Level1 is active)

extern bool drawGrid;

extern Entity::Player testPlayer;
Entity::Enemy  testEnemy2;
Entity::Enemy  testEnemy3;

// -----------------------------------------------------------------------------
// Load: file IO, textures, assets

void Level2_Load()
{
    std::cout << "Level 2 Loaded";
}

// Initialize some stuff
void Level2_Initialize()
{
    drawGrid = false;

    float oriX = testPlayer.getX() - 1600;
    float oriY = testPlayer.getY();

    testPlayer = Entity::Player(
        oriX, oriY,
        50.0f, 50.0f,
        100.0f, 5.0f, 0.0f
    );

    /* testEnemy2 = Entity::Enemy(
         000.0f, 0.0f,
         50.0f, 50.0f,
         100.0f, 2.0f, 0.0f
     );

     testEnemy3 = Entity::Enemy(
         000.0f, 0.0f,
         50.0f, 50.0f,
         100.0f, 2.0f, 0.0f
     );*/
}

// Initialize (update per frame?)
void Level2_Update()
{
    // Toggle grid
    /*if (AEInputCheckTriggered(AEVK_F3))
    {
        drawGrid = !drawGrid;
    }*/

    testPlayer.update();


    // Go to the farm outside instead
    if (testPlayer.getX() < -(AEGfxGetWindowWidth() / 2))
    {
        next -= 1;
    }

}

// 
void Level2_Draw()
{
    if (drawGrid)
        World::drawGrid();

    testPlayer.draw();

}


//
void Level2_Free()
{
    return;
}

//
void Level2_Unload()
{
    return;
}
