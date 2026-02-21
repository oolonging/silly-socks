#ifndef LEVELSYSTEM_HPP
#define LEVELSYSTEM_HPP

#include <vector>

namespace LevelSystem {

	class Level {

	private:
		static constexpr int WIDTH = 16;
		static constexpr int HEIGHT = 9;

		std::vector<int> tiles;
		std::vector<bool> collision;

		int index(int x, int y) const;

	public:
		static constexpr int	COLLISION_LEFT = 0x00000001;	//0001
		static constexpr int	COLLISION_RIGHT = 0x00000002;	//0010
		static constexpr int	COLLISION_TOP = 0x00000004;		//0100
		static constexpr int	COLLISION_BOTTOM = 0x00000008;	//1000

		bool loadLevel(const char* filename);

		int getTile(int x, int y);

		int getWidth() const;
		int getHeight() const;

		int worldToTileX(float x);

		int worldToTileY(float x);

		bool isBlocked(int x, int y);
		int checkBinaryCollision(float posX, float posY, float scaleX, float scaleY);

		void draw(AEGfxVertexList* mesh);
	};

}



#endif