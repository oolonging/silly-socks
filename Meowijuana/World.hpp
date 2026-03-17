#ifndef WORLD_HPP
#define WORLD_HPP

#include "Graphics.hpp"
#include "Inventory.hpp"

namespace Entity {
	class Player;
	class Entity;

}

namespace UI_Elements {
	class PlayerInventory;
}

namespace World
{
	enum behaviourFlag
	{
		// -- Misc. -- //
		Ground, // normal ground
		Wall, // walls, obstacle objects
		InteractableObj, // chest etc.

		// -- Crops -- //
		// Might need Change Grown Crops to the diff crops instead
		EmptyCropTile,
		PlantedCropTile,
		GrownCropTile,
		DoneCropTile,

		// -- New Stuff -- //
		OOB,
		Teleporter,
		TeleporterOtherSide,
	};

	struct tileObject
	{
		int objID;
		std::string name;
		std::string description;
		AEGfxTexture* image = nullptr;
	};

	class worldGrid
	{
	private:
		std::vector<std::vector<int>> tilesID;
		std::unordered_map<int, tileObject> tileDatabase;
		int gridWidth, gridHeight, tileSize;
		int row, column;
		float offsetX, offsetY;

	public:
		/*
		Functions:
		REDOING EVERYTHING

		-> Initialise 

		-> Get Index (based on real word coordinates and translate that the Grid Tile)

		-> Get Tile ID (changing from array of tile objects to tile ID [lesser memory usage], gets tile ID based on index
		*/

		worldGrid();
		~worldGrid();

		worldGrid(const worldGrid&) = delete;
		worldGrid& operator=(const worldGrid&) = delete;

		// Initialize the grid with given dimensions
		void initGrid(int width, int height, int size);
		void initMapTexture();
		void outWorldMap(const std::string& filename);
		
		std::pair<int, int> getIndex(float cordX, float cordY) const;

		int& pointerToTile(int gridX, int gridY);
		const int getTileID(int gridX, int gridY) const;

		void fillGrid(const std::string& filename);

		// Get grid dimensions
		int getWidth() const { return gridWidth; }
		int getHeight() const { return gridHeight; }
		int getTileSize() const { return tileSize; }
		int getRow() const { return row; }
		int getColumn() const { return column; }
		float getOffsetX() const { return offsetX; }
		float getOffsetY() const { return offsetY; }
	
		void drawTexture(const World::worldGrid& Griddy);
		void initTextureBox();
		void unloadMapTexture();
	};

	// --- Tiler Helper functions --- //
	// Active Tile -> based on the user position and the mouse as well (ref stardew valley)
	std::pair<int, int> activeTile(float userX, float userY, const World::worldGrid& Griddy);

	// Get World Coords -> Translate Array index to real word coordinates
	std::pair<float, float> getWorldCoords(std::pair<int, int> tile, const World::worldGrid& Griddy);

	// --- Tile Colliison stuff --- //
	// Main function to handle collision
	//void collidableNearby(Entity::Player& user, World::worldGrid& Griddy);

	//// Helper to user collision function 
	//bool collideWithWall(Shapes::Quad user, Shapes::Quad wall);

	//void snapPlayer(Entity::Player& user, World::worldGrid& Griddy, bool* collisionCheck, bool* dir);

	// --- Tile Interaction stuff --- //
	void interactTile(std::pair<int, int> tile, World::worldGrid& Griddy, UI_Elements::PlayerInventory inven, Entity::Player& user);

	bool useItemOnTile(std::pair<int, int> activeTile, World::worldGrid& grid, UI_Elements::PlayerInventory inven, Entity::Player& user);

	// Based on user position -> check what tile user is on then change interactions respectively
	void standOnTile(int& next, Entity::Player user, World::worldGrid& Griddy, int nextlvl);

	// --- Tile Draw Stuff --- //
	void drawTile(std::pair<int, int> tile, const World::worldGrid& Griddy);

	void drawGrid();
	void freeGrid();

}


#endif // WORLD_HPP