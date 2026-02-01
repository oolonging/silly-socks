#include "AEEngine.h"
#include <cmath>

// okay its a little messy rn but i'll clean it up eventually... i think
namespace Collision {

	// circle rectangle intersection
	bool collidedWith(float enemyX, float enemyY, float playerX, float playerY, float radius, float playerW, float playerH) {
		float left = playerX - playerW * 0.5f;
		float right = playerX + playerW * 0.5f;
		float up = playerY - playerH * 0.5f;
		float down = playerY - playerH * 0.5f;

		float closestX = enemyX;
		float closestY = enemyY;
		if (closestX < left) closestX = left;
		else if (closestX > right) closestX = right;
		if (closestY < up) closestY = up;
		else if (closestY > down) closestY = down;

		float distX = enemyX - closestX;
		float distY = enemyY - closestY;
		float distance = sqrt((distX * distX) + (distY * distY));

		return (distance <= radius);

	}

	// circle circle intersection
	bool collidedWith(float c1_x, float c1_y, float r1, float c2_x, float c2_y, float r2) {
		float distance = sqrt((c2_x - c1_x) * (c2_x - c1_x) + (c2_y - c1_y) * (c2_y - c1_y));
		return distance <= (r1 + r2) ? 1 : 0;

	}

}