#include "AEEngine.h"
#include <cmath>

#include "Graphics.hpp"
#include "Collision.hpp"

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

	// geometric collisions
	bool rectInRect(Shapes::Quad rect1, Shapes::Quad rect2, Shapes::SHAPE_MODE mode = Shapes::CORNER) {
		bool xOverlap{};
		bool yOverlap{};

		if (mode == Shapes::CORNER) {
			xOverlap = ((rect1.position.x < (rect2.position.x + rect2.width)) && ((rect1.position.x + rect1.width) > rect2.position.x));
			yOverlap = ((rect1.position.y < (rect2.position.y + rect2.height)) && ((rect1.position.y + rect1.height) > rect2.position.y));
		}
		else if (mode == Shapes::CENTER) {
			xOverlap = ((rect1.position.x - rect1.width / 2 < (rect2.position.x + rect2.width / 2)) && ((rect1.position.x + rect1.width / 2) > rect2.position.x - rect2.width / 2));
			yOverlap = ((rect1.position.y - rect1.height / 2 < (rect2.position.y + rect2.height / 2)) && ((rect1.position.y + rect1.height / 2) > rect2.position.y - rect2.height / 2));

		}


		return (xOverlap && yOverlap);
	}

	bool rectInCircle(Shapes::Quad rect, Shapes::Circle circle, Shapes::SHAPE_MODE mode = Shapes::CORNER) {
		float rectX{};
		float rectY{};
		float rectW{};
		float rectH{};
		if (mode == Shapes::CORNER) {
			rectX = rect.position.x;
			rectY = rect.position.y;
			rectW = rect.width;
			rectH = rect.height;
		}
		else if (mode == Shapes::CENTER) {
			rectX = rect.position.x - rect.width / 2;
			rectY = rect.position.y - rect.height / 2;
			rectW = rect.width;
			rectH = rect.height;
		}
		float closestX = circle.position.x;
		float closestY = circle.position.y;
		if (closestX < rectX) closestX = rectX;
		else if (closestX > (rectX + rectW)) closestX = rectX + rectW;
		if (closestY < rectY) closestY = rectY;
		else if (closestY > (rectY + rectH)) closestY = rectY + rectH;
		float distX = circle.position.x - closestX;
		float distY = circle.position.y - closestY;
		float distance = sqrt((distX * distX) + (distY * distY));
		return (distance <= circle.radius);
	}

	bool circleInCircle(Shapes::Circle circle1) {
		// double check the math here, might not work for differently sized ellipses
		float distance = sqrt((circle1.position.x - circle1.position.x) * (circle1.position.x - circle1.position.x) +
			(circle1.position.y - circle1.position.y) * (circle1.position.y - circle1.position.y));

		return (distance <= (circle1.radius + circle1.radius));
	}
}