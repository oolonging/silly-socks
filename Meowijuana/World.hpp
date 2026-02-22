#ifndef WORLD_HPP
#define WORLD_HPP

#include "AEEngine.h"
#include <vector>
#include <string>

namespace World
{
	enum behaviourFlag
	{
		walkable = 0, // normal ground
		solid = 1, // walls, obstacle objects
		interactable = 2, // chest etc.
		teleport = 3, // teleport to other floors
		trap = 4

		// Will add more ltr on	
	};

	struct tileObject
	{
		int objID;
		std::string name;
		std::string description;
		AEGfxTexture* image;
		// replaces the bool so can have event handler that will check based on the flag
		// plus easier to add more ways for player to interact w object 
		int behaviour;
	};

	class worldGrid
	{
	private:
		std::vector<std::vector<tileObject>> tiles;
		int gridWidth, gridHeight, tileSize;
		float offsetX, offsetY;

	public:
		/*
		Functions:
		- Initialize : initialise the grid
		- ReadMap : maybe implement a way to read maps based on txt file
		- GetTile : return the tile type
		- SetTIle : set new tile
		- Free : free everything 
		- SnapToCell : return new coord of where the obj should be 
		- isWalkable : return bool if the obj is walkable
		- isInteractable : same as walkable but for interactable
		- Collision : basically like Soroor assignment 3 part 1
		*/

		worldGrid();
		~worldGrid();

		// Initialize the grid with given dimensions
		void initGrid(int width, int height, int size);
		
		std::pair<int, int> getIndex(float cordX, float cordY);

		tileObject* getTile(int gridX, int gridY);
		const tileObject* getTile(int gridX, int gridY) const;

		void setTile(int gridX, int gridY, tileObject* ob);

		// Get grid dimensions
		int getWidth() const { return gridWidth; }
		int getHeight() const { return gridHeight; }
		int getTileSize() const { return tileSize; }
	};

	void drawGrid();
	void freeGrid();
}


#endif // WORLD_HPP