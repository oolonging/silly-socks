#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Graphics.hpp"
#include "Collision.hpp"
#include "Pathfinding.hpp"
#include "UI_Elements/UI_Elements.hpp"
#include "LevelSystem.hpp"
#include "Inventory.hpp"
#include "World.hpp"

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

		// Atack
		float atkCD = 2.0;
		float atkSpd = 0.8;
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
		float getAtkSpd() const { return atkSpd; }
		float getAtkCD() const { return atkCD; }

		AEGfxTexture* getSprite() const { return sprite; }
		
		// Setters
		void setPosition(float posX, float posY);
		void setHp(float health);
		void setSpeed(float spd);
		void setArmor(float arm);
		void setSprite(AEGfxTexture* tex);

		// Attack
		void setAtkSpd(float spd);
		void setAtkCD(float CD);
		bool isAlive() const { return hp > 0; }


		// Health bar management
		void updateHealthBar();
		void drawHealthBar();

		// More attack
		bool canAttack();
		void tickAttackTimer();
		void resetAttackTimer();
		void takeDamage(float rawDamage);

		void setWeapon(Inventory::Weapon* weapon) { equippedWeapon = weapon; }
		Inventory::Weapon* getEquippedWeapon() const { return equippedWeapon; }
		void attack(Entity& target);

		// To set the Position seperately
		void setX(float newX) { x = newX; }
		void setY(float newY) { y = newY; }

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
		bool movingDirections[4];
		Inventory::Item* inventory[9]; // Array of 9 inventory slots
		int selectedInventorySlot; // 0-8

		void handleMovement();
		// Testing Movement ref the grid
		void handleMovement(World::worldGrid& Griddy);

		void handleMovement(LevelSystem::Level& level);


	public:
		// Inventory methods
		Inventory::Item* getInventoryItem(int slot) const;
		void setInventoryItem(int slot, Inventory::Item* item);
		int getSelectedInventorySlot() const;
		void setSelectedInventorySlot(int slot);
		int getInventorySize() const { return 9; }
		void clearInventorySlot(int slot);
		void freeInventory();

		// Getting moving directions
		bool(&getDirections())[4] { return movingDirections; }

		void update();
		void update(World::worldGrid& Griddy);
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

		/*void movement(const Player& player, World::worldGrid& Griddy, float deltaTime);*/

	public:
		void movement(const Player& player, float deltaTime, World::worldGrid& Griddy);

		// Draw that takes player reference for AI
		void draw(const Player& player, World::worldGrid& Griddy, bool pause);

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
		std::vector<std::string> idleLines;
		size_t linenum = 0;
		size_t idlenum = 0;
		bool started = false;
		bool idling = false;
		bool isPaused = false;
		bool conditionTrue = false;

	public:
		// Dialog management
		const std::vector<std::string>& getDialogLines() const;
		void setDialogLines(const std::vector<std::string>& lines);
		void addDialogLine(const std::string& line);

		const std::vector<std::string>& getIdleLines() const;
		void setIdleLines(const std::vector<std::string>& lines);
		void addIdleLine(const std::string& line);

		size_t getLineNum();
		size_t getIdleNum();

		bool getIsPaused();
		bool getIsIdling();
		bool getConditionTrue();

		void setConditionTrue();
		void setIdling(bool value);
		void resumeDialogue(UI_Elements::DialogueBox& dialogueBox);

		void speak(UI_Elements::DialogueBox& dialogueBox);
		void idleSpeak(UI_Elements::DialogueBox& dialogueBox);
		void draw() override;


		// Constructors
		NPC(float posX, float posY, float w, float h, float health, float spd, float arm);
		NPC();

	public:

		enum class DialogueType {
			MAIN,
			IDLE
		};
	};
}

#endif // ENTITY_HPP
