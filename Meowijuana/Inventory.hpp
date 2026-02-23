#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <string>

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

	class Item {
	private:
		int id;
		std::string name;
		std::string description;
		float value; // for selling or buying
		// TODO: add stack size, and also image/icon representation

	public:
		// Getters
		int getID() const { return this->id; }
		std::string getName() const { return this->name; }
		std::string getDescription() const { return this->description; }
		float getValue() const { return this->value; }

		// Setters
		void setID(int id) { this->id = id; }
		void setName(const std::string& name) { this->name = name; }
		void setDescription(const std::string& description) { this->description = description; }
		void setValue(float value) { this->value = value; }

		// Ctors
		Item() : id(0), name("Generic item"), description("If you're reading this congrats on finding this easter egg"), value(0.0f) {}

		Item(int id, const std::string& name, const std::string& description, float value)
			: id(id), name(name), description(description), value(value) {
		}

		// Virtual destructor for proper cleanup of derived classes
		virtual ~Item() {}
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
		float getAttackSpeed() const { return this->attackSpeed; }
		float getRange() const { return this->range; }
		Rarity getRarity() const { return this->rarity; }

		// Setters
		void setDamage(float damage) { this->damage = damage; }
		void setAttackSpeed(float attackSpeed) { this->attackSpeed = attackSpeed; }
		void setRange(float range) { this->range = range; }
		void setRarity(Rarity rarity) { this->rarity = rarity; }

		// Ctors
		Weapon()
			: Item(), damage(0.0f), attackSpeed(0.0f), range(0.0f), rarity(Common) {
		}

		Weapon(int id, const std::string& name, const std::string& description, float value,
			float damage, float attackSpeed, float range, Rarity rarity)
			: Item(id, name, description, value),
			damage(damage), attackSpeed(attackSpeed), range(range), rarity(rarity) {
		}
	};

	class MeleeWeapon : public Weapon {
	private:
		float knockback;

	public:
		// Getters
		float getKnockback() const { return this->knockback; }

		// Setters
		void setKnockback(float knockback) { this->knockback = knockback; }

		// Ctors
		MeleeWeapon()
			: Weapon(), knockback(0.0f) {
		}

		MeleeWeapon(int id, const std::string& name, const std::string& description, float value,
			float damage, float attackSpeed, float range, Rarity rarity, float knockback)
			: Weapon(id, name, description, value, damage, attackSpeed, range, rarity),
			knockback(knockback) {
		}
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
		void setAmmoCapacity(int ammoCapacity) { this->ammoCapacity = ammoCapacity; }
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
	};

	// TODO: will probably not use this
	class DefensiveWeapon : public Weapon {
	private:
		float blockAmount;   // Percentage or flat damage reduction
		float durability;    // How long it can block

	public:
		// Getters
		float getBlockAmount() const { return this->blockAmount; }
		float getDurability() const { return this->durability; }

		// Setters
		void setBlockAmount(float blockAmount) { this->blockAmount = blockAmount; }
		void setDurability(float durability) { this->durability = durability; }

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
		void setType(ConsumableType type) { this->type = type; }
		void setEffectAmount(float effectAmount) { this->effectAmount = effectAmount; }
		void setEffectDuration(float effectDuration) { this->effectDuration = effectDuration; }

		// Ctors
		Consumable()
			: Item(), type(HealthRestore), effectAmount(0.0f), effectDuration(0.0f) {
		}

		Consumable(int id, const std::string& name, const std::string& description, float value,
			ConsumableType type, float effectAmount, float effectDuration)
			: Item(id, name, description, value),
			type(type), effectAmount(effectAmount), effectDuration(effectDuration) {
		}

		// Use method - to be implemented in game logic
		// void use(Player* player); // Apply effect to player
	};

	// Initializes all items
	void init(void);

	// does clean up (might not be needed until the images get involved)
	void exit(void);
}

#endif // INVENTORY_HPP