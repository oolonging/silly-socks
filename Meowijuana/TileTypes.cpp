#include "TileTypes.hpp"
#include "AEEngine.h"


AEGfxTexture* floorTex;
AEGfxTexture* wallTex;
AEGfxTexture* emptyTex;

namespace TileTypes{

     TileDetail tiledetail[TILECOUNT];

    void InitTileDetails(AEGfxTexture* floor, AEGfxTexture* wall)
    {
        tiledetail[FLOOR] = { FLOOR, true, false, floor };
        tiledetail[WALL] = { WALL, false, false, wall };
        tiledetail[EMPTY] = { EMPTY, true, false, nullptr };

        //tiledetail[TileTypes::CHEST] = { false, true,  chestTex };
        //tiledetail[TileTypes::ENEMYN1] = { true,  false, enemyn1Tex };
        //tiledetail[TileTypes::ENEMYN2] = { true,  false, enemyn2Tex };
    }
}
