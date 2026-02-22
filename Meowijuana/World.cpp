#include "AEEngine.h"
#include "World.hpp"
#include "Graphics.hpp"

World::worldGrid grid;

namespace World {

	AEGfxVertexList* gridMesh;

	worldGrid::worldGrid() : gridWidth(0), gridHeight(0), tileSize(0), offsetX(0.0f), offsetY(0.0f) {}

	worldGrid::~worldGrid() {
		tiles.clear();
	}

	// Initialize the grid with given dimensions
	void worldGrid::initGrid(int width, int height, int size)
	{
		gridWidth = width;
		gridHeight = height;
		tileSize = size;

		// Calculate offset to center the grid on screen
		// offsetX: center horizontally (negative to shift left)
		// offsetY: center vertically and position top edge at positive Y
		offsetX = -(gridWidth * tileSize) / 2.0f;
		offsetY = (gridHeight * tileSize) / 2.0f;

		// Resize the 2D vector
		tiles.resize(gridHeight);
		for (int y = 0; y < gridHeight; ++y) {
			tiles[y].resize(gridWidth);
		}

		float leftX = offsetX;
		float rightX = offsetX + gridWidth * tileSize;

		float topY = offsetY;
		float bottomY = offsetY - gridHeight * tileSize;

		if (gridMesh == nullptr)
		{
			AEGfxMeshStart();

			// Horizontal lines
			for (int y = 0; y <= gridHeight; ++y)
			{
				float screenY = topY - y * tileSize;

				AEGfxVertexAdd(leftX, screenY, 0xFFFFFFFF, 1.f, 1.f);
				AEGfxVertexAdd(rightX, screenY, 0xFFFFFFFF, 1.f, 1.f);
			}

			// Vertical lines
			for (int x = 0; x <= gridWidth; ++x)
			{
				float screenX = leftX + x * tileSize;

				AEGfxVertexAdd(screenX, topY, 0xFFFFFFFF, 1.f, 1.f);
				AEGfxVertexAdd(screenX, bottomY, 0xFFFFFFFF, 1.f, 1.f);
			}

			gridMesh = AEGfxMeshEnd();
		}
	}

	std::pair<int, int> worldGrid::getIndex(float cordX, float cordY)
	{
		int gridX, gridY;

		gridX = static_cast<int>(floor((cordX - offsetX) / tileSize));
		gridY = static_cast<int>(floor((offsetY - cordY) / tileSize));

		std::pair<int, int> index = std::make_pair(gridX, gridY);

		return index;
	}

	tileObject* worldGrid::getTile(int gridX, int gridY)
	{
		return &tiles[gridX][gridY];
	}

	const tileObject* worldGrid::getTile(int gridX, int gridY) const
	{
		return &tiles[gridX][gridY];
	}

	void worldGrid::setTile(int gridX, int gridY, tileObject* ob)
	{
		tiles[gridX][gridY].objID = ob->objID;
		tiles[gridX][gridY].name = ob->name;
		tiles[gridX][gridY].description = ob->description;
		tiles[gridX][gridY].image = ob->image;
		tiles[gridX][gridY].behaviour = ob->behaviour;
	}

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
		if (gridMesh)
		{
			AEGfxMeshDraw(gridMesh, AE_GFX_MDM_LINES);
		}
	}

	void freeGrid()
	{
		AEGfxMeshFree(gridMesh);
		gridMesh = nullptr;
	}

	//// Will use stated values so that it can be able to zoom
	//void initGrid(int width, int height, float tileSize)
	//{
	//	grid.width = width;
	//	grid.height = height;
	//	grid.tileSize = tileSize;

	//	grid.tiles = new Tile * [grid.height];

	//	for (int y = 0; y < grid.height; y++)
	//	{
	//		grid.tiles[y] = new Tile[grid.width];

	//		for (int x = 0; x < grid.width; x++)
	//		{
	//			Tile& tile = grid.tiles[y][x];
	//			tile.x = x;
	//			tile.y = y;
	//		}
	//	}

	//	float minX = -(static_cast<float>(grid.width / 2));
	//	float maxX = (static_cast<float>(grid.width / 2));
	//	float minY = -(static_cast<float>(grid.height / 2));
	//	float maxY = (static_cast<float>(grid.height / 2));

	//	// If empty then initialise the grid meshes stuff
	//	if (gridMesh == nullptr)
	//	{
	//		AEGfxMeshStart();
	//		// Horizontal lines
	//		for (int y = 0; y <= grid.height; y++)
	//		{
	//			float screenY = maxY - y * grid.tileSize;
	//			AEGfxVertexAdd(minX, screenY, 0xFFFFFFFF, 1.f, 1.f);
	//			AEGfxVertexAdd(maxX, screenY, 0xFFFFFFFF, 1.f, 1.f);
	//		}

	//		// Vertical lines
	//		for (int x = 0; x <= grid.width; x++)
	//		{
	//			float screenX = minX + x * grid.tileSize;
	//			AEGfxVertexAdd(screenX, maxY, 0xFFFFFFFF, 1.f, 1.f);
	//			AEGfxVertexAdd(screenX, minY, 0xFFFFFFFF, 1.f, 1.f);
	//		}

	//		gridMesh = AEGfxMeshEnd();
	//	}
	//}

	// Testing the Lines //

	//void drawGrid()
	//{
	//	// Ensure render state is valid
	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//	AEGfxSetTransparency(1.0f);

	//	// Reset transform so grid doesn't follow entities
	//	AEMtx33 identity;
	//	AEMtx33Identity(&identity);
	//	AEGfxSetTransform(identity.m);

	//	// Black grid
	//	AEGfxSetColorToMultiply(0.f, 0.f, 0.f, 1.f);
	//	AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);

	//	// Draw
	//	if (gridMesh)
	//	{
	//		AEGfxMeshDraw(gridMesh, AE_GFX_MDM_LINES);
	//	}
	//}

	// Test ltr
	/*void updatGridSize(float tileSize)
	{
		grid.tileSize = tileSize;
		AEGfxMeshFree(gridMesh);
		gridMesh = nullptr;
	}*/


	/*void freeGrid()
	{
		for (int y = 0; y < worldGrid::getHeight; y++)
		{
			delete[] grid.tiles[y];
		}

		delete[] grid.tiles;

		AEGfxMeshFree(gridMesh);
		gridMesh = nullptr;
	}*/
}