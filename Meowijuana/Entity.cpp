#include <iostream>
#include "AEEngine.h"
#include "Entity.hpp"
#include "Collision.hpp"

namespace Entity {

	// -------------------------------------------------------------------------
	// Entity Base Class Implementation
	// -------------------------------------------------------------------------

	Entity::Entity(float posX, float posY, float w, float h, float health, float spd, float arm)
		: x(posX), y(posY), width(w), height(h), hp(health), maxHp(health), speed(spd), armor(arm), sprite(nullptr),
		  healthBar(0.0f, 0.0f, w, 10.0f, health, 0.0f, health, Shapes::CORNER) {
	}

	Entity::Entity()
		: x(0), y(0), width(50), height(50), hp(100), maxHp(100), speed(1), armor(0), sprite(nullptr),
		  healthBar(0.0f, 0.0f, 50.0f, 10.0f, 100.0f, 0.0f, 100.0f, Shapes::CORNER) {
	}

	void Entity::setPosition(float posX, float posY) {
		x = posX;
		y = posY;
	}

	void Entity::setHp(float health) {
		hp = (health > maxHp) ? maxHp : ((health < 0) ? 0 : health);
		updateHealthBar();
	}

	void Entity::setSpeed(float spd) {
		speed = spd;
	}

	void Entity::setArmor(float arm) {
		armor = arm;
	}

	void Entity::setSprite(AEGfxTexture* tex) {
		sprite = tex;
	}

	void Entity::updateHealthBar() {
		healthBar.setValue(hp);
	}

	void Entity::drawHealthBar() {
		// Update health bar value
		healthBar.setValue(hp);
		
		// Position health bar above entity
		float barX = x - width / 2.0f;
		float barY = y + height / 2.0f + 10.0f;
		
		// ProgressBar::draw() now uses its internal position, 
		// but we need to update position dynamically
		// We'll call the overloaded version that accepts position
		
		// Calculate filled width based on HP percentage
		float fillPercentage = (maxHp > 0) ? (hp / maxHp) : 0.0f;
		float filledWidth = width * fillPercentage;


		// Draw background
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(1.0f);
		Color::fill(100, 100, 100, 255);
		Shapes::rect(barX, barY, width, 10.0f, Shapes::CORNER);

		// Draw foreground (health)
		Color::noStroke();
		if (fillPercentage > 0.5f) {
			Color::fill(0, 255, 0, 255); // Green
		} else if (fillPercentage > 0.25f) {
			Color::fill(255, 255, 0, 255); // Yellow
		} else {
			Color::fill(255, 0, 0, 255); // Red
		}
		Shapes::rect(barX, barY, filledWidth, 10.0f, Shapes::CORNER);
	}

	void Entity::takeDamage(float rawDamage) {
		float effectiveDamage = rawDamage - armor;
		if (effectiveDamage < 0) effectiveDamage = 0;

		hp -= effectiveDamage;
		if (hp < 0) hp = 0;

		updateHealthBar();

	}

	void Entity::attack(Entity& target) {
		if (!equippedWeapon) {
			std::cout << "No weapon\n";
			return;
		}

		std::cout << "Attack called\n";

		float range = equippedWeapon->getRange();
		float dx = target.getX() - x;
		float dy = target.getY() - y;
		float dist = sqrt(dx * dx + dy * dy);

		std::cout << "Distance: " << dist << " | Range: " << range << "\n";

		bool hit = Collision::collidedWith(x, y, target.getX(), target.getY(), range, target.getWidth(), target.getHeight());

		if (!hit)
			return;

		std::cout << "Collision non-issue\n";
		
		equippedWeapon->onAttack(*this, target);

	}

	void Entity::draw() {
		// Default implementation (can be overridden)
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(2.0f);
		Color::fill(128, 128, 128, 255);
		Shapes::rect(x, y, width, height, Shapes::CENTER);
	}

	// -------------------------------------------------------------------------
	// Player Implementation
	// -------------------------------------------------------------------------

	Player::Player(float posX, float posY, float w, float h, float health, float spd, float arm)
		: Entity(posX, posY, w, h, health, spd, arm), movingDirections{false, false, false, false} {
	}

	Player::Player()
		: Entity(), movingDirections{false, false, false, false} {
	}

	void Player::handleMovement() {
		movingDirections[0] = AEInputCheckCurr(AEVK_W); // up
		movingDirections[1] = AEInputCheckCurr(AEVK_S); // down
		movingDirections[2] = AEInputCheckCurr(AEVK_A); // left
		movingDirections[3] = AEInputCheckCurr(AEVK_D); // right

		float deltaX = 0.0f;
		float deltaY = 0.0f;

		if (movingDirections[0]) deltaY += speed;
		if (movingDirections[1]) deltaY -= speed;
		if (movingDirections[2]) deltaX -= speed;
		if (movingDirections[3]) deltaX += speed;

		// Normalize diagonal movement
		if (deltaX != 0.0f && deltaY != 0.0f) {
			float length = sqrtf(deltaX * deltaX + deltaY * deltaY);
			deltaX = (deltaX / length) * speed;
			deltaY = (deltaY / length) * speed;
		}

		setPosition(x + deltaX, y + deltaY);
	}


	// ==================================== additional (for collision)

	void Player::handleMovement(LevelSystem::Level& level) {
		movingDirections[0] = AEInputCheckCurr(AEVK_W); // up
		movingDirections[1] = AEInputCheckCurr(AEVK_S); // down
		movingDirections[2] = AEInputCheckCurr(AEVK_A); // left
		movingDirections[3] = AEInputCheckCurr(AEVK_D); // right

		float deltaX = 0.0f;
		float deltaY = 0.0f;

		if (movingDirections[0]) deltaY += speed;
		if (movingDirections[1]) deltaY -= speed;
		if (movingDirections[2]) deltaX -= speed;
		if (movingDirections[3]) deltaX += speed;

		// Normalize diagonal movement
		if (deltaX != 0.0f && deltaY != 0.0f) {
			float length = sqrtf(deltaX * deltaX + deltaY * deltaY);
			deltaX = (deltaX / length) * speed;
			deltaY = (deltaY / length) * speed;
		}

		// --------------------- Collision stuffs ------------------------

		int colX = level.checkBinaryCollision(x + deltaX, y, width, height);
		if (!(colX & (LevelSystem::Level::COLLISION_LEFT | LevelSystem::Level::COLLISION_RIGHT))) {
			x += deltaX;
		}

		int colY = level.checkBinaryCollision(x, y + deltaY, width, height);
		if (!(colY & (LevelSystem::Level::COLLISION_TOP | LevelSystem::Level::COLLISION_BOTTOM))) {
			y += deltaY;
		}

		setPosition(x, y);

	}
	

	void Player::update() {
		handleMovement();
	}

	void Player::update(LevelSystem::Level& level) {
		handleMovement(level);
	}


	void Player::draw() {
		// this + override made me add a player update im so sorry
		// handleMovement();

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		
		// Apply stroke and fill
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(2.0f);
		Color::fill(0, 0, 255, 255);
		
		Shapes::rect(x, y, width, height, Shapes::CENTER);

		// Draw health bar if HP is less than max
		if (hp < maxHp) {
			drawHealthBar();
		}
	}



	// -------------------------------------------------------------------------
	// Enemy Implementation
	// -------------------------------------------------------------------------

	Enemy::Enemy(float posX, float posY, float w, float h, float health, float spd, float arm)
		: Entity(posX, posY, w, h, health, spd, arm),
		  fov(500.0f), deltaX(0.0f), deltaY(0.0f),
		  walking(false), endAtX(0.0f), endAtY(0.0f), wait(2.0f), waited(0.0f) {
	}

	Enemy::Enemy()
		: Entity(),
		  fov(500.0f), deltaX(0.0f), deltaY(0.0f),
		  walking(false), endAtX(0.0f), endAtY(0.0f), wait(2.0f), waited(0.0f) {
	}

	void Enemy::setFov(float newFov) {
		fov = newFov;
	}

	float Enemy::getFov() const {
		return fov;
	}

	void Enemy::movement(const Player& player, float deltaTime) {
		bool playerSpotted = Collision::collidedWith(
			x, y, 
			player.getX(), player.getY(), 
			fov, 
			player.getWidth(), player.getHeight()
		);

		if (!playerSpotted) {
			// Wander behavior
			if (!walking) {
				Path::Vect set = Path::randPoint();
				endAtX = x + set.x;
				endAtY = y + set.y;
				walking = true;
				waited = 0.0f;
			}

			float dirX = endAtX - x;
			float dirY = endAtY - y;
			float dist = sqrtf(dirX * dirX + dirY * dirY);

			if (dist > speed) {
				dirX /= dist;
				dirY /= dist;
				setPosition(x + dirX * speed, y + dirY * speed);
			}
			else {
				waited += deltaTime;
				if (waited >= wait) {
					walking = false;
					waited = 0.0f;
				}
			}
		}
		else {
			// Chase player
			float diffX = player.getX() - x;
			float diffY = player.getY() - y;
			float dist = sqrtf((diffX * diffX) + (diffY * diffY));

			if (dist > 1.0f) {
				diffX /= dist;
				diffY /= dist;
			}

			// Stop at distance threshold
			const float stopDistance = 70.0f;
			if (dist >= stopDistance) {
				setPosition(x + diffX * speed, y + diffY * speed);
			}
		}
	}

	void Enemy::draw(const Player& player) {
		movement(player, static_cast<float>(AEFrameRateControllerGetFrameTime()));

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		
		// Apply stroke and fill
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(2.0f);
		Color::fill(255, 0, 0, 255);
		
		Shapes::rect(x, y, width, height, Shapes::CENTER);

		// Always draw health bar for enemies
		drawHealthBar();
	}

	// -------------------------------------------------------------------------
	// NPC Implementation
	// -------------------------------------------------------------------------

	NPC::NPC(float posX, float posY, float w, float h, float health, float spd, float arm)
		: Entity(posX, posY, w, h, health, spd, arm) {
	}

	NPC::NPC()
		: Entity() {
	}

	const std::vector<std::string>& NPC::getDialogLines() const {
		return dialogLines;
	}

	void NPC::setDialogLines(const std::vector<std::string>& lines) {
		dialogLines = lines;
	}

	void NPC::addDialogLine(const std::string& line) {
		dialogLines.push_back(line);
	}

	void NPC::draw() {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		
		// Apply stroke and fill
		Color::stroke(0, 0, 0, 255);
		Color::strokeWeight(2.0f);
		Color::fill(0, 255, 0, 255);
		
		Shapes::rect(x, y, width, height, Shapes::CENTER);

		// NPCs typically don't show health bars unless damaged
		if (hp < maxHp) {
			drawHealthBar();
		}
	}
}