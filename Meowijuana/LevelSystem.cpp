#include "AEEngine.h"
#include "LevelSystem.hpp"
#include "TileTypes.hpp"
#include <vector>


namespace LevelSystem {

	static int width = 16;
	static int height = 9;

	static std::vector<int> tiles (144);
	static std::vector<bool> collision (144);

	// ok in my defense this sounded rational at the time 
	//TileTypes::TileDetail tiledetail[TileTypes::TILECOUNT];

	static int index(int x, int y) {
		return y * width + x;
	}

	bool LevelSystem::loadLevel(const char* filename) {
		FILE* file;
		fopen_s(&file, filename, "r");

		if (!file) {
			return false;
		}

		for (int y = 0; y < height; ++y) {

			for (int x = 0; x < width; ++x) {
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


	void freeLevel() {

		tiles.clear();
		collision.clear();

	}


	int LevelSystem::getWidth() {
		return width;
	}

	int LevelSystem::getHeight() {
		return height;
	}



	int getTile(int x, int y) {
		if (x < 0 || y < 0 || x >= width || y >= height) return -1;
		return tiles[index(x, y)];
	}



	bool isBlocked(int x, int y) {
		if (x < 0 || y < 0 || x >= width || y >= height)
			return false;

		return collision[index(x, y)];

	}

	// yes this is my soroor assignment code
	// no, don't ask

	int checkBinaryCollision(float posX, float posY, float scaleX, float scaleY) {
		// to store which sides are colliding
		int flag = 0;

		// hotspot coords
		float x1{ 0.0f }, x2{ 0.0f }, y1{ 0.0f }, y2{ 0.0f };

		// left: hotspots 1/4 above and below center
		x1 = posX - scaleX / 2.0f;		// To reach the left side
		y1 = posY + scaleY / 4.0f;		// To go up 1/4 of the height


		x2 = posX - scaleX / 2.0f;		// To reach the left side
		y2 = posY - scaleY / 4.0f;		// To go down 1/4 of the height


		// if touches, mark left collision
		if (isBlocked(int(x1), int(y1)) || isBlocked(int(x2), int(y2))) {
			flag |= COLLISION_LEFT;
		}



		// same thing but right side
		x1 = posX + scaleX / 2.0f;		// right edge
		y1 = posY + scaleY / 4.0f;		// topside


		x2 = posX + scaleX / 2.0f;		// right edge
		y2 = posY - scaleY / 4.0f;		// botside


		// if touches, mark right collision
		if (isBlocked(int(x1), int(y1)) || isBlocked(int(x2), int(y2))) {
			flag |= COLLISION_RIGHT;
		}




		// top side: 1/4 right and left of center
		x1 = posX + scaleX / 4.0f;		// right hotspot
		y1 = posY + scaleY / 2.0f;		// top edge


		x2 = posX - scaleX / 4.0f;		// left hotspot
		y2 = posY + scaleY / 2.0f;		// top edge


		// if touches, mark top collision
		if (isBlocked(int(x1), int(y1)) || isBlocked(int(x2), int(y2))) {
			flag |= COLLISION_TOP;
		}



		// bottom side
		x1 = posX + scaleX / 4.0f;		// right hotspot
		y1 = posY - scaleY / 2.0f;		// bottom edge


		x2 = posX - scaleX / 4.0f;		// left hotspot
		y2 = posY - scaleY / 2.0f;		// bottom edge


		// if touches, mark bottom collision
		if (isBlocked(int(x1), int(y1)) || isBlocked(int(x2), int(y2))) {
			flag |= COLLISION_BOTTOM;
		}



		return flag;
	}



	// yeah nah im gonna give up and go eat for now

	void draw(AEGfxVertexList* mesh)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				int tileID = getTile(x, y);
				if (tileID < 0) continue;

				TileTypes::TileDetail& def = TileTypes::tiledetail[tileID];
				if (!def.texture) continue;

				float tileSize = 50.0f;

				float worldX = (x - width / 2.0f + 0.5f);
				float worldY = (height / 2.0f - y - 0.5f);

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