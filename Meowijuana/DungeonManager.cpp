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
		}


		if (!next) return false; // no room there
		if (next->locked) return false; // lock so that they actually harvest their crops

		onRoomEntered(next);
		return true;
	}


	void DungeonManager::resetDungeon(RoomNode* node) {
		while (node) {
			RoomNode* next = node->north ? node->north : node->east ? node->east : node->west;  // yeah nah this was fun to write

			delete node;
			node = next;
		}
	}

	Direction DungeonManager::randomizeDirection(RoomNode* room) {
		static const Direction valid[3] = {
			Direction::North,
			Direction::East,
			Direction::West
		};
		return valid[rand() % 3];
	}

	void DungeonManager::lockDoors() { doorsLocked = true; }
	void DungeonManager::unlockDoors() { doorsLocked = false; }


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

		if (room->state == RoomState::Unvisited)
		{
			lockDoors(room);
			room->state = RoomState::InCombat;
		}
	}


	void DungeonManager::onRoomCleared() {
		if (currentRoom) {
			currentRoom->state = RoomState::Cleared;
			unlockDoors(currentRoom);
		}
	}

	//bool DungeonManager::canLeaveRoom() {
	//	return currentRoom->state == RoomState::InCombat? 0 : 1;
	//}

	//RoomNode* DungeonManager::isInRoom() {
	//	return ;

	//}



	RoomNode* DungeonManager::createRoom(int depth) {
		RoomNode* newRoom = new RoomNode{};
		newRoom->id = currentRoom->id++;
		newRoom->depth = depth;
		newRoom->type = randomizeType();

	}
}

