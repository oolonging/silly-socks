#ifndef TILES_HPP
#define TILES_HPP

#include "AEEngine.h"
#include "InputManager.hpp"
#include "Graphics.hpp"
#include <vector>

namespace World {
	// Forward declarations
	struct TileObject;
	class Tile;

	// Tile Object struct
	typedef struct TileObject {
		int id;
		bool traversable;
		bool interactable;
		AEGfxTexture* pTex;
	} TileObject;

	// Tile class
	class Tile {
		private:
			float worldX;
			float worldY;
			float gridX;
			float gridY;

			TileObject* pTileObject;


		public:

			// testing
			Color::Color tileColor = Color::createColorRGB(0.0f, 0.0f, 0.0f);
			
			// Getters
			float getWorldX() const { return this->worldX; }
			float getWorldY() const { return this->worldY; }
			int getGridX() const { return this->gridX; }
			int getGridY() const { return this->gridY; }
			TileObject* getTileObject() const { return this->pTileObject; }
			
			// checks to see if pointing to valid tile object
			bool hasTileObject(void) { return (this->pTileObject != nullptr); }

			// Clears tile object
			void clearTileObject(void) {
				this->pTileObject = nullptr;
			}

			// Setters
			void setWorldPosition(float x, float y);
			void setGridPosition(int x, int y);
			void setTileObject(TileObject* pTileObject);

			// draw function
			void draw(void);

			// Ctor
			Tile();
	};

	// Grid stuff
	constexpr int GRID_SIZE = 50;
	constexpr int GRID_WIDTH = 32;
	constexpr int GRID_HEIGHT = 24;
	
	static Tile grid[GRID_HEIGHT][GRID_WIDTH];
	
	// see if the mouse is over a tile
	bool mouseOnTile(Tile& const tile);
	// get a tile from world coordinates
	Tile& getTileFromWorld(float x, float y);

	void init(void);
	void draw(void);
	void exit(void);
}

#endif // TILES_HPP
