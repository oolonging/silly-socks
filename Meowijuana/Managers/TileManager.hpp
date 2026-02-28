#ifndef TILEMANAGER_HPP
#define TILEMANAGER_HPP

#include <unordered_map>
#include <string>
#include <vector>
#include "../Tiles.hpp"
#include "AEEngine.h"

namespace TileManager {
	// TileObject registry - store different tile types
	extern std::unordered_map<std::string, World::TileObject> tileTypes;

	// Register a new tile type
	void registerTileType(const std::string& name, int id, bool traversable, bool interactable, const std::string& texturePath = "");

	// Get a registered tile type by name
	World::TileObject* getTileType(const std::string& name);

	// Set a tile at grid coordinates to a specific type
	void setTile(int gridX, int gridY, const std::string& typeName);

	// Clear a tile at grid coordinates
	void clearTile(int gridX, int gridY);

	// Get tile at grid coordinates
	World::Tile& getTileAt(int gridX, int gridY);

	// Get tile from world coordinates
	World::Tile& getTileFromWorld(float worldX, float worldY);

	// Check if coordinates are within grid bounds
	bool isValidGridPosition(int gridX, int gridY);

	// Load tile layout from file (JSON or custom format)
	bool loadTileLayout(const std::string& filepath);

	// Save tile layout to file
	bool saveTileLayout(const std::string& filepath);

	// Fill region with a tile type
	void fillRegion(int startX, int startY, int endX, int endY, const std::string& typeName);

	// Initialize grid
	void init();

	// Draw all tiles
	void draw();

	// Cleanup
	void exit();
}

#endif // TILEMANAGER_HPP
