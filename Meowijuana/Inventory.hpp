#ifndef INVENTORY_HPP
#define INVENTORY_HPP

namespace Entity {
	class Entity;
}

namespace Inventory {
	// Item hierarchy: Item -> Weapon (Melee/Ranged/Placeable/Defensive)
	//                 Item -> Consumable

	enum Rarity {
		Common,
		Uncommon,
		Rare,
		Epic,
		Legendary
	};

	enum ConsumableType {
		HealthRestore,    // Instant health restoration (Strawberry)
		SpeedBoost,       // Temporary speed increase (Chilli)
		StrengthBuff      // Temporary damage increase (Spinach)
	};

	// Forward declarations
	class Item;
	class MeleeWeapon;
	class RangedWeapon;
	class PlaceableWeapon;
	class DefensiveWeapon;
	class Consumable;

	// Item IDs for easy reference
	namespace ItemID {
		const int CARROT_SWORD = 1;
		const int POTATO_GRENADE = 2;
		const int CHERRY_BOMB = 3;
		const int WALNUT_SHIELD = 4;
		const int STRAWBERRY = 5;
		const int CHILLI = 6;
		const int SPINACH = 7;
		const int GOLDEN_CARROT_SWORD = 8; // Merged version
		const int WOODEN_SWORD = 9; // for the weaker enemies int he dungeon
		
		// SEEDS
		const int CARROT_SEEDS = 10;
		const int CHERRY_SEEDS = 11;
		const int POTATO_SEEDS = 12;

		// CROP
		const int CARROT = 13;
		const int CHERRY = 14;
		const int POTATO = 15;

		// Inventory item version of weapon
		const int CARROT_SWORD_INV = 16;
	}

	class Item {
	private:
		int id;
		std::string name;
		std::string description;
		float value; // for selling or buying
		AEGfxTexture* icon; // Icon texture for inventory display
		int count;

	public:
		// Getters
		int getID() const { return this->id; }
		std::string getName() const { return this->name; }
		std::string getDescription() const { return this->description; }
		float getValue() const { return this->value; }
		AEGfxTexture* getIcon() const { return this->icon; }
		int getCount() const { return count; }

		// Setters
		void setID(int id) { this->id = id; }
		void setName(const std::string& _name) { this->name = _name; }
		void setDescription(const std::string& _description) { this->description = _description; }
		void setValue(float _value) { this->value = _value; }
		void setIcon(AEGfxTexture* iconTexture) { this->icon = iconTexture; }
		void setCount(int c) { count = c; }

		// Ctors
		Item() : id(0), name("Generic item"), description("If you're reading this congrats on finding this easter egg"), value(0.0f), icon(nullptr), count(0) {}

		Item(int id, const std::string& name, const std::string& description, float value, AEGfxTexture* iconTexture = nullptr, int count = 1)
			: id(id), name(name), description(description), value(value), icon(iconTexture), count(count) {
		}

		// Virtual destructor for proper cleanup of derived classes
		virtual ~Item() {}

		// Virtual clone method for creating copies
		virtual Item* clone() const { return new Item(*this); }
	};

	class Weapon : public Item {
	private:
		float damage;
		float attackSpeed;
		float range;
		Rarity rarity;

	public:
		// Getters
		float getDamage() const { return this->damage; }

		virtual void onAttack(Entity::Entity& attacker, Entity::Entity& target);

		float getAttackSpeed() const { return this->attackSpeed; }
		float getRange() const { return this->range; }
		Rarity getRarity() const { return this->rarity; }

		// Setters
		void setDamage(float _damage) { this->damage = _damage; }
		void setAttackSpeed(float _attackSpeed) { this->attackSpeed = _attackSpeed; }
		void setRange(float _range) { this->range = _range; }
		void setRarity(Rarity _rarity) { this->rarity = _rarity; }


		// Ctors
		Weapon()
			: Item(), damage(0.0f), attackSpeed(0.0f), range(0.0f), rarity(Common) {
		}

		Weapon(int id, const std::string& name, const std::string& description, float value,
			float damage, float attackSpeed, float range, Rarity rarity)
			: Item(id, name, description, value),
			damage(damage), attackSpeed(attackSpeed), range(range), rarity(rarity) {
		}

		virtual Item* clone() const override { return new Weapon(*this); }
	};

	class MeleeWeapon : public Weapon {
	private:
		float knockback;

	public:
		// Getters
		float getKnockback() const { return this->knockback; }

		// Setters
		void setKnockback(float _knockback) { this->knockback = _knockback; }

		// Ctors
		MeleeWeapon()
			: Weapon(), knockback(0.0f) {
		}

		MeleeWeapon(int id, const std::string& name, const std::string& description, float value,
			float damage, float attackSpeed, float range, Rarity rarity, float knockback)
			: Weapon(id, name, description, value, damage, attackSpeed, range, rarity),
			knockback(knockback) {
		}

		virtual Item* clone() const override { return new MeleeWeapon(*this); }
	};

	class RangedWeapon : public Weapon {
	private:
		int ammoCapacity;
		float projectileSpeed;

	public:
		// Getters
		int getAmmoCapacity() const { return this->ammoCapacity; }
		float getProjectileSpeed() const { return this->projectileSpeed; }

		// Setters
		void setAmmoCapacity(int _ammoCapacity) { this->ammoCapacity = _ammoCapacity; }
		void setProjectileSpeed(float projectileSpeed) { this->projectileSpeed = projectileSpeed; }

		// Ctors
		RangedWeapon()
			: Weapon(), ammoCapacity(0), projectileSpeed(0.0f) {
		}

		RangedWeapon(int id, const std::string& name, const std::string& description, float value,
			float damage, float attackSpeed, float range, Rarity rarity,
			int ammoCapacity, float projectileSpeed)
			: Weapon(id, name, description, value, damage, attackSpeed, range, rarity),
			ammoCapacity(ammoCapacity), projectileSpeed(projectileSpeed) {
		}

		virtual Item* clone() const override { return new RangedWeapon(*this); }
	};

	class PlaceableWeapon : public Weapon {
	private:
		float explosionRadius;
		float fuseTime;

	public:
		// Getters
		float getExplosionRadius() const { return this->explosionRadius; }
		float getFuseTime() const { return this->fuseTime; }

		// Setters
		void setExplosionRadius(float explosionRadius) { this->explosionRadius = explosionRadius; }
		void setFuseTime(float fuseTime) { this->fuseTime = fuseTime; }

		// Ctors
		PlaceableWeapon()
			: Weapon(), explosionRadius(0.0f), fuseTime(0.0f) {
		}

		PlaceableWeapon(int id, const std::string& name, const std::string& description, float value,
			float damage, float attackSpeed, float range, Rarity rarity,
			float explosionRadius, float fuseTime)
			: Weapon(id, name, description, value, damage, attackSpeed, range, rarity),
			explosionRadius(explosionRadius), fuseTime(fuseTime) {
		}

		virtual Item* clone() const override { return new PlaceableWeapon(*this); }
	};

	class DefensiveWeapon : public Weapon {
	private:
		float blockAmount;   // Percentage or flat damage reduction
		float durability;    // How long it can block

	public:
		// Getters
		float getBlockAmount() const { return this->blockAmount; }
		float getDurability() const { return this->durability; }

		// Setters
		void setBlockAmount(float _blockAmount) { this->blockAmount = _blockAmount; }
		void setDurability(float _durability) { this->durability = _durability; }

		// Ctors
		DefensiveWeapon()
			: Weapon(), blockAmount(0.0f), durability(100.0f) {
		}

		DefensiveWeapon(int id, const std::string& name, const std::string& description, float value,
			float damage, float attackSpeed, float range, Rarity rarity,
			float blockAmount, float durability)
			: Weapon(id, name, description, value, damage, attackSpeed, range, rarity),
			blockAmount(blockAmount), durability(durability) {
		}

		virtual Item* clone() const override { return new DefensiveWeapon(*this); }
	};

	class Consumable : public Item {
	private:
		ConsumableType type;
		float effectAmount;    // Amount of health/speed/strength
		float effectDuration;  // 0.0f for instant effects (health restore)

	public:
		// Getters
		ConsumableType getType() const { return this->type; }
		float getEffectAmount() const { return this->effectAmount; }
		float getEffectDuration() const { return this->effectDuration; }

		// Setters
		void setType(ConsumableType _type) { this->type = _type; }
		void setEffectAmount(float _effectAmount) { this->effectAmount = _effectAmount; }
		void setEffectDuration(float _effectDuration) { this->effectDuration = _effectDuration; }

		// Ctors
		Consumable()
			: Item(), type(HealthRestore), effectAmount(0.0f), effectDuration(0.0f) {
		}

		Consumable(int id, const std::string& name, const std::string& description, float value,
			ConsumableType type, float effectAmount, float effectDuration)
			: Item(id, name, description, value),
			type(type), effectAmount(effectAmount), effectDuration(effectDuration) {
		}

		virtual Item* clone() const override { return new Consumable(*this); }

		// Use method - to be implemented in game logic
		// void use(Player* player); // Apply effect to player
	};

	// Item Registry and Factory
	class ItemRegistry {
	private:
		static std::map<int, Item*> prototypes;

	public:
		// Register an item prototype
		static void registerItem(Item* prototype);

		// Create a new instance of an item by ID
		static Item* createItem(int itemID);

		// Check if item exists
		static bool hasItem(int itemID);

		// Clean up prototypes (call on shutdown)
		static void cleanup();
	};

	// Initialize all items
	void init(void);
	
	// Loading the icons for the items
	void load();

	// Unloading the icons for the items
	void unload();

	// Helper function to get item by ID (returns nullptr if not found)
	Item* getItemPrototype(int itemID);
}

#endif // INVENTORY_HPP