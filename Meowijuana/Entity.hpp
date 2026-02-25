#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Graphics.hpp"
#include "Collision.hpp"
#include "Pathfinding.hpp"
#include "UI_Elements/UI_Elements.hpp"
#include "LevelSystem.hpp"
#include <vector>
#include <string>
#include "Inventory.hpp"

namespace Entity {

	// Forward declaration
	class Player;

	// Base Entity class
	class Entity {
	protected:
		float x;
		float y;
		float width;
		float height;
		float hp;
		float maxHp;
		float speed;
		float armor;
		AEGfxTexture* sprite;
		UI_Elements::ProgressBar healthBar;
		Inventory::Weapon* equippedWeapon = nullptr;

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
		void setPosition(float posX, float posY);
		void setHp(float health);
		void setSpeed(float spd);
		void setArmor(float arm);
		void setSprite(AEGfxTexture* tex);


		// Health bar management
		void updateHealthBar();
		void drawHealthBar();


		void takeDamage(float rawDamage);

		void setWeapon(Inventory::Weapon* weapon) { equippedWeapon = weapon; }
		Inventory::Weapon* getEquippedWeapon() const { return equippedWeapon; }
		void attack(Entity& target);


		// Virtual destructor for proper cleanup
		virtual ~Entity() = default;

		// Virtual draw - can be overridden
		virtual void draw();

	protected:
		// Constructors for derived classes
		Entity(float posX, float posY, float w, float h, float health, float spd, float arm);
		Entity();
	};

	class Player : public Entity {
	private:
		bool movingDirections[4]; // up, down, left, right

		void handleMovement();
		void handleMovement(LevelSystem::Level& level);

	public:
		void update();
		void update(LevelSystem::Level& level);
		void draw() override;

		// Constructors
		Player(float posX, float posY, float w, float h, float health, float spd, float arm);
		Player();
	};

	class Enemy : public Entity {
	private:
		float fov;
		float deltaX;
		float deltaY;
		
		// Wander behavior
		bool walking;
		float endAtX;
		float endAtY;
		float wait;
		float waited;

		void movement(const Player& player, float deltaTime);

	public:
		// Draw that takes player reference for AI
		void draw(const Player& player);

		// Setters
		void setFov(float newFov);
		float getFov() const;

		// Constructors
		Enemy(float posX, float posY, float w, float h, float health, float spd, float arm);
		Enemy();
	};

	class NPC : public Entity {
	private:
		std::vector<std::string> dialogLines;

	public:
		// Dialog management
		const std::vector<std::string>& getDialogLines() const;
		void setDialogLines(const std::vector<std::string>& lines);
		void addDialogLine(const std::string& line);

		void draw() override;

		// Constructors
		NPC(float posX, float posY, float w, float h, float health, float spd, float arm);
		NPC();
	};
}

#endif // ENTITY_HPP
