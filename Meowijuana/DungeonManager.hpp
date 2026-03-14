#ifndef DUNGEON_MANAGER_HPP
#define DUNGEON_MANAGER_HPP

#include "AEEngine.h"

namespace Room {

	// randomized:
	enum class RoomType
	{
		Main,
		SideSmall,
		SideMedium,
	};

	enum class Direction
	{
		North,
		East,
		West
	};


	// state:
	enum class RoomState
	{
		Unvisited,
		InCombat,
		Cleared
	};


	struct RoomNode
	{
		int id;

		RoomType type;
		RoomState state = RoomState::Unvisited;

		int depth; // linear. think more inscryption / slay the spire style room gen but no branches (just like in our git)

		// no south so no double back confusion
		RoomNode* north = nullptr;
		RoomNode* east = nullptr;
		RoomNode* west = nullptr;
		RoomNode* prev = nullptr;

		// ok i know this looks confusing but 
		// doorsLocked == no leaving mid-fight
		// room -> locked == no entering next room (pick your damn crops)
		bool locked = false;
	};



	// basically a linked list
	class DungeonManager {
	private:
		RoomNode* head = nullptr;
		RoomNode* currentRoom = nullptr;
		bool doorsLocked = false;
		int nextId = 0;

		RoomNode* createRoom(int depth);
		RoomType randomizeType();
		Direction randomizeDirection(RoomNode* from);
		void lockDoors();
		void unlockDoors();
		void onRoomEntered(RoomNode* room);
		void resetDungeon(RoomNode* node);

	public:
		DungeonManager() = default;
		~DungeonManager();

		void generateDungeon(int depth);
		bool move(Direction dir);
		void onRoomCleared();

		void lockNextRoom(RoomNode * room);
		void unlockNextRoom(RoomNode* room);

		RoomNode* getCurrentRoom() const;
		bool isDoorsLocked() const;
	};


}


#endif // DUNGEON_MANAGER_HPP