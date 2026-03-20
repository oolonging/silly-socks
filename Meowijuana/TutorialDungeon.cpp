#include "pch.h"
#include "TutorialDungeon.hpp"

#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../DungeonManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../World.hpp"
#include "../../GameStateManager.hpp"
#include "../../PauseMenu.hpp"
#include "../../Settings.hpp"


World::worldGrid TDungeonGrid;
std::pair<int, int> tutActiveTile;
int tutorialRoomIndex = 0;


//  TODO : CHARACTER DEATH, MAKE ROOMS MORE INTERESTING, ENTITY COLLISION WITH WALL, PARTICLES IF POSSIBLE, KNOCKBACK???????
namespace Death {
    float opacity = 0.0f;
    bool dead = false;
    float fade;
}

// ------------- ROOM STATE (similar to the non-hard coded dungeonsS) ----------------------------'
struct TutorialRoomState {

    bool visited = false;
    bool cleared = false;

};

TutorialRoomState tutorialRooms[4];

// placeholder til teleporter comes in
void LoadRoom(int roomIndex) {
    if (roomIndex == 1)
        TDungeonGrid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_11.txt");
    else if (roomIndex == 2)
        TDungeonGrid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_21.txt");
    else if (roomIndex == 3)
        TDungeonGrid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_33.txt");
    else
        TDungeonGrid.fillGrid("Assets/LevelMaps/TutorialDungeon/mainRoomL1.txt");


    if (!tutorialRooms[roomIndex].visited) {
        tutorialRooms[roomIndex].visited = true;

        if (roomIndex != 0) {
            EntityManager::spawnEnemies(2, 800.0f, 600.0f);

            Inventory::Weapon* eWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::WOODEN_SWORD));
            if (eWeapon) {
                EntityManager::weaponEnemies(eWeapon);
                std::cout << "has weapon";
            }
        }
    }



}


void TutorialDungeon_Load() {
}


void TutorialDungeon_Initialize() {
    Settings::currentScreen = "TutorialDungeon.cpp";

    TDungeonGrid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
    TDungeonGrid.initMapTexture();
    TDungeonGrid.initTextureBox();

    EntityManager::init();
    auto* tutPlayer = EntityManager::getPlayer("player");

    Inventory::init();
    Inventory::Weapon* pWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD));
    if (pWeapon) {
        tutPlayer->setWeapon(pWeapon);
        tutPlayer->setAtkSpd(pWeapon->getAttackSpeed());
    }

    //Inventory::Weapon* eWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::WOODEN_SWORD));
    //if (eWeapon) {
    //    EntityManager::weaponEnemies(eWeapon);
    //    std::cout << "has weapon";
    //}

    LoadRoom(tutorialRoomIndex);
}



void TutorialDungeon_Update() {
    auto* tutPlayer = EntityManager::getPlayer("player");

    tutActiveTile = World::activeTile(tutPlayer->getX(), tutPlayer->getY(), TDungeonGrid);

    tutPlayer->update(TDungeonGrid);
    tutPlayer->tickAttackTimer();

    EntityManager::updateEnemies(*tutPlayer);


    if (AEInputCheckTriggered(AEVK_LBUTTON) && tutPlayer->canAttack()) {
        EntityManager::attackEnemies(*tutPlayer);
        tutPlayer->resetAttackTimer(); // reset once after hitting all enemies
    }


    if (!tutorialRooms[tutorialRoomIndex].cleared && EntityManager::allEnemiesDead()) {
        tutorialRooms[tutorialRoomIndex].cleared = true;
    }



    if (tutPlayer->getHp() <= 0) {
        tutPlayer->setHp(0);

        Death::dead = true;
        tutPlayer->isDead = true;

        if (Death::opacity < 255.0f) Death::opacity += 2.0f;;

        if (Death::opacity >= 255.0f) {
            Death::opacity = 255.0f;
            tutPlayer->isDead = false;
            next = GS_RESPAWN;
        }

    }



    float halfHeight = AEGfxGetWindowHeight() / 2.0f;
    float halfWidth = AEGfxGetWindowWidth() / 2.0f;
    float savedX = tutPlayer->getX();
    float savedY = tutPlayer->getY();

    if (tutorialRooms[tutorialRoomIndex].cleared && EntityManager::allEnemiesDead()) {

        if (tutPlayer->getY() >= halfHeight && tutorialRoomIndex < 3) {
            tutorialRoomIndex++;

            LoadRoom(tutorialRoomIndex);
            //auto* tutPlayer = EntityManager::getPlayer("player");
            tutPlayer->setPosition(savedX, -halfHeight + 10.0f);
        }

        else if (tutPlayer->getY() <= -halfHeight && tutorialRoomIndex > 0) {
            tutorialRoomIndex--;
            LoadRoom(tutorialRoomIndex);
            tutPlayer->setPosition(savedX, halfHeight - 10.0f);
        }

        if (tutPlayer->getX() >= halfWidth && tutorialRoomIndex < 3) {
            tutorialRoomIndex--;
            LoadRoom(tutorialRoomIndex);
            tutPlayer->setPosition(-halfWidth + 10.0f, savedY);
        }

        else if (tutPlayer->getX() <= -halfWidth && tutorialRoomIndex > 0) {
            tutorialRoomIndex++;
            LoadRoom(tutorialRoomIndex);
            tutPlayer->setPosition(halfWidth - 10.0f, savedY);
        }

        if (tutorialRooms[3].cleared) {
            World::standOnTile(next, *tutPlayer, TDungeonGrid, GS_FARM);
        }
    }
}


void TutorialDungeon_Draw() {
    auto* tutPlayer = EntityManager::getPlayer("player");
    TDungeonGrid.drawTexture(TDungeonGrid);
    World::drawTile(tutActiveTile, TDungeonGrid);

    // debug: show weapon attack range
    float attackRange = 170; // i put 170 since i put it as i believe 85 as range?

    if(Settings::gDebugMode)
    circle(tutPlayer->getX(), tutPlayer->getY(), attackRange, Shapes::CENTER);

    tutPlayer->draw();

    EntityManager::drawEnemies(*tutPlayer, TDungeonGrid, isPaused);



    if (Death::dead) {
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        Color::fill(255.0f, 255.0f, 255.0f, Death::opacity);
        Shapes::rect(0, 0, 1600, 900, Shapes::CENTER);

    }
}

void TutorialDungeon_Free() {
    TDungeonGrid.unloadMapTexture();
    World::freeGrid();
    EntityManager::clearEnemies();
    Death::dead = false;
    Death::opacity = 0.0f;
}

void TutorialDungeon_Unload() {
}

