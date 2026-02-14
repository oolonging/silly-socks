#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Graphics.hpp"

namespace Collision {

	bool collidedWith(float enemyX, float enemyY, float playerX, float playerY, float radius, float playerW, float playerH);
	bool collidedWith(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2);

	// geometric collisions
	bool rectInRect(Shapes::Quad rect1, Shapes::Quad rect2, Shapes::SHAPE_MODE mode);
	bool rectInCircle(Shapes::Quad rect, Shapes::Circle circle, Shapes::SHAPE_MODE mode);
	bool circleInCircle(Shapes::Circle circle1, Shapes::Circle circle2);
}



#endif