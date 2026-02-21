#include "AEEngine.h"
#include <iostream> // just for debugging
#include "../GameStateManager.hpp"
#include "../Graphics.hpp"
#include "../LevelSystem.hpp"
#include "../Entity.hpp"
#include "../World.hpp"
#include "../TileTypes.hpp"
#include "DungeonScreen.hpp"

extern Entity::Player testPlayer;
static AEGfxVertexList* sTileMesh = nullptr;

// its technically level 1 right
static LevelSystem::Level Level1;


void Dungeon_Load() {

	Level1.loadLevel("Assets/LevelMaps/l1test.txt");

	AEGfxTexture* floorTex = AEGfxTextureLoad("Assets/Tiles/DUNGEON_TILE.png");
	AEGfxTexture* wallTex = AEGfxTextureLoad("Assets/Tiles/DIRT_TILE.png");

	std::cout << floorTex << " " << wallTex << std::endl;

	TileTypes::InitTileDetails(floorTex, wallTex);


	if (!sTileMesh)
	{
		AEGfxMeshStart();

		AEGfxTriAdd(-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

		AEGfxTriAdd(0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f);

		sTileMesh = AEGfxMeshEnd();
	}


	//AEGfxTexture* chestTex = AEGfxTextureLoad("Assets/chest.png");
	//AEGfxTexture* enemyn1Tex = AEGfxTextureLoad("Assets/enemy1.png");
	//AEGfxTexture* enemyn2Tex = AEGfxTextureLoad("Assets/enemy2.png");


}

void Dungeon_Initialize() {
	World::initGrid(Level1.getWidth(), Level1.getHeight(), 50.0f);

	float oriX = testPlayer.getX();
	float oriY = testPlayer.getY();

	testPlayer = Entity::Player(
		oriX, oriY,
		50.0f, 50.0f,
		100.0f, 5.0f, 0.0f
	);

}

void Dungeon_Update() {
	// Nothing to init just a plain background and some text

	testPlayer.update(Level1);

	//AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	//Color::textFill(255, 255, 255);
	//Color::background({ 0, 0, 0, 255 });
	//Text::text("Dungeon screen", 0, 0);
}

void Dungeon_Draw() {

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetColorToMultiply(1, 1, 1, 1);

	
	Level1.draw(sTileMesh);
	testPlayer.draw();


}

void Dungeon_Free() {
	
}

void Dungeon_Unload() {


}
