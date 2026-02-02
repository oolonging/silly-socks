#ifndef COLLISION_HPP
#define COLLISION_HPP


namespace Collision {

	bool collidedWith(float enemyX, float enemyY, float playerX, float playerY, float radius, float playerW, float playerH);
	bool collidedWith(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

}



#endif