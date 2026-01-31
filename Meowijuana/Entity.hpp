
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Graphics.hpp"

namespace Entity {
	// Entities:: Player, Enemy, NPC, etc.

	class Entity {
		private:
			float x;
			float y;
			float width;
			float height;
			float hp;
			float maxHp;
			float speed;
			float armor;

		public:
			// Getters
			float getX() const { return x; }
			float getY() const { return y; }
			float getWidth() const { return width; }
			float getHeight() const { return height; }
			float getHp() const { return hp; }
			float getMaxHp() const { return maxHp; }
			float getSpeed() const { return speed; }
			float getArmor() const { return armor; }
			// Setters
			void setPosition(float posX, float posY) { x = posX; y = posY; }
			void setHp(float health) { hp = (health > maxHp) ? maxHp : health; }
			void setSpeed(float spd) { speed = spd; }
			void setArmor(float arm) { armor = arm; }
			// Other methods can be added as needed

			// Constructors
			Entity(float posX, float posY, float w, float h, float health, float spd, float arm)
				: x(posX), y(posY), width(w), height(h), hp(health), maxHp(health), speed(spd), armor(arm) {
			}

			Entity() : x(0), y(0), width(50), height(50), hp(100), maxHp(100), speed(1), armor(0) {
			}
	};

	class Player : public Entity {
		private:
			// movement directions;
			bool movingDirections[4] = { false, false, false, false }; // up, down, left, right

		public:
			void handleMovement(void) {
				movingDirections[0] = AEInputCheckCurr(AEVK_W); // up
				movingDirections[1] = AEInputCheckCurr(AEVK_S); // down
				movingDirections[2] = AEInputCheckCurr(AEVK_A); // left
				movingDirections[3] = AEInputCheckCurr(AEVK_D); // right

				float deltaX = 0.0f;
				float deltaY = 0.0f;
				if (movingDirections[0]) deltaY += getSpeed(); // up
				if (movingDirections[1]) deltaY -= getSpeed(); // down
				if (movingDirections[2]) deltaX -= getSpeed(); // left
				if (movingDirections[3]) deltaX += getSpeed(); // right

				setPosition(getX() + deltaX, getY() + deltaY);
			}

			void draw(void) {
				// movement
				Player::handleMovement();

				// Just a red rect for now
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				Color::fill(255, 0, 0);
				Shapes::rect(getX(), getY(), getWidth(), getHeight(), Shapes::CORNER);
			}

			// Constructors
			Player(float posX, float posY, float w, float h, float health, float spd, float arm)
				: Entity(posX, posY, w, h, health, spd, arm) {
			}

			Player() : Entity() {
			}
	};
}


#endif // ENTITY_HPP
