#include "AEEngine.h"

namespace TileTypes {

	enum TileType
	{
		FLOOR = 0,		// 0
		WALL,			// 1
		EMPTY,			// 2
		//CHEST,		// 3
		//ENEMYN1,		// 4
		//ENEMYN2,		// 5
		TILECOUNT
	};

	struct TileDetail
	{
		int tileID;
		bool traversable, interactable;

		AEGfxTexture * texture;
	};

	extern TileTypes::TileDetail tiledetail[TILECOUNT];

	void InitTileDetails(AEGfxTexture* floor, AEGfxTexture* wall);
}