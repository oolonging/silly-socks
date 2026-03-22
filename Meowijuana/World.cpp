#include "pch.h"
#include "World.hpp"
#include "Entity.hpp"

World::worldGrid grid;

namespace World {

	bool dungeonTracker[4] = { false,false,false, false };
	int checkNum = 0;

	// Meshes stored in the world namespace
	AEGfxVertexList* gridMesh = nullptr;
	AEGfxVertexList* tileMesh = nullptr;

	worldGrid::worldGrid() : gridWidth(0), gridHeight(0), tileSize(0), offsetX(0.0f), offsetY(0.0f), column(0), row(0) {}

	worldGrid::~worldGrid() {
	}

	// Initialising all the map textures
	void worldGrid::initMapTexture()
	{

		std::ifstream file("Assets/Tiles/TileData.txt");
		if (!file.is_open())
		{
			std::cerr << "Failed to open TileData.txt\n";
			return;
		}

		int id;
		std::string name, texturePath;

		while (file >> id >> name >> texturePath)
		{
			tileObject tile;
			tile.objID = id;
			tile.name = name;
			tile.description = texturePath;
			tile.image = AEGfxTextureLoad(texturePath.c_str());
			tileDatabase[id] = tile;
		}

		file.close();
	}


	void worldGrid::initMapTextureSprite(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) { return; }

		int id, col, row;
		std::string name;

		while (file >> id >> name >> col >> row) {
			tileObject tile;
			tile.objID = id;
			tile.name = name;
			tile.sprite = SpriteManager::createSprite("dungeon", col, row);
			tile.useSprite = true;
			tileDatabase[id] = tile;
		}
		file.close();
	}
	

	// Free the assests and stuff
	void worldGrid::unloadMapTexture()
	{
		for (auto& pair : tileDatabase)
		{
			tileObject& tile = pair.second;

			if (tile.image)
			{
				AEGfxTextureUnload(tile.image);
				tile.image = nullptr;
			}
		}

		tileDatabase.clear();
	}

	// Initialises texture box for grid [ only once ]
	void worldGrid::initTextureBox()
	{
		if (tileMesh == nullptr)
		{
			AEGfxMeshStart();

			float half = tileSize / 2.0f;

			// 2 triangles (square centered at origin)
			AEGfxVertexAdd(-half, -half, 0xFFFFFFFF, 0.f, 1.f);
			AEGfxVertexAdd(half, -half, 0xFFFFFFFF, 1.f, 1.f);
			AEGfxVertexAdd(half, half, 0xFFFFFFFF, 1.f, 0.f);

			AEGfxVertexAdd(-half, -half, 0xFFFFFFFF, 0.f, 1.f);
			AEGfxVertexAdd(half, half, 0xFFFFFFFF, 1.f, 0.f);
			AEGfxVertexAdd(-half, half, 0xFFFFFFFF, 0.f, 0.f);

			tileMesh = AEGfxMeshEnd();
		}
	}

	// Initialize the grid with given dimensions
	void worldGrid::initGrid(int width, int height, int size)
	{
		gridWidth = width / size;   // number of columns
		gridHeight = height / size;   // number of rows
		tileSize = size;

		// Calculate offset to center the grid on screen
		offsetX = -AEGfxGetWindowWidth() / 2.0f;
		offsetY = AEGfxGetWindowHeight() / 2.0f;

		tilesID.resize(gridHeight, std::vector<int>(gridWidth, 0));

		float leftX = offsetX;
		float rightX = offsetX + gridWidth * tileSize;

		float topY = offsetY;
		float bottomY = offsetY - gridHeight * tileSize;

		// Clear grid if there was previously called one
		if (gridMesh)
		{
			AEGfxMeshFree(gridMesh);
			gridMesh = nullptr;
		}

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

	// Using a file to read the ID and filling the vector w respective IDs
	void worldGrid::fillGrid(const std::string& filename)
	{
		std::ifstream file(filename);

		if (!file.is_open())
		{
			printf("Failed to open map file\n");
			return;
		}

		int number;

		for (int y = 0; y < gridHeight; y++)
		{
			for (int x = 0; x < gridWidth; x++)
			{
				file >> number;

				tilesID[y][x] = number;

				if (!file)
				{
					printf("There is an issue with the map\n");
					return;
				}
			}
		}

		printf("Map Loaded\n");

		file.close();
	}

	// Testing to print out the binary map to see if it works properly
	void worldGrid::outWorldMap(const std::string& filename)
	{
		std::ofstream file(filename);

		if (!file.is_open())
		{
			printf("Failed to create file\n");
			return;
		}

		for (int y = 0; y < gridHeight; ++y)
		{
			for (int x = 0; x < gridWidth; ++x)
			{
				file << tilesID[y][x];

				if (x < gridWidth - 1)
					file << " ";
			}

			file << "\n";
		}

		file.close();

		printf("Map exported\n");

	}

	// Getting the Index of the tile based on the world coordinates
	std::pair<int, int> worldGrid::getIndex(float cordX, float cordY) const
	{
		int gridX = static_cast<int>(floor((cordX - offsetX) / tileSize));
		int gridY = static_cast<int>(floor((offsetY - cordY) / tileSize));

		// Clamp X
		if (gridX < 0) gridX = 0;
		if (gridX >= gridWidth) gridX = gridWidth - 1;

		// Clamp Y
		if (gridY < 0) gridY = 0;
		if (gridY >= gridHeight) gridY = gridHeight - 1;

		return { gridX, gridY };
	}


	int& worldGrid::pointerToTile(int gridX, int gridY)
	{
		return tilesID[gridY][gridX];
	}

	const int worldGrid::getTileID(int gridX, int gridY) const
	{
		return tilesID[gridY][gridX];
	}

	// Based on the player position and the mouse, highlight the active tile red
	std::pair<int, int> activeTile(float userX, float userY, const World::worldGrid& Griddy)
	{
		int mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		auto playerTile = Griddy.getIndex(userX, userY);

		// Clamp mouse to window bounds BEFORE converting to world coords
		mouseX = max(0, min(mouseX, (int)AEGfxGetWindowWidth()));
		mouseY = max(0, min(mouseY, (int)AEGfxGetWindowHeight()));

		float worldMouseX = mouseX - AEGfxGetWindowWidth() / 2.0f;
		float worldMouseY = AEGfxGetWindowHeight() / 2.0f - mouseY;

		auto mouseTile = Griddy.getIndex(worldMouseX, worldMouseY);
		
		// Clamp to valid grid bounds
		mouseTile.first = max(0, min(mouseTile.first, Griddy.getWidth() - 1));
		mouseTile.second = max(0, min(mouseTile.second, Griddy.getHeight() - 1));

		std::pair<int, int> ActiveTile;

		// Get the Direction of the Curser 
		int dirX = mouseTile.first - playerTile.first;
		int dirY = mouseTile.second - playerTile.second;

		// Positive = 1, Negative = -1, Zero = 0
		// Eg: if X > 0 and Y > 0 = Bottom Left
		// -1, -1 |  -1, +0  | -1, +1
		// +0, -1 | UserTile | +0, +1
		// +1, -1 |  +1, +0  | +1, +1

		// Based 
		int activeX = (dirX > 0) - (dirX < 0);
		int activeY = (dirY > 0) - (dirY < 0);

		ActiveTile = {
			playerTile.first + activeX,
			playerTile.second + activeY
		};

		return ActiveTile;
	}

	// Based on the tile index, get the world coordinates 
	std::pair<float, float> getWorldCoords(std::pair<int, int> tile, const World::worldGrid& Griddy)
	{
		int tileSize = Griddy.getTileSize();

		float X = Griddy.getOffsetX()
			+ tile.first * tileSize
			+ tileSize / 2.0f;

		float Y = Griddy.getOffsetY()
			- tile.second * tileSize
			- tileSize / 2.0f;

		return { X, Y };
	}

	// User interacting w tile using E NEED DO: Change to add entity player as well 
	void interactTile(std::pair<int, int> tile, World::worldGrid& Griddy, UI_Elements::PlayerInventory inven, Entity::Player& user)
	{
		int& ID = Griddy.pointerToTile(tile.first, tile.second);

		switch (ID)
		{

		case EmptyCropTile:
		{
			int slotIndex = inven.getSelectedSlot();
			Inventory::Item* item = user.getInventoryItem(slotIndex);

			if (item == nullptr) break;

			int itemID = item->getID();

			if (useItemOnTile(tile, Griddy, inven, user))
			{
				switch (itemID)
				{
				case Inventory::ItemID::CARROT_SEEDS:
					printf("Planted Carrot! Yay!!\n");
					ID = PlantedCarrotCropTile;
					break;

				case Inventory::ItemID::CHERRY_SEEDS:
					printf("Planted Cherry! Yay!!\n");
					ID = PlantedCherryCropTile;
					break;
				
				case Inventory::ItemID::POTATO_SEEDS:
					printf("Planted Potato! Yay!!\n");
					ID = PlantedPotatoCropTile;
					break;
				}
			}
			break;
		}

		case PlantedCarrotCropTile:
			printf("A Carrot is growing! Yay!!\n");
			break;

		case PlantedCherryCropTile:
			printf("A Cherry is growing! Yay!!\n");
			break;

		case PlantedPotatoCropTile:
			printf("A Potato is growing! Yay!!\n");
			break;

		case CarrotCropTile:
			printf("Collected Carrot! Yay!!\n");
			inven.giveItem(user, Inventory::ItemID::CARROT, 1);
			ID = EmptyCropTile;
			break;

		case CherryCropTile:
			printf("Collected Cherry! Yay!!\n");
			inven.giveItem(user, Inventory::ItemID::CHERRY, 1);
			ID = EmptyCropTile;
			break;

		case PotatoCropTile:
			printf("Collected Potato! Yay!!\n");
			inven.giveItem(user, Inventory::ItemID::POTATO, 1);
			ID = EmptyCropTile;
			break;

		case InteractableObj:
			// Open Chest -> Pop up or something
			break;
		}

	}

	void worldGrid::growPlants(World::worldGrid& Griddy)
	{
		// Basically using global bool array to update based on level clear
		if (dungeonTracker[checkNum])
		{
			for (int row = 0; row < Griddy.getHeight(); row++)
			{
				for (int col = 0; col < Griddy.getWidth(); col++)
				{
					int& tileID = Griddy.pointerToTile(col, row);
					switch (tileID)
					{
					case PlantedCarrotCropTile:
						tileID = CarrotCropTile;
						break;
					case PlantedCherryCropTile:
						tileID = CherryCropTile;
						break;
					case PlantedPotatoCropTile:
						tileID = PotatoCropTile;
						break;
					}

				}
			}
		}
	}

	// Returns true if the interaction succeeded
	bool useItemOnTile(std::pair<int, int> tile, World::worldGrid& Griddy, UI_Elements::PlayerInventory inven, Entity::Player& user)
	{

		int& ID = Griddy.pointerToTile(tile.first, tile.second);

		// Ensure only plant on Empty Crop
		if (ID != EmptyCropTile) return false;


		int slot = inven.getSelectedSlot();
		Inventory::Item* currentItem = user.getInventoryItem(slot);

		if (currentItem == nullptr) return false;
		if (currentItem->getCount() <= 0) return false;

		// Check if item is useable 
		// must be >= 10 and <= 12 because those are the seed numbers -> 
		if (currentItem->getID() >= 10 && currentItem->getID() <= 12)
		{
			currentItem->setCount(currentItem->getCount() - 1);

			if (currentItem->getCount() <= 0)
			{
				if (currentItem->getCount() <= 0)
				{
					printf("Clearing slot %d\n", slot); // does this print?
					user.clearInventorySlot(slot);

					// Verify it's null
					printf("Slot after clear: %p\n", user.getInventoryItem(slot)); // should print 0
				}
			}

			return true;
		}

		return false;
	}

	void World::standOnTile(int& next, Entity::Player user, World::worldGrid& Griddy, int nextlvl)
	{
		// Get the tile the player is currently standing on
		std::pair<int, int> index = Griddy.getIndex(user.getX(), user.getY());
		
		int tileID = Griddy.getTileID(index.first, index.second);

		if (tileID == World::ActivatedTeleporter)
		{
			next = nextlvl; // trigger level transition
		}

	}

	// Function to help replace all of 1 type to another of 1 type
	void worldGrid::replacingID(int oldID, int newID)
	{
		for (int y = 0; y < gridHeight; y++)
		{
			for (int x = 0; x < gridWidth; x++)
			{
				if (tilesID[y][x] == oldID)
				{
					tilesID[y][x] = newID;
				}
			}
		}
	}

	// Draw indicator on all of 1 type of tile
	void drawIndicatorsOnTileType(const World::worldGrid& Griddy, int ID, Animations::Indicator& ind)
	{
		// Update ONCE per frame, outside the loop
		// For bobbing effect
		static float t = 0.0f;
		t += 0.05f;
		if (t > 6.2832f) t = 0.0f;
		ind.offset = (sinf(t) + 1.0f) * 5.0f;

		for (int row = 0; row < Griddy.getHeight(); row++) 
		{
			for (int col = 0; col < Griddy.getWidth(); col++)
			{
				if (Griddy.getTileID(col, row) == ID) 
				{
					std::pair<float, float> tileCord = World::getWorldCoords({ col, row }, Griddy);
					Animations::drawIndicator(tileCord.first, tileCord.second, ind.offset);
				}
			}
		}
	}

	// Drawing and other stuff //
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

	// Drawing each tile based on the ID in the world coords
	void worldGrid::drawTexture(const World::worldGrid& Griddy)
	{
		
		Color::background({ 150, 75, 0, 255 });
		// Set these ONCE before the loop
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		for (int y = 0; y < gridHeight; y++) {
			for (int x = 0; x < gridWidth; x++) {
				int tileID = tilesID[y][x];
				auto it = tileDatabase.find(tileID);
				if (it == tileDatabase.end()) continue;
				if (tileID == Ground) continue;

				const tileObject& def = it->second;
				std::pair<float, float> coords = getWorldCoords({ x, y }, Griddy);

				if (def.useSprite) {
					Color::background({ 234, 165, 108, 255 });
					SpriteManager::drawSpriteFromSheet(def.sprite, coords.first, coords.second, tileSize, tileSize);
					continue;
				}

				if (def.image) {
					AEMtx33 trans;
					AEMtx33Trans(&trans, coords.first, coords.second);
					AEGfxSetTransform(trans.m);
					AEGfxTextureSet(def.image, 0, 0);
					AEGfxMeshDraw(tileMesh, AE_GFX_MDM_TRIANGLES);
				}
			}
		}
	}

	// Draw highlighted tile based on the coords 
	void drawTile(std::pair<int, int> tile, const World::worldGrid& Griddy)
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		int tileSize = Griddy.getTileSize();
		std::pair<float, float> coords = getWorldCoords(tile, Griddy);

		// Filling the square as red for highlight
		Color::fill(255, 0, 0, 50);
		Shapes::square(coords.first, coords.second, tileSize);
	}

	void freeGrid()
	{
		if (gridMesh != nullptr)
		{
			AEGfxMeshFree(gridMesh);
			gridMesh = nullptr;
		}

		if (tileMesh != nullptr)
		{
			AEGfxMeshFree(tileMesh);
			tileMesh = nullptr;
		}
	}
}

