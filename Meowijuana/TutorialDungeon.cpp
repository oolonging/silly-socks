#include "pch.h"
#include "AEEngine.h"
#include "TutorialDungeon.hpp"
#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../DungeonManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../World.hpp"

World::worldGrid TDungeonGrid;
std::pair<int, int> tutActiveTile;
int tutorialRoomIndex = 0;

Entity::Player tutPlayer;
Entity::Enemy tutEnemy1, tutEnemy2, tutEnemy3;


// placeholder til teleporter comes in
void LoadRoom(int roomIndex) {
    if (roomIndex == 1)
        TDungeonGrid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_11.txt");
    else if (roomIndex == 2)
        TDungeonGrid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_12.txt");
    else
        TDungeonGrid.fillGrid("Assets/LevelMaps/TutorialDungeon/mainRoomL1.txt");
}


void TutorialDungeon_Load() {
}


void TutorialDungeon_Initialize() {
    TDungeonGrid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
    TDungeonGrid.initMapTexture();
    TDungeonGrid.initTextureBox();
    LoadRoom(tutorialRoomIndex);

    tutPlayer = Entity::Player(-600.0f, 50.0f, 50.0f, 50.0f, 100.0f, 5.0f, 0.0f);
    tutEnemy1 = Entity::Enemy(-100.0f, 50.0f, 50.0f, 50.0f, 100.0f, 2.0f, 0.0f);
    tutEnemy2 = Entity::Enemy(100.0f, 50.0f, 50.0f, 50.0f, 100.0f, 2.0f, 0.0f);
    tutEnemy3 = Entity::Enemy(200.0f, 50.0f, 50.0f, 50.0f, 100.0f, 2.0f, 0.0f);


    Inventory::init();
    Inventory::Weapon* pWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD));
    if (pWeapon) {
        tutPlayer.setWeapon(pWeapon);
        tutPlayer.setAtkSpd(pWeapon->getAttackSpeed());
    }

    Inventory::Weapon* eWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::WOODEN_SWORD));
    if (eWeapon) {
        tutEnemy1.setWeapon(eWeapon);
        tutEnemy2.setWeapon(eWeapon);
        tutEnemy3.setWeapon(eWeapon);
    }
}



void TutorialDungeon_Update() {
    tutActiveTile = World::activeTile(tutPlayer.getX(), tutPlayer.getY(), TDungeonGrid);
    if (AEInputCheckTriggered(AEVK_E))
        World::interactTile(tutActiveTile, TDungeonGrid);

    tutPlayer.update();
    tutPlayer.tickAttackTimer();

    Entity::Enemy* enemies[] = { &tutEnemy1, &tutEnemy2, &tutEnemy3 };

    for (auto* enemy : enemies) {
        if (!enemy->isAlive()) continue;
        enemy->tickAttackTimer();
        if (enemy->canAttack()) {
            enemy->attack(tutPlayer);
            enemy->resetAttackTimer();
        }
    }


    if (AEInputCheckTriggered(AEVK_LBUTTON) && tutPlayer.canAttack()) {
        for (auto* enemy : enemies) {
            if (enemy->isAlive()) {
                tutPlayer.attack(*enemy);
                if (enemy->getHp() <= 0) enemy->setHp(0);
            }
        }
        tutPlayer.resetAttackTimer(); // reset once after hitting all enemies
    }



    if (tutPlayer.getHp() <= 0) tutPlayer.setHp(0);

    float halfHeight = AEGfxGetWindowHeight() / 2.0f;
    float savedX = tutPlayer.getX();

    if (tutPlayer.getY() >= halfHeight && tutorialRoomIndex < 2) {
        tutorialRoomIndex++;
        TutorialDungeon_Initialize();
        tutPlayer.setPosition(savedX, -halfHeight + 10.0f);
    }

    else if (tutPlayer.getY() <= -halfHeight && tutorialRoomIndex > 0) {
        tutorialRoomIndex--;
        TutorialDungeon_Initialize();
        tutPlayer.setPosition(savedX, halfHeight - 10.0f);
    }
}


void TutorialDungeon_Draw() {
    TDungeonGrid.drawTexture(TDungeonGrid);
    World::drawTile(tutActiveTile, TDungeonGrid);

    // debug: show weapon attack range
    float attackRange = 170; // i put 170 since i put it as i believe 85 as range?

    circle(tutPlayer.getX(), tutPlayer.getY(), attackRange, Shapes::CENTER);

    tutPlayer.draw();

    Entity::Enemy* enemies[] = { &tutEnemy1, &tutEnemy2, &tutEnemy3 };
    for (auto* enemy : enemies) {
        if (enemy->isAlive())
            enemy->draw(tutPlayer);
    }
}

void TutorialDungeon_Free() {
    TDungeonGrid.unloadMapTexture();
    World::freeGrid();
}

void TutorialDungeon_Unload() {
}

