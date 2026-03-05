#ifndef WORLD_HPP
#define WORLD_HPP

#include "AEEngine.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace World
{
	enum behaviourFlag
	{
		// -- Misc. -- //
		Ground, // normal ground
		Wall, // walls, obstacle objects
		InteractableObj, // chest etc.

		// -- Crops -- //
		EmptyCropTile,
		PlantedCropTile,
		GrownCropTile,
		DoneCropTile

	};

	struct tileObject
	{
		int objID;
		std::string name;
		std::string description;
		AEGfxTexture* image = nullptr;;
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

	void drawGrid();
	void freeGrid();

	// Active Tile -> based on the user position and the mouse as well (ref stardew valley)
	std::pair<int, int> activeTile(float userX, float userY, const World::worldGrid& Griddy);
	
	// Get World Coords -> Translate Array index to real word coordinates
	std::pair<float, float> getWorldCoords(std::pair<int, int> tile, const World::worldGrid& Griddy);
	
	void drawTile(std::pair<int, int> tile, const World::worldGrid& Griddy);

	void interactTile(std::pair<int, int> tile, World::worldGrid& Griddy);

}


#endif // WORLD_HPP