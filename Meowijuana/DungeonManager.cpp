#include "pch.h"
#include "DungeonManager.hpp"


// just a lot of incomplete stuff (still). 

namespace Room {
	void DungeonManager::generateDungeon(int depth) {
		resetDungeon(head);
		head = nullptr;
		nextId = 0;

		RoomNode* prev = nullptr;

		// make the rooms
		for (int i = 0; i < depth; ++i) {
			RoomNode* room = createRoom(i);

			// first room main room
			if (i == 0) {
				head = room;
				head->type = RoomType::Main;
			}

			// has previous? random direction & make room there
			if (prev != nullptr) {
				Direction dir = randomizeDirection(prev);

				switch (dir) {
				case Direction::North: prev->north = room;
					break;

				case Direction::East: prev->east = room;
					break; 

				case Direction::West: prev->west = room;
					break;
				}

				room->prev = prev;
			}

			prev = room;
		}

		onRoomEntered(head);
	}

	DungeonManager::~DungeonManager(){
		resetDungeon(head);
	}

	bool DungeonManager::move(Direction dir) {
		if (doorsLocked) return false;

		RoomNode* next = nullptr;
		switch (dir) {
		case Direction::North: next = currentRoom->north;
			break;

		case Direction::East:  next = currentRoom->east;
			break;

		case Direction::West:  next = currentRoom->west;
			break;

		// added south because i forgot backtracking was a thing
		case Direction::South:  next = currentRoom->prev;
			break;

		}


		if (!next) return false; // no room there
		if (next->locked) return false; // lock so that they actually harvest their crops

		onRoomEntered(next);
		return true;
	}


	void DungeonManager::resetDungeon(RoomNode* node) {
		// while not nptr, find node->(whicheverdirection) >> delete >> move down the sllist
		while (node) {
			RoomNode* next = node->north ? node->north : node->east ? node->east : node->west;

			delete node;
			node = next;
		}
	}


	Direction DungeonManager::randomizeDirection(RoomNode* room) {

		// if it exist and the room was previously (direction), ban going the opposite direction 
		if (room->prev && room->prev->east == room) {
			return rand() % 2 ? Direction::North : Direction::West;
		}

		// e.g if it's: main room -> east, DONT GENERATE WEST 
		if (room->prev && room->prev->west == room) {
			return rand() % 2 ? Direction::North : Direction::East;
		}

		// north is fine though every direction is ok
		Direction available[3];
		int count = 0;
		if (!room->north) available[count++] = Direction::North;
		if (!room->east)  available[count++] = Direction::East;
		if (!room->west)  available[count++] = Direction::West;
		if (count == 0)   return Direction::North;
		return available[rand() % count];

	}

	// lock n unlock for doors (TODO cause enemy spawn)
	void DungeonManager::lockDoors() { doorsLocked = true; }
	void DungeonManager::unlockDoors() { doorsLocked = false; }

	// either small room or medium room (i wanna add more rooms too but uhhhh if i have time)
	RoomType DungeonManager::randomizeType() {
		return rand() % 2 ? RoomType::SideSmall : RoomType::SideMedium;
	}

	// reminder that doors are locked til plants harvested because this is the third time i'm staring at room like ???

	RoomNode* DungeonManager::getCurrentRoom() const {
		return currentRoom; 
	}

	bool DungeonManager::isDoorsLocked() const {
		return doorsLocked;
	}

	void DungeonManager::onRoomEntered(RoomNode* room) {
		currentRoom = room;
		if (room->state == RoomState::Unvisited) {
			// lockDoors();  // re-enable when combat is hooked up (for now free to walk, tho no visual ind to what's open)
			room->state = RoomState::InCombat;
		}
	}


	void DungeonManager::onRoomCleared() {
		if (currentRoom) {
			currentRoom->state = RoomState::Cleared;
			unlockDoors();
		}
	}

	//bool DungeonManager::canLeaveRoom() {
	//	return currentRoom->state == RoomState::InCombat? 0 : 1;
	//}

	//RoomNode* DungeonManager::isInRoom() {
	//	return ;

	//}

	// locking (still, no combat)
	void DungeonManager::lockNextRoom(RoomNode* room) { if (room) room->locked = true; }
	void DungeonManager::unlockNextRoom(RoomNode* room) { if (room) room->locked = false; }


	// assignment when making new node
	RoomNode* DungeonManager::createRoom(int depth) {
		RoomNode* newRoom = new RoomNode{};
		newRoom->id = nextId++;
		newRoom->depth = depth;
		newRoom->type = randomizeType();
		return newRoom;
	}


	// room data for loading 
	std::string getRoomFile(RoomType type) {
		switch (type) {
		case RoomType::Main:       return "Assets/LevelMaps/DungeonList/MAIN.txt";
		case RoomType::SideSmall:  return "Assets/LevelMaps/DungeonList/SIDE_SMALL.txt";
		case RoomType::SideMedium: return "Assets/LevelMaps/DungeonList/SIDE_MEDIUM.txt";
		}
		return "";
	}

}



