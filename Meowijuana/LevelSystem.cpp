#include "AEEngine.h"
#include "LevelSystem.hpp"
#include "TileTypes.hpp"
#include <cmath> // floorf


namespace LevelSystem {

	// ok in my defense this sounded rational at the time 
	//TileTypes::TileDetail tiledetail[TileTypes::TILECOUNT];

	int Level::index(int x, int y) const {
		return y * WIDTH + x;
	}

	bool Level::loadLevel(const char* filename) {
		FILE* file;
		fopen_s(&file, filename, "r");

		if (!file) {
			return false;
		}

		tiles.assign(WIDTH * HEIGHT, 0);
		collision.assign(WIDTH * HEIGHT, false);

		for (int y = 0; y < HEIGHT; ++y) {

			for (int x = 0; x < WIDTH; ++x) {
				int id; 
				fscanf_s(file, "%d", &id);

				int i = index(x, y);

				tiles[i] = id;
				collision[i] = (id == TileTypes::WALL);

			}
		}


		fclose(file);
		return true;


	}



	int Level::getWidth() const {
		return WIDTH;
	}

	int Level::getHeight() const {
		return HEIGHT;
	}



	int Level::getTile(int x, int y) {
		if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT) return -1;
		return tiles[index(x, y)];
	}



	bool Level::isBlocked(int x, int y) {
		if (x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT)
			return true;

		return collision[index(x, y)];

	}


	// well this is just no longer my soroor assignment code


	int Level::worldToTileX(float x){

		return (int)(x / 100.0f + WIDTH * 0.5f);

	}

	int Level::worldToTileY(float y){

		return (int)(HEIGHT * 0.5f - y / 100.0f);
	
	}



	int Level::checkBinaryCollision(float x, float y, float w, float h){

		float halfW = w * 0.5f;
		float halfH = h * 0.5f;

		int left = worldToTileX(x - halfW);
		int right = worldToTileX(x + halfW);
		int top = worldToTileY(y + halfH);
		int bottom = worldToTileY(y - halfH);

		int flag = 0;

		for (int ty = top; ty <= bottom; ty++)
		{
			for (int tx = left; tx <= right; tx++)
			{
				if (isBlocked(tx, ty))
				{
					if (tx == left)   flag |= COLLISION_LEFT;
					if (tx == right)  flag |= COLLISION_RIGHT;
					if (ty == top)    flag |= COLLISION_TOP;
					if (ty == bottom) flag |= COLLISION_BOTTOM;
				}
			}
		}

		return flag;
	}





	void Level::draw(AEGfxVertexList* mesh)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			for (int x = 0; x < WIDTH; ++x)
			{
				int tileID = getTile(x, y);
				if (tileID < 0) continue;

				TileTypes::TileDetail& def = TileTypes::tiledetail[tileID];
				if (!def.texture) continue;

				float tileSize = 100.0f;

				float worldX = (x - WIDTH / 2.0f + 0.5f);
				float worldY = (HEIGHT / 2.0f - y - 0.5f);

				AEMtx33 transform;
				AEMtx33Identity(&transform);

				transform.m[0][0] = tileSize;
				transform.m[1][1] = tileSize;

				transform.m[0][2] = worldX * tileSize;
				transform.m[1][2] = worldY * tileSize;

				AEGfxSetTransform(transform.m);
				AEGfxTextureSet(def.texture, 0, 0);
				AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

			}
		}
	}

}