#ifndef LEVELSYSTEM_HPP
#define LEVELSYSTEM_HPP



namespace LevelSystem {


	static int BINARY_MAP_WIDTH = 16;
	static int BINARY_MAP_HEIGHT = 9;
	static int** MapData;



	const int	COLLISION_LEFT = 0x00000001;	//0001
	const int	COLLISION_RIGHT = 0x00000002;	//0010
	const int	COLLISION_TOP = 0x00000004;	//0100
	const int	COLLISION_BOTTOM = 0x00000008;	//1000



	bool loadLevel(const char* filename);
	void freeLevel();

	int getTile(int x, int y);

	int getWidth();
	int getHeight();


	//int getTile(int x, int y);
	bool isBlocked(int x, int y);
	int checkBinaryCollision(float posX, float posY, float scaleX, float scaleY);

	void draw(AEGfxVertexList* mesh);

}



#endif