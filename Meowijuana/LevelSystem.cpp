#include "AEEngine.h"
#include "LevelSystem.hpp"
#include "TileTypes.hpp"


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


	// yes this is my soroor assignment code (tweaked)

	int Level::checkBinaryCollision(float posX, float posY, float scaleX, float scaleY) {
		float tileSize = 50.0f;
		int tileX1{}, tileY1{}, tileX2{}, tileY2{};

		// to store which sides are colliding
		int flag = 0;

		// hotspot coords
		float x1{ 0.0f }, x2{ 0.0f }, y1{ 0.0f }, y2{ 0.0f };

		// left: hotspots 1/4 above and below center
		x1 = posX - scaleX / 2.0f;		// To reach the left side
		y1 = posY + scaleY / 4.0f;		// To go up 1/4 of the height


		x2 = posX - scaleX / 2.0f;		// To reach the left side
		y2 = posY - scaleY / 4.0f;		// To go down 1/4 of the height


		// conversions 
		tileX1 = int((x1 / tileSize) + WIDTH / 2.0f);
		tileY1 = int((HEIGHT / 2.0f) - (y1 / tileSize));

		tileX2 = int((x2 / tileSize) + WIDTH / 2.0f);
		tileY2 = int((HEIGHT / 2.0f) - (y2 / tileSize));
		

		// if touches, mark left collision
		if (isBlocked(int(tileX1), int(tileY1)) || isBlocked(int(tileX2), int(tileY2))) {
			flag |= COLLISION_LEFT;
		}



		// same thing but right side
		x1 = posX + scaleX / 2.0f;		// right edge
		y1 = posY + scaleY / 4.0f;		// topside


		x2 = posX + scaleX / 2.0f;		// right edge
		y2 = posY - scaleY / 4.0f;		// botside



		// conversions 
		tileX1 = int((x1 / tileSize) + WIDTH / 2.0f);
		tileY1 = int((HEIGHT / 2.0f) - (y1 / tileSize));

		tileX2 = int((x2 / tileSize) + WIDTH / 2.0f);
		tileY2 = int((HEIGHT / 2.0f) - (y2 / tileSize));


		// if touches, mark left collision
		if (isBlocked(int(tileX1), int(tileY1)) || isBlocked(int(tileX2), int(tileY2))) {
			flag |= COLLISION_RIGHT;
		}




		// top side: 1/4 right and left of center
		x1 = posX + scaleX / 4.0f;		// right hotspot
		y1 = posY + scaleY / 2.0f;		// top edge


		x2 = posX - scaleX / 4.0f;		// left hotspot
		y2 = posY + scaleY / 2.0f;		// top edge



		// conversions 
		tileX1 = int((x1 / tileSize) + WIDTH / 2.0f);
		tileY1 = int((HEIGHT / 2.0f) - (y1 / tileSize));

		tileX2 = int((x2 / tileSize) + WIDTH / 2.0f);
		tileY2 = int((HEIGHT / 2.0f) - (y2 / tileSize));


		// if touches, mark left collision
		if (isBlocked(int(tileX1), int(tileY1)) || isBlocked(int(tileX2), int(tileY2))) {
			flag |= COLLISION_TOP;
		}



		// bottom side
		x1 = posX + scaleX / 4.0f;		// right hotspot
		y1 = posY - scaleY / 2.0f;		// bottom edge


		x2 = posX - scaleX / 4.0f;		// left hotspot
		y2 = posY - scaleY / 2.0f;		// bottom edge



		// conversions 
		tileX1 = int((x1 / tileSize) + WIDTH / 2.0f);
		tileY1 = int((HEIGHT / 2.0f) - (y1 / tileSize));

		tileX2 = int((x2 / tileSize) + WIDTH / 2.0f);
		tileY2 = int((HEIGHT / 2.0f) - (y2 / tileSize));


		// if touches, mark left collision
		if (isBlocked(int(tileX1), int(tileY1)) || isBlocked(int(tileX2), int(tileY2))) {
			flag |= COLLISION_BOTTOM;
		}



		return flag;
	}



	// yeah nah im gonna give up and go eat for now

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

				float tileSize = 50.0f;

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