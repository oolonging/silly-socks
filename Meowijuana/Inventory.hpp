#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <string>

namespace Inventory {
	// Item, Item:weapon Item:weapon:melee, Item:weapon:ranged, item:consumable

	enum Rarity {
		Common,
		Uncommon,
		Rare,
		Epic,
		Legendary
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
	};

	class Weapon : public Item {
		private:
			float damage;
			float attackSpeed;
			float range;
			Rarity rarity;

		public:
			Weapon()
				: Item(), damage(0.0f), attackSpeed(0.0f), range(0.0f), rarity(Common) {
			}

			Weapon(int id, const std::string& name, const std::string& description, float value,
				float damage, float attackSpeed, float range, Rarity rarity)
				: Item(id, name, description, value),
				damage(damage), attackSpeed(attackSpeed), range(range), rarity(rarity) {
			}
	};
}

#endif // INVENTORY_HPP
