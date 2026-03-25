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

static Entity::Player* localPlayer = nullptr;

static UI_Elements::PopupBox* tutPopup;

//  TODO : CHARACTER DEATH, MAKE ROOMS MORE INTERESTING, ENTITY COLLISION WITH WALL, PARTICLES IF POSSIBLE, KNOCKBACK???????
namespace Death {
    float opacity = 0.0f;
    bool dead = false;
    int deathCounter = 0;
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
    localPlayer = EntityManager::getPlayer("player");

    inv.setPlayer(EntityManager::getPlayer("player"));
    inv.loadInventory(localPlayer, gameData);

    Inventory::Weapon* pWeapon = dynamic_cast<Inventory::Weapon*>(Inventory::ItemRegistry::createItem(Inventory::ItemID::CARROT_SWORD));
    if (pWeapon) {
        localPlayer->setWeapon(pWeapon);
        localPlayer->setAtkSpd(pWeapon->getAttackSpeed());
    }

    tutPopup = UIManager::create<UI_Elements::PopupBox>("tutPopup", 120.0f, -80.0f, 500.0f, 250.0f, "You have cleared the tutorial!", "You will be on your own now", "Please explore the dungeons and save the world!");
    tutPopup->setOnDismiss([]() {
        tutPopup->hide();
        });

    grid.toggleWall();

    LoadRoom(tutorialRoomIndex);
}



void TutorialDungeon_Update() {

    tutActiveTile = World::activeTile(localPlayer->getX(), localPlayer->getY(), grid);

    localPlayer->update(grid);
    localPlayer->tickAttackTimer();

    inv.update(localPlayer);

    EntityManager::updateEnemies(*localPlayer);


    if (AEInputCheckTriggered(AEVK_LBUTTON) && localPlayer->canAttack()) {
        EntityManager::attackEnemies(*localPlayer);
        World::checkCarrotSwordConsume(inv, *localPlayer);
        localPlayer->resetAttackTimer(); // reset once after hitting all enemies
    }

    // use item upon pressing E
    if (AEInputCheckTriggered(AEVK_E))
    {
        World::useInventoryItem(grid, inv, *localPlayer);
    }

    if (AEInputCheckTriggered(AEVK_F9))
    {
        inv.giveItem(*localPlayer, Inventory::ItemID::CARROT, 10);
        inv.giveItem(*localPlayer, Inventory::ItemID::CHERRY, 10);
    }

    if (!tutorialRooms[tutorialRoomIndex].cleared && EntityManager::allEnemiesDead()) {
        tutorialRooms[tutorialRoomIndex].cleared = true;
    }


    if (AEInputCheckTriggered(AEVK_F10))
    {
        Death::deathCounter = 3;
    }

    float halfHeight = AEGfxGetWindowHeight() * 0.5f;
    float halfWidth = AEGfxGetWindowWidth() * 0.5f;
    float savedX = localPlayer->getX();
    float savedY = localPlayer->getY();

    if (tutorialRooms[tutorialRoomIndex].cleared && EntityManager::allEnemiesDead()) {

        if (localPlayer->getY() >= halfHeight && tutorialRoomIndex < 3) {
            tutorialRoomIndex++;

            LoadRoom(tutorialRoomIndex);
            //auto* tutPlayer = EntityManager::getPlayer("player");
            localPlayer->setPosition(savedX, -halfHeight + 10.0f);
        }

        else if (localPlayer->getY() <= -halfHeight && tutorialRoomIndex > 0) {
            tutorialRoomIndex--;
            LoadRoom(tutorialRoomIndex);
            localPlayer->setPosition(savedX, halfHeight - 10.0f);
        }

        if (localPlayer->getX() >= halfWidth && tutorialRoomIndex < 3) {
            tutorialRoomIndex--;
            LoadRoom(tutorialRoomIndex);
            localPlayer->setPosition(-halfWidth + 10.0f, savedY);
        }

        else if (localPlayer->getX() <= -halfWidth && tutorialRoomIndex > 0) {
            tutorialRoomIndex++;
            LoadRoom(tutorialRoomIndex);
            localPlayer->setPosition(halfWidth - 10.0f, savedY);
        }

        if (tutorialRooms[3].cleared) {
            grid.replacingID(World::Teleporter1, World::TeleporterBlue);
            World::dungeonTracker[World::checkNum] = true;
            tutPopup->show();
            World::standOnTile(next, *localPlayer, grid, GS_FARM, World::TeleporterBlue);
        }
    }
}


void TutorialDungeon_Draw() {
    grid.drawTexture(grid);
    World::drawTile(tutActiveTile, grid);

    // debug: show weapon attack range
    float attackRange = 170; // i put 170 since i put it as i believe 85 as range?

    if (showInventory)
    {
        inv.draw();
    }

    if(Settings::gDebugMode)
    circle(localPlayer->getX(), localPlayer->getY(), attackRange, Shapes::CENTER);


    EntityManager::drawEnemies(*localPlayer, grid, isPaused);

    if (tutorialRooms[3].cleared)
    {
        World::drawIndicatorsOnTileType(grid, World::TeleporterBlue, point);
    }

    // Draw the player last
    localPlayer->draw();
}

void TutorialDungeon_Free() {
    //grid.unloadMapTexture();
    //World::freeGrid();
    EntityManager::clearEnemies();

    inv.saveInventory(localPlayer, gameData);
    inv.setPlayer(nullptr);
    grid.toggleWall();

    EntityManager::clear();
}

void TutorialDungeon_Unload() {
}

