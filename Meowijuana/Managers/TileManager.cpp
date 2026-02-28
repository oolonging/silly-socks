#include "TileManager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

namespace TileManager {
	std::unordered_map<std::string, World::TileObject> tileTypes;

	void registerTileType(const std::string& name, int id, bool traversable, bool interactable, const std::string& texturePath) {
		World::TileObject tileObj;
		tileObj.id = id;
		tileObj.traversable = traversable;
		tileObj.interactable = interactable;

		if (!texturePath.empty()) {
			tileObj.pTex = AEGfxTextureLoad(texturePath.c_str());
		}
		else {
			tileObj.pTex = nullptr;
		}

		tileTypes[name] = tileObj;
		std::cout << "Registered tile type: " << name << " (ID: " << id << ", Traversable: " << traversable << ")" << std::endl;
	}

	World::TileObject* getTileType(const std::string& name) {
		auto it = tileTypes.find(name);
		if (it != tileTypes.end()) {
			return &(it->second);
		}
		std::cerr << "Warning: Tile type '" << name << "' not found!" << std::endl;
		return nullptr;
	}

	bool isValidGridPosition(int gridX, int gridY) {
		return (gridX >= 0 && gridX < World::GRID_WIDTH &&
			gridY >= 0 && gridY < World::GRID_HEIGHT);
	}

	World::Tile& getTileAt(int gridX, int gridY) {
		// Bounds checking
		if (!isValidGridPosition(gridX, gridY)) {
			std::cerr << "Invalid grid position: (" << gridX << ", " << gridY << ")" << std::endl;
			return World::grid[0][0]; // Return default tile
		}
		return World::grid[gridY][gridX];
	}

	World::Tile& getTileFromWorld(float worldX, float worldY) {
		return World::getTileFromWorld(worldX, worldY);
	}

	void setTile(int gridX, int gridY, const std::string& typeName) {
		if (!isValidGridPosition(gridX, gridY)) {
			std::cerr << "Cannot set tile - invalid position: (" << gridX << ", " << gridY << ")" << std::endl;
			return;
		}

		World::TileObject* pTileType = getTileType(typeName);
		if (pTileType) {
			World::grid[gridY][gridX].setTileObject(pTileType);
			std::cout << "Set tile at (" << gridX << ", " << gridY << ") to type '" << typeName << "'" << std::endl;
		}
		else {
			std::cerr << "Tile type not found: " << typeName << std::endl;
		}
	}

	void clearTile(int gridX, int gridY) {
		if (!isValidGridPosition(gridX, gridY)) {
			return;
		}
		World::grid[gridY][gridX].clearTileObject();
	}

	void fillRegion(int startX, int startY, int endX, int endY, const std::string& typeName) {
		World::TileObject* pTileType = getTileType(typeName);
		if (!pTileType) {
			std::cerr << "Tile type not found: " << typeName << std::endl;
			return;
		}

		int tilesSet = 0;
		for (int y = startY; y <= endY; ++y) {
			for (int x = startX; x <= endX; ++x) {
				if (isValidGridPosition(x, y)) {
					World::grid[y][x].setTileObject(pTileType);
					tilesSet++;
				}
			}
		}
		std::cout << "Filled region with '" << typeName << "' - " << tilesSet << " tiles set" << std::endl;
	}

	bool loadTileLayout(const std::string& filepath) {
		std::cout << "Attempting to load tile layout from: " << filepath << std::endl;

		std::ifstream file(filepath, std::ios::in);

		if (!file.is_open()) {
			std::cerr << "Failed to open tile layout file: " << filepath << std::endl;
			return false;
		}

		std::cout << "File opened successfully" << std::endl;

		// Check if file is empty
		file.seekg(0, std::ios::end);
		std::streampos fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		std::cout << "File size: " << fileSize << " bytes" << std::endl;

		if (fileSize == 0) {
			std::cerr << "File is empty!" << std::endl;
			file.close();
			return false;
		}

		std::string line;
		int lineNum = 0;
		int tilesLoaded = 0;

		while (std::getline(file, line)) {
			lineNum++;
			std::cout << "  Raw line " << lineNum << ": '" << line << "' (length: " << line.length() << ")" << std::endl;

			// Remove carriage return if present (Windows line endings)
			if (!line.empty() && line[line.length() - 1] == '\r') {
				line.erase(line.length() - 1);
			}

			// Trim whitespace
			size_t start = line.find_first_not_of(" \t\r\n");
			size_t end = line.find_last_not_of(" \t\r\n");

			if (start == std::string::npos) {
				std::cout << "  Line " << lineNum << ": Empty after trim, skipping" << std::endl;
				continue;
			}

			line = line.substr(start, end - start + 1);
			std::cout << "  Trimmed line " << lineNum << ": '" << line << "'" << std::endl;

			// Parse line
			std::istringstream iss(line);
			int gridX, gridY;
			std::string typeName;

			if (!(iss >> gridX >> gridY >> typeName)) {
				std::cerr << "  Error parsing line " << lineNum << ": '" << line << "'" << std::endl;
				continue;
			}

			std::cout << "  Parsed: gridX=" << gridX << ", gridY=" << gridY << ", type='" << typeName << "'" << std::endl;
			setTile(gridX, gridY, typeName);
			tilesLoaded++;
		}

		file.close();
		std::cout << "File closed. Successfully loaded " << tilesLoaded << " tiles from " << filepath << std::endl;
		return true;
	}

	bool saveTileLayout(const std::string& filepath) {
		std::cout << "Attempting to save tile layout to: " << filepath << std::endl;
		
		std::ofstream file(filepath, std::ios::out | std::ios::trunc);

		if (!file.is_open()) {
			std::cerr << "Failed to create tile layout file: " << filepath << std::endl;
			return false;
		}

		std::cout << "File opened for writing" << std::endl;
		int tilesSaved = 0;

		// Save tiles that have objects
		for (int y = 0; y < World::GRID_HEIGHT; ++y) {
			for (int x = 0; x < World::GRID_WIDTH; ++x) {
				World::Tile& tile = World::grid[y][x];
				if (tile.hasTileObject()) {
					// Find the tile type name by comparing the ID
					World::TileObject* pObj = tile.getTileObject();
					bool found = false;

					for (const auto& pair : tileTypes) {
						// Compare by ID
						if (pair.second.id == pObj->id) {
							std::string line = std::to_string(x) + " " + std::to_string(y) + " " + pair.first + "\n";
							file << line;
							
							// Debug: Print what we're writing
							if (tilesSaved < 5) {
								std::cout << "  Writing: " << line.substr(0, line.length()-1) << std::endl;
							}
							
							tilesSaved++;
							found = true;
							break;
						}
					}

					if (!found) {
						std::cerr << "Warning: Tile at (" << x << ", " << y << ") has unknown tile type (ID: " << pObj->id << ")" << std::endl;
					}
				}
			}
		}

		// Explicit flush before closing
		file.flush();
		file.close();
		
		std::cout << "File closed. Saved " << tilesSaved << " tiles to: " << filepath << std::endl;
		
		// Verify the file was written
		std::ifstream verifyFile(filepath);
		if (verifyFile.is_open()) {
			verifyFile.seekg(0, std::ios::end);
			std::streampos fileSize = verifyFile.tellg();
			std::cout << "Verification: File size is now " << fileSize << " bytes" << std::endl;
			verifyFile.close();
		}
		
		return true;
	}

	void init() {
		std::cout << "Initializing TileManager..." << std::endl;

		// Initialize the world grid first
		World::init();

		// Register default tile types
		registerTileType("empty", 0, true, false, "");  // No texture for empty
		registerTileType("wall", 1, false, false, "Assets/Images/Tiles/wall.png");
		registerTileType("door", 2, false, true, "Assets/Images/Tiles/door.png");
		registerTileType("water", 3, false, false, "Assets/Images/Tiles/water.png");
		registerTileType("grass", 4, true, false, "Assets/Images/Tiles/grass.png");

		std::cout << "TileManager initialized with " << tileTypes.size() << " tile types" << std::endl;
	}

	void draw() {
		World::draw();
	}

	void exit() {
		std::cout << "Cleaning up TileManager..." << std::endl;

		// Unload textures
		for (auto& pair : tileTypes) {
			if (pair.second.pTex) {
				AEGfxTextureUnload(pair.second.pTex);
				pair.second.pTex = nullptr;
			}
		}

		tileTypes.clear();
		World::exit();

		std::cout << "TileManager cleaned up" << std::endl;
	}
}