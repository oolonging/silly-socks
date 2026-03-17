#include "pch.h"
#include "World.hpp"
#include "Entity.hpp"

World::worldGrid grid;

namespace World {

	// Meshes stored in the world namespace
	AEGfxVertexList* gridMesh = nullptr;
	AEGfxVertexList* tileMesh = nullptr;

	worldGrid::worldGrid() : gridWidth(0), gridHeight(0), tileSize(0), offsetX(0.0f), offsetY(0.0f), column(0), row(0) {}

	worldGrid::~worldGrid() {
	}

	// Initialising all the map textures
	void worldGrid::initMapTexture()
	{
		//// -- Misc. -- //
		//Ground, // normal ground
		//Wall, // walls, obstacle objects
		//InteractableObj, // chest etc.

		//// -- Crops -- //
		//EmptyCropTile,
		//PlantedCropTile,
		//GrownCropTile,
		//DoneCropTile

		tileObject ground;
		ground.objID = 0;
		ground.name = "Ground";
		ground.description = "Assets/Tiles/Ground.png";
		ground.image = AEGfxTextureLoad(ground.description.c_str());

		tileDatabase[0] = ground;

		tileObject wall;
		wall.objID = 1;
		wall.name = "Wall";
		wall.description = "Assets/Tiles/Wall.png";
		wall.image = AEGfxTextureLoad(wall.description.c_str());

		tileDatabase[1] = wall;

		tileObject inter;
		inter.objID = 2;
		inter.name = "Interactable Object";
		inter.description = "Assets/Tiles/Wall.png";
		inter.image = AEGfxTextureLoad(inter.description.c_str());

		tileDatabase[2] = inter;

		tileObject emptyCrop;
		emptyCrop.objID = 3;
		emptyCrop.name = "Empty Crop Land";
		emptyCrop.description = "Assets/Tiles/Empty_Crop.png";
		emptyCrop.image = AEGfxTextureLoad(emptyCrop.description.c_str());

		tileDatabase[3] = emptyCrop;

		tileObject plantedCrop;
		plantedCrop.objID = 4;
		plantedCrop.name = "Planted Crop Land";
		plantedCrop.description = "Assets/Tiles/Planted_Crop.png";
		plantedCrop.image = AEGfxTextureLoad(plantedCrop.description.c_str());

		tileDatabase[4] = plantedCrop;

		tileObject grownCrop;
		grownCrop.objID = 5;
		grownCrop.name = "Grown Crop Land";
		grownCrop.description = "Assets/Tiles/Growing_Crop.png";
		grownCrop.image = AEGfxTextureLoad(grownCrop.description.c_str());

		tileDatabase[5] = grownCrop;

		tileObject doneCrop;
		doneCrop.objID = 6;
		doneCrop.name = "Grown Crop Land";
		doneCrop.description = "Assets/Tiles/Done_Crop.png";
		doneCrop.image = AEGfxTextureLoad(doneCrop.description.c_str());

		tileDatabase[6] = doneCrop;

		tileObject OOB;
		OOB.objID = 7;
		OOB.name = "Out Of Bounds (void)";
		OOB.description = "Assets/Tiles/VOID.png";
		OOB.image = AEGfxTextureLoad(OOB.description.c_str());

		tileDatabase[7] = OOB;

		tileObject Teleporter;
		Teleporter.objID = 8;
		Teleporter.name = "Teleporter";
		Teleporter.description = "Assets/Tiles/WHITE.png";
		Teleporter.image = AEGfxTextureLoad(Teleporter.description.c_str());

		tileDatabase[8] = Teleporter;

		tileObject TeleporterOtherSide;
		TeleporterOtherSide.objID = 9;
		TeleporterOtherSide.name = "TeleporterOtherSide";
		TeleporterOtherSide.description = "Assets/Tiles/WHITE.png";
		TeleporterOtherSide.image = AEGfxTextureLoad(TeleporterOtherSide.description.c_str());

		tileDatabase[9] = TeleporterOtherSide;
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

			// Inventory check 
			if (useItemOnTile(tile, Griddy, inven, user))
			{
				printf("Planted Crops! Yay!!\n");
				ID = PlantedCropTile;
			}

			break;

		case PlantedCropTile:
			// If users try to press E with the crop -> shld give little message saying
			if (!useItemOnTile(tile, Griddy, inven, user))
			{
				printf("Seeds has already been planted!\n");
			}
			break;

		case DoneCropTile:
			printf("Collected Plants! Yay!!\n");
			ID = EmptyCropTile;
			break;

		case InteractableObj:
			// Open Chest -> Pop up or something
			break;
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

	// OKAY SO I AM CHANGING THE ENTITY INSTEAD OF WORLD SO YEAH
	//void collidableNearby(Entity::Entity* entity, World::worldGrid& Griddy)
	//{
	//	std::pair<int, int> cords = Griddy.getIndex(entity->getX(), entity->getY());
	//	int col = cords.first;
	//	int row = cords.second;

	//	if (Griddy.getTileID(col + 1, row) != Wall &&
	//		Griddy.getTileID(col - 1, row) != Wall &&
	//		Griddy.getTileID(col, row + 1) != Wall &&
	//		Griddy.getTileID(col, row - 1) != Wall &&
	//		Griddy.getTileID(col + 1, row + 1) != Wall &&
	//		Griddy.getTileID(col - 1, row - 1) != Wall &&
	//		Griddy.getTileID(col + 1, row - 1) != Wall &&
	//		Griddy.getTileID(col - 1, row + 1) != Wall)
	//		return;

	//	Shapes::Quad player = { entity->getX(), entity->getY(), entity->getWidth(), entity->getHeight() };
	//	bool(&dir)[4] = entity->getDirections();
	//	const int UP = 0;
	//	const int DOWN = 1;
	//	const int LEFT = 2;
	//	const int RIGHT = 3;
	//	bool collisionOccured[2] = {};

	//	if (dir[RIGHT] && Griddy.getTileID(col + 1, row) == Wall)
	//	{
	//		std::pair<float, float> worldCords4Tile = getWorldCoords({ col + 1, row }, Griddy);
	//		Shapes::Quad wall = { worldCords4Tile.first, worldCords4Tile.second, static_cast<float>(Griddy.getTileSize()), static_cast<float>(Griddy.getTileSize()) };
	//		if (collideWithWall(player, wall))
	//			collisionOccured[0] = true;
	//	}
	//	else if (dir[LEFT] && Griddy.getTileID(col - 1, row) == Wall)
	//	{
	//		std::pair<float, float> worldCords4Tile = getWorldCoords({ col - 1, row }, Griddy);
	//		Shapes::Quad wall = { worldCords4Tile.first, worldCords4Tile.second, static_cast<float>(Griddy.getTileSize()), static_cast<float>(Griddy.getTileSize()) };
	//		if (collideWithWall(player, wall))
	//			collisionOccured[0] = true;
	//	}

	//	if (dir[UP] && Griddy.getTileID(col, row - 1) == Wall)
	//	{
	//		std::pair<float, float> worldCords4Tile = getWorldCoords({ col, row - 1 }, Griddy);
	//		Shapes::Quad wall = { worldCords4Tile.first, worldCords4Tile.second, static_cast<float>(Griddy.getTileSize()), static_cast<float>(Griddy.getTileSize()) };
	//		if (collideWithWall(player, wall))
	//			collisionOccured[1] = true;
	//	}
	//	else if (dir[DOWN] && Griddy.getTileID(col, row + 1) == Wall)
	//	{
	//		std::pair<float, float> worldCords4Tile = getWorldCoords({ col, row + 1 }, Griddy);
	//		Shapes::Quad wall = { worldCords4Tile.first, worldCords4Tile.second, static_cast<float>(Griddy.getTileSize()), static_cast<float>(Griddy.getTileSize()) };
	//		if (collideWithWall(player, wall))
	//			collisionOccured[1] = true;
	//	}

	//	if (collisionOccured[0] || collisionOccured[1])
	//		snapEntity(entity, Griddy, collisionOccured, dir);
	//}


	//bool collideWithWall(Shapes::Quad user, Shapes::Quad wall)
	//{
	//	return (Collision::rectInRect(user, wall, Shapes::CORNER));
	//}

	//void snapEntity(Entity::Entity* entity, World::worldGrid& Griddy, bool* collisionCheck, bool* dir)
	//{
	//	std::pair<int, int> index = Griddy.getIndex(entity->getX(), entity->getY());
	//	int col = index.first;
	//	int row = index.second;
	//	float tileSize = static_cast<float>(Griddy.getTileSize());
	//	const int UP = 0;
	//	const int DOWN = 1;
	//	const int LEFT = 2;
	//	const int RIGHT = 3;

	//	// Vertical Check
	//	if (collisionCheck[0])
	//	{
	//		if (dir[RIGHT])
	//		{
	//			std::pair<float, float> wallCoords = getWorldCoords({ col + 1, row }, Griddy);
	//			entity->setX(wallCoords.first - entity->getWidth());
	//		}
	//		else if (dir[LEFT])
	//		{
	//			std::pair<float, float> wallCoords = getWorldCoords({ col - 1, row }, Griddy);
	//			entity->setX(wallCoords.first + tileSize);
	//		}
	//	}

	//	// Horizontal Check
	//	if (collisionCheck[1])
	//	{
	//		if (dir[DOWN])
	//		{
	//			std::pair<float, float> wallCoords = getWorldCoords({ col, row + 1 }, Griddy);
	//			entity->setY(wallCoords.second - entity->getHeight());
	//		}
	//		else if (dir[UP])
	//		{
	//			std::pair<float, float> wallCoords = getWorldCoords({ col, row - 1 }, Griddy);
	//			entity->setY(wallCoords.second + tileSize);
	//		}
	//	}
	//}

	void World::standOnTile(int& next, Entity::Player user, World::worldGrid& Griddy, int nextlvl)
	{
		// Get the tile the player is currently standing on
		std::pair<int, int> index = Griddy.getIndex(user.getX(), user.getY());
		
		int tileID = Griddy.getTileID(index.first, index.second);

		if (tileID == World::Teleporter)
		{
			next = nextlvl; // trigger level transition
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
		static bool backgroundSet = false;

		if (!backgroundSet) {
			Color::background({ 150, 75, 0, 255 });
			backgroundSet = true;
		}

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		for (int y = 0; y < gridHeight; y++)
		{
			for (int x = 0; x < gridWidth; x++)
			{
				int tileID = tilesID[y][x];

				auto it = tileDatabase.find(tileID);
				if (it == tileDatabase.end())
					continue;

				if (tileID == Ground) continue; // Dun draw ground
				const tileObject& def = it->second;

				if (!def.image)
					continue;

				std::pair<float, float> coords = getWorldCoords({ x, y }, Griddy);

				AEMtx33 transform;
				AEMtx33 trans;

				AEMtx33Trans(&trans, coords.first, coords.second);
				transform = trans;

				AEGfxSetTransform(transform.m);

				AEGfxTextureSet(def.image, 0, 0);

				AEGfxMeshDraw(tileMesh, AE_GFX_MDM_TRIANGLES);
			}
		}
	}

	// Draw highlighted tile based on the coords 
	void drawTile(std::pair<int, int> tile, const World::worldGrid& Griddy)
	{
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

