#include "AEEngine.h"
#include "World.hpp"
#include "Graphics.hpp"

namespace World {


	// Will use stated values so that it can be able to zoom
	void initGrid(Grid* grid, int width, int height, float tileSize)
	{
		grid->width = width;
		grid->height = height;
		grid->tileSize = tileSize;

		grid->tiles = new Tile * [grid->height];

		for (int y = 0; y < grid->height; y++)
		{
			grid->tiles[y] = new Tile[grid->width];

			for (int x = 0; x < grid->width; x++)
			{
				Tile& tile = grid->tiles[y][x];
				tile.x = x;
				tile.y = y;
				tile.traversable = true;
				tile.interactable = false;
			}
		}
	}

	void drawGrid(Grid const& grid)
	{
		float minX = AEGfxGetWinMinX(); // Left edge
		float maxY = AEGfxGetWinMaxY(); // Top edge

		for (int y = 0; y < grid.height; y++)
		{
			for (int x = 0; x < grid.width; x++)
			{
				float screenX = minX + x * grid.tileSize;
				float screenY = maxY - y * grid.tileSize;

				// Right now drawing squares will actually make the game super laggy i'll try
				// to figure out how to draw lines instead?

				Shapes::square(
					screenX + grid.tileSize * 0.5f,
					screenY - grid.tileSize * 0.5f,
					grid.tileSize
				);
			}
		}
	}

	void freeGrid(Grid* grid)
	{
		for (int y = 0; y < grid->height; y++)
			delete[] grid->tiles[y];

		delete[] grid->tiles;
	}
}