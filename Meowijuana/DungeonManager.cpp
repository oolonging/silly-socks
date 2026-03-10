//#include "DungeonManager.hpp"
//

// just a lot of incomplete stuff, dont mind it. 
// wanted to do something along the lines of randomly generating a direction and a room type and saving it into minimap
// to display 

//Direction RandomizeDirection() {
//	int randomized = rand() % 4;
//	return static_cast<Direction>(randomized);
//}
//
//RoomType RandomizeType() {
//	return rand() % 2 ? RoomType::SideSmall : RoomType::SideMedium;
//}
//
//
//
//void EnterRoom(RoomNode* room) {
//	currentRoom = room;
//
//	if (room->state == RoomState::Unvisited)
//	{
//		LockDoors(room);
//		room->state = RoomState::InCombat;
//	}
//}
//
//void LockDoors(RoomNode* room) {
//	doorsLocked = true;
//}
//
//void UnlockDoors(RoomNode* room) {
//	doorsLocked = false;
//}
//
//void OnRoomCleared(RoomNode* room) {
//	room->state = RoomState::Cleared;
//	UnlockDoors(room);
//}
//
//bool CanLeaveRoom() {
//	return currentRoom->state != RoomState::InCombat;
//}
//
//void EnteredRoom(RoomNode* room) {
//
//}
//
//
//
//
//RoomNode* GenerateRooms(int depth) {
//	RoomNode* newRoom = new Room::RoomNode{};
//
//}