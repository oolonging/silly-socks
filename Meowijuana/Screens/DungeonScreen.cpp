#include "../pch.h"
#include "DungeonScreen.hpp"

#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "../LevelSystem.hpp"
#include "../Entity.hpp"
#include "../World.hpp"
#include "../TileTypes.hpp"
#include "../Managers/EntityManager.hpp"
#include "../DungeonManager.hpp"

World::worldGrid DungeonGrid;
std::pair<int, int> prevActive;
std::pair<int, int> dungeonActiveTile;
Room::DungeonManager dungeon;
static bool roomNeedsRedraw = false;


void Dungeon_Load() {


}


void Dungeon_Initialize() {

    EntityManager::init();

    DungeonGrid.initGrid(AEGfxGetWindowWidth(), AEGfxGetWindowHeight(), 50);
    DungeonGrid.initMapTexture();
    DungeonGrid.initTextureBox();
    dungeon.generateDungeon(3);


    // draw starting room
    auto* room = dungeon.getCurrentRoom();
    DungeonGrid.fillGrid(Room::getRoomFile(room->type));

    auto* player = EntityManager::getPlayer("player");
    player->setPosition(0.0f, 50.0f);

    DungeonGrid.outWorldMap("../../Assets/LevelMaps/Check.txt");
}


void Dungeon_Update() {
    auto* player = EntityManager::getPlayer("player");
    player->update(DungeonGrid);
    player->tickAttackTimer(); // TODO: combat

    bool moved = false;
    Room::Direction dir = Room::Direction::North; // default, gets overwritten

    float hw = AEGfxGetWindowWidth() / 2.0f;
    float hh = AEGfxGetWindowHeight() / 2.0f;

    dungeonActiveTile = World::activeTile(player->getX(), player->getY(), DungeonGrid);

    // just a lot of boundary checking  vv
    if (player->getY() > hh) {
        dir = Room::Direction::North;
        moved = dungeon.move(dir);
    }

    else if (player->getX() > hw) {
        dir = Room::Direction::East;
        moved = dungeon.move(dir);
    }
    
    else if (player->getX() < -hw) {
        dir = Room::Direction::West;
        moved = dungeon.move(dir);
    }
    
    else if (player->getY() < -hh) {
        if (dungeon.getCurrentRoom()->type == Room::RoomType::Main) {
            dungeon.lockNextRoom(dungeon.getCurrentRoom());
            next = GS_FARM;
        }
        else {
            dir = Room::Direction::South;
            moved = dungeon.move(dir);
        }
    }



    // making sure player comes from the right side
    if (moved) {
        auto* room = dungeon.getCurrentRoom();
        DungeonGrid.fillGrid(Room::getRoomFile(room->type));

        if (dir == Room::Direction::North) player->setPosition(0.0f, -hh + 80.0f);
        else if (dir == Room::Direction::East)  player->setPosition(-hw + 80.0f, 0.0f);
        else if (dir == Room::Direction::West)  player->setPosition(hw - 80.0f, 0.0f);
        else if (dir == Room::Direction::South) player->setPosition(0.0f, hh - 80.0f);
    }
}

void Dungeon_Draw() {
  
    DungeonGrid.drawTexture(DungeonGrid);
    World::drawTile(dungeonActiveTile, DungeonGrid);

    // ---------- ENTITIES  (gotta figure out entity spawning) ------------------

    EntityManager::draw("player");
}

void Dungeon_Free() {

    DungeonGrid.unloadMapTexture();
    World::freeGrid();
}


void Dungeon_Unload() {
    EntityManager::clear();
}

