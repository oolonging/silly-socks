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

Entity::Player testPlayer;
Entity::Enemy testEnemy;

Weapon::Sword weapon;

// UI Element Tests
float sliderValue = 50.0f;
float progressValue = 75.0f;

UI_Elements::Button testButton;
UI_Elements::Slider testSlider = UI_Elements::Slider(
    -300.0f, 200.0f,
    300.0f, 30.0f,
    sliderValue,
    0.0f, 100.0f,
    Shapes::CORNER
);
UI_Elements::ProgressBar testProgressBar;

bool firstTime = true;

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
    if (firstTime) {
        testPlayer = Entity::Player(
            -400.0f, -400.0f,
            50.0f, 50.0f,
            100.0f, 5.0f, 0.0f
        );

        firstTime = false;
    }
    else {
        testPlayer.setPosition(
            testPlayer.getX() + 1600, testPlayer.getY()
        ); 
    }

    testEnemy = Entity::Enemy(
        0.0f, 0.0f,
        50.0f, 50.0f,
        100.0f, 2.0f, 0.0f
    );

    // Initialize weapon
    weapon = Weapon::Sword(
        testPlayer.getX() + 20, testPlayer.getY(),
        100.0f, 20.0f,
        25.0f, 0.0f, 45.0f,
        Shapes::CORNER
    );

    // Initialize UI Elements for testing
    
    // Button with callback
    testButton = UI_Elements::Button(
        -300.0f, 300.0f,
        200.0f, 60.0f,
        "Click Me!",
        Shapes::CORNER
    );
    testButton.setOnClick([]() {
        std::cout << "Button clicked! Progress increased.\n";
        progressValue += 10.0f;
        if (progressValue > 100.0f) progressValue = 100.0f;
    });

    // Custom style for button
    UI_Elements::ElementStyle buttonStyle;
    buttonStyle.primaryColor = Color::CL_Color_Create(50, 150, 200, 255);
    buttonStyle.secondaryColor = Color::CL_Color_Create(100, 200, 255, 255);
    buttonStyle.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
    buttonStyle.strokeWeight = 3.0f;
    testButton.setStyle(buttonStyle);

    // Slider is already initialized at global scope with sliderValue reference
    // Just set custom style
    UI_Elements::ElementStyle sliderStyle;
    sliderStyle.primaryColor = Color::CL_Color_Create(150, 150, 150, 255);
    sliderStyle.secondaryColor = Color::CL_Color_Create(255, 150, 0, 255);
    sliderStyle.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
    sliderStyle.strokeWeight = 2.0f;
    testSlider.setStyle(sliderStyle);

    // Progress Bar
    testProgressBar = UI_Elements::ProgressBar(
        -300.0f, 100.0f,
        300.0f, 40.0f,
        progressValue,
        0.0f, 100.0f,
        Shapes::CORNER
    );

    // Custom style for progress bar
    UI_Elements::ElementStyle progressStyle;
    progressStyle.primaryColor = Color::CL_Color_Create(80, 80, 80, 255);
    progressStyle.secondaryColor = Color::CL_Color_Create(0, 255, 100, 255);
    progressStyle.strokeColor = Color::CL_Color_Create(0, 0, 0, 255);
    progressStyle.strokeWeight = 2.0f;
    testProgressBar.setStyle(progressStyle);
}

// Update per frame
void Level1_Update()
{
    // Weapon position update based on player movement direction
    if (AEInputCheckCurr(AEVK_A)) {
        weapon.setPosition(testPlayer.getX() - 80, testPlayer.getY());
    }
    else if (AEInputCheckCurr(AEVK_D)) {
        weapon.setPosition(testPlayer.getX() + 30, testPlayer.getY());
    }
    else if (AEInputCheckCurr(AEVK_W)) {
        weapon.setPosition(testPlayer.getX(), testPlayer.getY() + 50);
    }
    else if (AEInputCheckCurr(AEVK_S)) {
        weapon.setPosition(testPlayer.getX(), testPlayer.getY() - 30);
    }
    else {
        // Default position (right side)
        weapon.setPosition(testPlayer.getX() + 30, testPlayer.getY());
    }

    // Attack enemy
    if (AEInputCheckTriggered(AEVK_LBUTTON) && testEnemy.getHp() > 0) {
        if (weapon.attack(testEnemy)) {
            std::cout << "Hit! Enemy HP: " << testEnemy.getHp() << "\n";
            
            if (testEnemy.getHp() <= 0) {
                testEnemy.setHp(0);
                std::cout << "Enemy defeated!\n";
            }
        }
        else {
            std::cout << "Missed!\n";
        }
    }

    // Update player speed based on slider value (map 0-100 to 1-10)
    float newSpeed = 1.0f + (sliderValue / 100.0f) * 9.0f;
    testPlayer.setSpeed(newSpeed);

    // Update progress bar value
    testProgressBar.setValue(progressValue);

    // Auto-decrease progress over time (for demonstration)
    progressValue -= 5.0f * static_cast<float>(AEFrameRateControllerGetFrameTime());
    if (progressValue < 0.0f) progressValue = 0.0f;

    // Level transition check
    if (testPlayer.getX() > (AEGfxGetWindowWidth() / 2)) {
        next += 1;
    }
}

// Draw
void Level1_Draw()
{
    World::drawGrid();

    // Draw UI Elements first (behind game objects)
    testButton.draw();
    testSlider.draw();
    testProgressBar.draw();

    // Draw game objects
    testPlayer.draw();

    // Enemy disappears when dead
    if (testEnemy.getHp() > 0) {
        testEnemy.draw(testPlayer);
    }

    weapon.draw();

    // Draw UI labels using text (if font is set)
    Color::textFill(255, 255, 255, 255);
    // Note: Uncomment these when font is loaded
    // Text::text("Speed Control:", -300, 240, Text::ALIGN_LEFT);
    // Text::text("Progress:", -300, 140, Text::ALIGN_LEFT);
}

// Free
void Level1_Free()
{
    std::cout << "Level1:Free\n";
}

// Unload
void Level1_Unload()
{
    return;
}
