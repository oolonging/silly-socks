#ifndef DUNGEON_MANAGER_HPP
#define DUNGEON_MANAGER_HPP

#include "AEEngine.h"

namespace Room {

	enum class RoomType
	{
		Main,
		SideSmall,
		SideMedium,
	};

	enum class Direction
	{
		North,
		South,
		East,
		West
	};

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

		int level;
		int depth; // linear. think more inscryption / slay the spire style room gen but no branches (just like in our git)

		RoomNode* north = nullptr;
		RoomNode* south = nullptr;
		RoomNode* east = nullptr;
		RoomNode* west = nullptr;
	};


	RoomNode* currentRoom = nullptr;
	bool doorsLocked = false;
}


#endif // DUNGEON_MANAGER_HPP