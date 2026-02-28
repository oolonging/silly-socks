#include "Tiles.hpp"


namespace World {
	// some sample tile objects
	TileObject wallTile = {
		1, // ID
		false, //Traversable
		false, // Interactable
		nullptr // set in the init function
	};

	// Tile class implementation
	Tile::Tile() : worldX(0.0f), worldY(0.0f), gridX(0), gridY(0) {}

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
		bool graphicalDraw = this->pTileObject != nullptr && this->pTileObject->pTex != nullptr;

		if (graphicalDraw) {
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			Graphics::image(this->worldX, this->worldY, GRID_SIZE, GRID_SIZE, this->getTileObject()->pTex);
		}
		else {
			AEGfxSetRenderMode(AE_GFX_RM_COLOR);
			Shapes::rect(this->worldX, this->worldY, GRID_SIZE, GRID_SIZE);
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

		return grid[row][col];
	}

	void init(void) {
		// Load the image into wall
		wallTile.pTex = AEGfxTextureLoad("Assets/Images/Tiles/wall.png");

		float tileWidth = static_cast<float>(GRID_SIZE);
		float tileHeight = static_cast<float>(GRID_SIZE);

		float startX = -AEGfxGetWindowWidth() * 0.5f;
		float startY = AEGfxGetWindowHeight() * 0.5f;

		for (int row = 0; row < GRID_HEIGHT; ++row) {
			for (int col = 0; col < GRID_WIDTH; ++col) {
				float worldX = startX + col * tileWidth;
				float worldY = startY - row * tileHeight;

				grid[row][col].setGridPosition(col, row);
				grid[row][col].setWorldPosition(worldX, worldY);
			}
		}
	}

	void draw(void) {
		for (int row = 0; row < GRID_HEIGHT; ++row) {
			for (int col = 0; col < GRID_WIDTH; ++col) {
				Tile& tile = grid[row][col];

				if (mouseOnTile(tile)) {
					if (AEInputCheckTriggered(AEVK_LBUTTON)) {
						tile.setTileObject(&wallTile);
					}
					if (AEInputCheckTriggered(AEVK_RBUTTON)) {
						tile.clearTileObject();
					}
				}

				tile.draw();
			}
		}
	}

	void exit(void) {
		AEGfxTextureUnload(wallTile.pTex);
		wallTile.pTex = nullptr;
	}
}