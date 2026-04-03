#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Graphics.hpp"
#include "Collision.hpp"
#include "Pathfinding.hpp"
#include "UI_Elements/UI_Elements.hpp"
#include "LevelSystem.hpp"
#include "Inventory.hpp"
#include "World.hpp"
#include "../../Managers/SpriteManager.hpp"

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
		UI_Elements::ProgressBar healthBar;

		// movement vars
		bool isWalking = false;
		int facingDirection = 0; // 0 = LEFT, 1 = RIGHT


		// attack vars
		bool isAttacking = false;
		float attackTimer = 0.0f;
		float attackDuration = .5f;
		bool holdingWeapon;

		// Spritesheet and animations
		AEGfxTexture* sprite = nullptr;
		std::string spriteSheetName;
		SpriteManager::SpriteSheet* spriteSheet = nullptr;
		SpriteManager::Animation* walkAnimation = nullptr;
		SpriteManager::Animation* attackAnimation = nullptr;
		
		
		// Atack
		float atkCD = 2.0f;
		float atkSpd = 0.8f;

		Inventory::Weapon* equippedWeapon = nullptr;

	public:
		// dead check
		bool isDead = false;
		virtual bool isPlayer() const { return false; }

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
		void weaponCheck(bool check) { holdingWeapon = check; };

		void setSprite(AEGfxTexture* tex);
		void setSpritesheet(std::string name, std::string filepath, float sheetWidth, float sheetHeight, float spriteWidth, float spriteheight) {
			this->spriteSheetName = name;
			this->spriteSheet = SpriteManager::loadSpriteSheet(
				name, filepath,
				sheetWidth, sheetHeight,
				spriteWidth, spriteheight
			);
		}

		void setWalkAnimation(std::string name, std::string sheetName, int _x, int _y, int count, float duration, bool loop = true) {
			this->walkAnimation = SpriteManager::createAnimationFromRange(
				name, sheetName,
				_x, _y, count,
				duration, loop
			);

			if (!this->walkAnimation->isValid() || this->walkAnimation == nullptr)
				this->walkAnimation = SpriteManager::getAnimation(name);
		}

		void setAttackAnimation(std::string name, std::string sheetName, int _x, int _y, int count, float duration, bool loop = true) {
			this->attackAnimation = SpriteManager::createAnimationFromRange(
				name, sheetName,
				_x, _y, count,
				duration, loop
			);
		}

		// memory cleanup on a per-entity basis
		void clean(void) {
			if (this->sprite != nullptr) {
				AEGfxTextureUnload(this->sprite);
				this->sprite = nullptr;
			}
		}

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
		int selectedInventorySlot = 0; // 0-8
		int weaponSwungCount;
		void handleMovement();
		// Testing Movement ref the grid
		void handleMovement(World::worldGrid& Griddy);

		void handleMovement(LevelSystem::Level& level);


	public:
		Inventory::Item* inventory[9]{
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr
		}; // Array of 9 inventory slots
		
		// Inventory methods
		Inventory::Item* getInventoryItem(int slot) const;
		void setInventoryItem(int slot, Inventory::Item* item);
		int getSelectedInventorySlot() const;
		void setSelectedInventorySlot(int slot);
		int getInventorySize() const { return 9; }
		int findItem(int itemID);
		void clearInventorySlot(int slot);
		void freeInventory();
		void giveItem(int itemID, int itemCount);
		bool isPlayer() const override { return true; };
		
		int getSwingCount() { return weaponSwungCount; };
		void increaseSwingCount() { weaponSwungCount++; };
		void resetSwing() { weaponSwungCount = 0; };

		// Getting moving directions
		bool(&getDirections())[4] { return movingDirections; }

		void update();
		void update(World::worldGrid& Griddy);
		void update(LevelSystem::Level& level);
		

		void draw() override;

		bool isHoldingWeapon();

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
		void draw(Player& player, World::worldGrid& Griddy, bool pause);

		// Setters
		void setFov(float newFov);
		float getFov() const;
		bool dead() { return isDead; };

		// Constructors
		Enemy(float posX, float posY, float w, float h, float health, float spd, float arm);
		Enemy();
	};

	class NPC : public Entity {
	private:
		std::vector<std::string> dialogLines;
		std::vector<std::string> idleLines;
		const char* charName;
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
		void setCharName(const char* name) { charName = name; };

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

		void restartIdle();
		void restartDialogue();
		bool idleDone();
		bool dialogueDone();

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
