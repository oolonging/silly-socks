#include "AEEngine.h"
#include "World.hpp"
#include "Graphics.hpp"

namespace World {

	AEGfxVertexList* gridMesh;

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
		//float minX = AEGfxGetWinMinX(); // Left edge
		//float maxY = AEGfxGetWinMaxY(); // Top edge

		//for (int y = 0; y < grid.height; y++)
		//{
		//	for (int x = 0; x < grid.width; x++)
		//	{
		//		float screenX = minX + x * grid.tileSize;
		//		float screenY = maxY - y * grid.tileSize;

		//		// Right now drawing squares will actually make the game super laggy i'll try
		//		// to figure out how to draw lines instead?

		//		Shapes::square(
		//			screenX + grid.tileSize * 0.5f,
		//			screenY - grid.tileSize * 0.5f,
		//			grid.tileSize
		//		);
		//	}
		//}



		// Testing the Lines //

		float minX = AEGfxGetWinMinX();
		float maxX = AEGfxGetWinMaxX();
		float minY = AEGfxGetWinMinY();
		float maxY = AEGfxGetWinMaxY();

		AEGfxMeshStart();

		// Horizontal lines
		for (int y = 0; y <= grid.height; y++)
		{
			float screenY = maxY - y * grid.tileSize;
			AEGfxVertexAdd(minX, screenY, 0xFFFFFFFF, 1.f, 1.f);
			AEGfxVertexAdd(maxX, screenY, 0xFFFFFFFF, 1.f, 1.f);
		}

		// Vertical lines
		for (int x = 0; x <= grid.width; x++)
		{
			float screenX = minX + x * grid.tileSize;
			AEGfxVertexAdd(screenX, maxY, 0xFFFFFFFF, 1.f, 1.f);
			AEGfxVertexAdd(screenX, minY, 0xFFFFFFFF, 1.f, 1.f);
		}

		gridMesh = AEGfxMeshEnd();

		// Basically resets the render transform state so it doesn't follow ennemy
		AEMtx33 identity;
		AEMtx33Identity(&identity);

		AEGfxSetTransform(identity.m);

		AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 1.f);
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);

		AEGfxMeshDraw(gridMesh, AE_GFX_MDM_LINES);

	}

	void freeGrid(Grid* grid)
	{
		for (int y = 0; y < grid->height; y++)
			delete[] grid->tiles[y];

		delete[] grid->tiles;

		AEGfxMeshDraw(gridMesh, AE_GFX_MDM_LINES);
	}
}