
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Graphics.hpp"
#include "Collision.hpp"
#include "Pathfinding.hpp"
#include "UI_Elements.hpp"
#include <vector>
#include <string>

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
			AEGfxTexture* sprite = nullptr;
			

		public:
			// health bar
			UI_Elements::ProgressBar healthBar = UI_Elements::ProgressBar(
				0.0f, 0.0f,
				50.0f, 5.0f,
				Entity::hp,
				0.0f,
				Entity::maxHp,
				Shapes::CORNER
			);


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
			void setSprite(AEGfxTexture* tex) { sprite = tex; }

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

				// Just a blue rect for now
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				Color::fill(0, 0, 255);
				Color::stroke(0, 0, 0);
				Shapes::rect(getX(), getY(), getWidth(), getHeight(), Shapes::CENTER);
			}

			// Constructors
			Player(float posX, float posY, float w, float h, float health, float spd, float arm)
				: Entity(posX, posY, w, h, health, spd, arm) {
			}

			Player() : Entity() {
			}
	};

	class Enemy : public Entity {
		private:
			float fov = 500.0f; 

			float deltaX = 0.0f;
			float deltaY = 0.0f;
			
			// wamder
			bool walking = false;
			float endAtX = 0.0f;
			float endAtY = 0.0f;
			float wait = 2.0f;
			float waited = 0.0f;
			
			
		public:

			void movement (const Player& player, float deltaTime) {

				bool playerSpotted = Collision::collidedWith(getX(), getY(), player.getX(), player.getY(), fov, player.getWidth(), player.getHeight());

				if (!playerSpotted) {
					if (!walking) {
						Path::Vect set = Path::randPoint();
						endAtX = getX() + set.x;
						endAtY = getY() + set.y;
						walking = true;
						waited = 0.0f;
					}

					float dirX = endAtX - getX();
					float dirY = endAtY - getY();
					float dist = sqrtf(dirX * dirX + dirY * dirY);


					if (dist > getSpeed()) {
						dirX /= dist;
						dirY /= dist;
						setPosition(getX() + dirX * getSpeed(), getY() + dirY * getSpeed() );
					}

					else {

						waited += deltaTime;
						if (waited >= wait) {
							walking = false;
							waited = 0.0f;
						}

					}
					
	
				}


				if (playerSpotted) {
					float diffX = player.getX() - getX();  
					float diffY = player.getY() - getY();

					// if this doesn't work i will cry (threat)
					float dist = sqrtf((diffX * diffX) + (diffY * diffY));

					if (dist > 1.0f) {
						diffX /= dist;
						diffY /= dist;
					}

					if (dist >= 70) {
						setPosition(getX() + diffX * getSpeed(), getY() + diffY * getSpeed());
					}
					
				}

			


				

			}

			void draw(const Player& player) {
				Enemy::movement(player, (float) AEFrameRateControllerGetFrameTime());

				// Just a red rect for now
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				Color::fill(255, 0, 0);
				Shapes::rect(getX(), getY(), getWidth(), getHeight(), Shapes::CENTER);

				// draw the health bar
				healthBar.draw(getX() - getWidth() / 2.0f, (getY() + getHeight() / 2.0f) + 10.0f, getWidth(), 10.0f);
				healthBar.setValue(getHp());

			}
			// Constructors
			Enemy(float posX, float posY, float w, float h, float health, float spd, float arm)
				: Entity(posX, posY, w, h, health, spd, arm) {
			}
			Enemy() : Entity() {
			}
	};

	class NPC : public Entity {
		private:
			std::vector<std::string> dialogLines;

		public:
			// Getters and Setters for dialogLines
			const std::vector<std::string>& getDialogLines() const { return dialogLines; }
			void setDialogLines(const std::vector<std::string>& lines) { dialogLines = lines; }

			// Add a dialog line
			void addDialogLine(const std::string& line) { dialogLines.push_back(line); }


			void draw(void) {
				// Just a green rect for now
				AEGfxSetRenderMode(AE_GFX_RM_COLOR);
				Color::fill(0, 255, 0);
				Shapes::rect(getX(), getY(), getWidth(), getHeight(), Shapes::CENTER);
			}
			// Constructors
			NPC(float posX, float posY, float w, float h, float health, float spd, float arm)
				: Entity(posX, posY, w, h, health, spd, arm) {
			}
			NPC() : Entity() {
			}
	};
}


#endif // ENTITY_HPP
