#include "AEEngine.h"
#include "World.hpp"
#include "Graphics.hpp"

namespace World {

	AEGfxVertexList* gridMesh;

	static Grid grid;

	// Will use stated values so that it can be able to zoom
	void initGrid(int width, int height, float tileSize)
	{
		grid.width = width;
		grid.height = height;
		grid.tileSize = tileSize;

		grid.tiles = new Tile * [grid.height];

		for (int y = 0; y < grid.height; y++)
		{
			grid.tiles[y] = new Tile[grid.width];

			for (int x = 0; x < grid.width; x++)
			{
				Tile& tile = grid.tiles[y][x];
				tile.x = x;
				tile.y = y;
				tile.traversable = true;
				tile.interactable = false;
			}
		}

		float minX = -(grid.width / 2);
		float maxX = (grid.width / 2);
		float minY = -(grid.height / 2);
		float maxY = (grid.height / 2);

		// If empty then initialise the grid meshes stuff
		if (gridMesh == nullptr)
		{
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
		}
	}

	// Testing the Lines //

	void drawGrid()
	{
		// Ensure render state is valid
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		// Reset transform so grid doesn't follow entities
		AEMtx33 identity;
		AEMtx33Identity(&identity);
		AEGfxSetTransform(identity.m);

		// Black grid
		AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 1.f);
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);

		// Draw
		if (gridMesh) // SAFETY CHECK
			AEGfxMeshDraw(gridMesh, AE_GFX_MDM_LINES);

	}

	// Test ltr
	/*void updatGridSize(float tileSize)
	{
		grid.tileSize = tileSize;
		AEGfxMeshFree(gridMesh);
		gridMesh = nullptr;
	}*/

	void freeGrid()
	{
		for (int y = 0; y < grid.height; y++)
		{
			delete[] grid.tiles[y];
		}

		delete[] grid.tiles;

		AEGfxMeshFree(gridMesh);
		gridMesh = nullptr;
	}
}