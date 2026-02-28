#include "Tiles.hpp"
#include "Managers/TileManager.hpp"
#include <iostream>

namespace World {
	// Define grid
	Tile grid[GRID_HEIGHT][GRID_WIDTH];

	// Tile class implementation
	Tile::Tile() : worldX(0.0f), worldY(0.0f), gridX(0), gridY(0), pTileObject(nullptr) {}

	void Tile::setWorldPosition(float x, float y) {
		this->worldX = x;
		this->worldY = y;
	}

	void Tile::setGridPosition(int x, int y) {
		this->gridX = x;
		this->gridY = y;
	}

	void Tile::setTileObject(TileObject* pTileObject) {
		this->pTileObject = pTileObject;
	}

	void Tile::draw(void) {
		bool hasTile = this->pTileObject != nullptr;
		bool graphicalDraw = hasTile && this->pTileObject->pTex != nullptr;

		if (graphicalDraw) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			Graphics::image(this->worldX, this->worldY, GRID_SIZE, GRID_SIZE, this->getTileObject()->pTex);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);

			// Draw tiles based on their type
			if (hasTile) {
				// Different colors for different tile types
				switch (this->pTileObject->id) {
				case 1: // wall
					Color::fill(100, 100, 100, 255); // Gray
					break;
				case 2: // door
					Color::fill(139, 69, 19, 255); // Brown
					break;
				case 3: // water
					Color::fill(0, 0, 255, 255); // Blue
					break;
				case 4: // grass
					Color::fill(0, 255, 0, 255); // Green
					break;
				default:
					Color::fill(255, 0, 0, 255); // Red for unknown
				}
				Color::stroke(0, 0, 0, 255); // Black border
				Color::strokeWeight(2.0f);
				Shapes::rect(this->worldX, this->worldY, GRID_SIZE, GRID_SIZE, Shapes::CORNER);
			}
			// Don't draw empty tiles
		}
	}

	// Grid functions

	bool mouseOnTile(Tile& const tile) {
		float mouseX = Input::getMouseX();
		float mouseY = Input::getMouseY();

		bool xOverlap = mouseX > tile.getWorldX() && mouseX < tile.getWorldX() + GRID_SIZE;
		bool yOverlap = mouseY < tile.getWorldY() && mouseY > tile.getWorldY() - GRID_SIZE;
		return xOverlap && yOverlap;
	}

	Tile& getTileFromWorld(float x, float y) {
		int row = static_cast<int>((AEGfxGetWindowHeight() * 0.5f - y) / GRID_SIZE);
		int col = static_cast<int>((x + AEGfxGetWindowWidth() * 0.5f) / GRID_SIZE);

		// Bounds checking
		if (row < 0) row = 0;
		if (row >= GRID_HEIGHT) row = GRID_HEIGHT - 1;
		if (col < 0) col = 0;
		if (col >= GRID_WIDTH) col = GRID_WIDTH - 1;

		return grid[row][col];
	}

	void init(void) {
		std::cout << "World::init() called" << std::endl;

		float tileWidth = static_cast<float>(GRID_SIZE);
		float tileHeight = static_cast<float>(GRID_SIZE);

		float startX = -AEGfxGetWindowWidth() * 0.5f;
		float startY = AEGfxGetWindowHeight() * 0.5f;

		std::cout << "Grid start position: (" << startX << ", " << startY << ")" << std::endl;

		for (int row = 0; row < GRID_HEIGHT; ++row) {
			for (int col = 0; col < GRID_WIDTH; ++col) {
				float worldX = startX + col * tileWidth;
				float worldY = startY - row * tileHeight;

				grid[row][col].setGridPosition(col, row);
				grid[row][col].setWorldPosition(worldX, worldY);
				grid[row][col].setTileObject(nullptr); // Ensure tiles start empty
			}
		}

		std::cout << "World::init() complete - Grid initialized" << std::endl;
	}

	void draw(void) {
		static bool firstDraw = true;
		static int tilesWithObjects = 0;

		if (firstDraw) {
			// Count tiles with objects
			tilesWithObjects = 0;
			for (int row = 0; row < GRID_HEIGHT; ++row) {
				for (int col = 0; col < GRID_WIDTH; ++col) {
					if (grid[row][col].hasTileObject()) {
						tilesWithObjects++;
						if (tilesWithObjects <= 5) {
							std::cout << "Tile[" << row << "][" << col << "] at ("
								<< grid[row][col].getWorldX() << ", " << grid[row][col].getWorldY()
								<< ") has TileObject ID: " << grid[row][col].getTileObject()->id << std::endl;
						}
					}
				}
			}
			std::cout << "First draw: Found " << tilesWithObjects << " tiles with objects" << std::endl;
			firstDraw = false;
		}

		for (int row = 0; row < GRID_HEIGHT; ++row) {
			for (int col = 0; col < GRID_WIDTH; ++col) {
				Tile& tile = grid[row][col];
				

				// Draw tile
				tile.draw();

				if (mouseOnTile(tile)) {
					if (AEInputCheckTriggered(AEVK_LBUTTON)) {
						// Use TileManager to get wall tile
						World::TileObject* wallType = TileManager::getTileType("wall");
						if (wallType) {
							tile.setTileObject(wallType);
							std::cout << "Added wall at grid[" << row << "][" << col << "]" << std::endl;
						}
						else {
							std::cout << "ERROR: Could not get wall tile type!" << std::endl;
						}
					}
					if (AEInputCheckTriggered(AEVK_RBUTTON)) {
						tile.clearTileObject();
						std::cout << "Cleared tile at grid[" << row << "][" << col << "]" << std::endl;
					}

					// if mouse on tile draw highlight
					AEGfxSetRenderMode(AE_GFX_RM_COLOR);
					Color::fill(255, 255, 0, 100); // Yellow with some transparency
					Shapes::rect(tile.getWorldX(), tile.getWorldY(), GRID_SIZE, GRID_SIZE, Shapes::CORNER);
				}

			}
		}
	}

	void exit(void) {
		std::cout << "World::exit() called" << std::endl;

		// Clear all tile references
		for (int row = 0; row < GRID_HEIGHT; ++row) {
			for (int col = 0; col < GRID_WIDTH; ++col) {
				grid[row][col].clearTileObject();
			}
		}

		std::cout << "World::exit() complete" << std::endl;
	}
}