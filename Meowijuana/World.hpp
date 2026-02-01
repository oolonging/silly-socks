#ifndef WORLD_HPP
#define WORLD_HPP

namespace World
{
	//typedef struct grid
	//{
	//	// array for the grid
	//	// tiles
	//	tileObject* tilesObj;

	//};
	//
	//typedef struct tileObject
	//{
	//	int x, y;
	//	bool traverseable, interactable;

	//	// object sprite

	//};


	typedef struct Tile
	{
		int x, y;
		bool traversable, interactable;
		// Maybe can put pointer to entity
		// Tile texture maybe

	} Tile;

	typedef struct Grid
	{
		int width, height;
		float tileSize;
		Tile** tiles;   // 2D array of tiles

	} Grid;

	void initGrid(Grid* grid, int width, int height, float tileSize);
	void drawGrid(Grid const& grid);
	void freeGrid(Grid* grid);
	
}


#endif // WORLD_HPP