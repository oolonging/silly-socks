#include "pch.h"
#include "TutorialDungeon.hpp"

#include "../../Graphics.hpp"
#include "../../Tiles.hpp"
#include "../../Entity.hpp"
#include "../../DungeonManager.hpp"
#include "../../Managers/EntityManager.hpp"
#include "../../Managers/UIManager.hpp"
#include "../../World.hpp"
#include "../../GameStateManager.hpp"
#include "../../PauseMenu.hpp"
#include "../../Settings.hpp"

// Externs for Inventory
extern UI_Elements::PlayerInventory inv;
extern bool showInventory;
extern GameData gameData;

static Animations::Indicator point;

extern World::worldGrid grid;
std::pair<int, int> tutActiveTile;
int tutorialRoomIndex = 0;

static UI_Elements::PopupBox* tutPopup;

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
        grid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_11.txt");
    else if (roomIndex == 2)
        grid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_21.txt");
    else if (roomIndex == 3)
        grid.fillGrid("Assets/LevelMaps/TutorialDungeon/sideRoomL1_33.txt");
    else
        grid.fillGrid("Assets/LevelMaps/TutorialDungeon/mainRoomL1.txt");


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

    /*grid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
    grid.initMapTexture();
    grid.initTextureBox();*/

    EntityManager::init();
    auto* tutPlayer = EntityManager::getPlayer("player");

    inv.setPlayer(EntityManager::getPlayer("player"));
    inv.loadInventory(tutPlayer, gameData);

    Inventory::Weapon* pWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD));
    if (pWeapon) {
        tutPlayer->setWeapon(pWeapon);
        tutPlayer->setAtkSpd(pWeapon->getAttackSpeed());
    }

    tutPopup = UIManager::create<UI_Elements::PopupBox>("tutPopup", 120.0f, -80.0f, 500.0f, 250.0f, "You have cleared the tutorial!", "You will be on your own now", "Please explore the dungeons and save the world!");
    tutPopup->setOnDismiss([]() {
        tutPopup->hide();
        });

    grid.toggleWall();

    LoadRoom(tutorialRoomIndex);
}



void TutorialDungeon_Update() {
    auto* tutPlayer = EntityManager::getPlayer("player");

    tutActiveTile = World::activeTile(tutPlayer->getX(), tutPlayer->getY(), grid);

    tutPlayer->update(grid);
    tutPlayer->tickAttackTimer();

    inv.update(tutPlayer);

    EntityManager::updateEnemies(*tutPlayer);


    if (AEInputCheckTriggered(AEVK_LBUTTON) && tutPlayer->canAttack()) {
        EntityManager::attackEnemies(*tutPlayer);
        World::checkCarrotSwordConsume(inv, *tutPlayer);
        tutPlayer->resetAttackTimer(); // reset once after hitting all enemies
    }

    if (AEInputCheckTriggered(AEVK_E))
    {
        World::useInventoryItem(grid, inv, *tutPlayer);
    }

    if (AEInputCheckTriggered(AEVK_F9))
    {
        inv.giveItem(*tutPlayer, Inventory::ItemID::CARROT, 10);
        inv.giveItem(*tutPlayer, Inventory::ItemID::CHERRY, 10);
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
            grid.replacingID(World::Teleporter1, World::TeleporterBlue);
            World::dungeonTracker[World::checkNum] = true;
            tutPopup->show();
            World::standOnTile(next, *tutPlayer, grid, GS_FARM, World::TeleporterBlue);
        }
    }
}


void TutorialDungeon_Draw() {
    auto* tutPlayer = EntityManager::getPlayer("player");
    grid.drawTexture(grid);
    World::drawTile(tutActiveTile, grid);

    // debug: show weapon attack range
    float attackRange = 170; // i put 170 since i put it as i believe 85 as range?

    if (showInventory)
    {
        inv.draw();
    }

    if(Settings::gDebugMode)
    circle(tutPlayer->getX(), tutPlayer->getY(), attackRange, Shapes::CENTER);

    tutPlayer->draw();

    EntityManager::drawEnemies(*tutPlayer, grid, isPaused);

    if (Death::dead) {
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        Color::fill(255.0f, 255.0f, 255.0f, Death::opacity);
        Shapes::rect(0, 0, 1600, 900, Shapes::CENTER);

    }

    if (tutorialRooms[3].cleared)
    {
        World::drawIndicatorsOnTileType(grid, World::TeleporterBlue, point);
    }
}

void TutorialDungeon_Free() {
    //grid.unloadMapTexture();
    //World::freeGrid();
    EntityManager::clearEnemies();
    Death::dead = false;
    Death::opacity = 0.0f;

    auto* tutPlayer = EntityManager::getPlayer("player");
    inv.saveInventory(tutPlayer, gameData);
    inv.setPlayer(nullptr);
    grid.toggleWall();
}

void TutorialDungeon_Unload() {
}

