#include "Inventory.hpp"

namespace Inventory {
	// Static member initialization
	std::map<int, Item*> ItemRegistry::prototypes;

	void ItemRegistry::registerItem(Item* prototype) {
		if (prototype != nullptr) {
			prototypes[prototype->getID()] = prototype;
		}
	}

	Item* ItemRegistry::createItem(int itemID) {
		auto it = prototypes.find(itemID);
		if (it != prototypes.end()) {
			return it->second->clone(); // Create a copy using clone()
		}
		return nullptr;
	}

	bool ItemRegistry::hasItem(int itemID) {
		return prototypes.find(itemID) != prototypes.end();
	}

	void ItemRegistry::cleanup() {
		for (auto& pair : prototypes) {
			delete pair.second;
		}
		prototypes.clear();
	}

	Item* getItemPrototype(int itemID) {
		if (!ItemRegistry::hasItem(itemID)) {
			return nullptr;
		}

		return getItemPrototype(itemID);
	}

	void init(void) {
		// Register all weapons
		ItemRegistry::registerItem(new MeleeWeapon(
			ItemID::CARROT_SWORD,
			"Carrot Sword",
			"A sharp veggie blade forged from the finest carrots",
			10.0f,      // value
			15.0f,      // damage
			1.2f,       // attackSpeed
			2.0f,       // range
			Common,     // rarity
			5.0f        // knockback
		));

		ItemRegistry::registerItem(new RangedWeapon(
			ItemID::POTATO_GRENADE,
			"Potato Grenade",
			"An explosive spud that packs a punch",
			25.0f,      // value
			30.0f,      // damage
			0.5f,       // attackSpeed
			10.0f,      // range
			Uncommon,   // rarity
			5,          // ammoCapacity
			15.0f       // projectileSpeed
		));

		ItemRegistry::registerItem(new PlaceableWeapon(
			ItemID::CHERRY_BOMB,
			"Cherry Bomb",
			"Place it and run! BOOM!",
			30.0f,      // value
			50.0f,      // damage
			0.3f,       // attackSpeed
			5.0f,       // range
			Rare,       // rarity
			8.0f,       // explosionRadius
			3.0f        // fuseTime
		));

		ItemRegistry::registerItem(new DefensiveWeapon(
			ItemID::WALNUT_SHIELD,
			"Walnut Shield",
			"As hard as a nut, literally",
			20.0f,      // value
			5.0f,       // damage (shield bash)
			0.8f,       // attackSpeed
			1.0f,       // range
			Common,     // rarity
			0.5f,       // blockAmount (50% damage reduction)
			100.0f      // durability
		));

		// Register all consumables
		ItemRegistry::registerItem(new Consumable(
			ItemID::STRAWBERRY,
			"Strawberry",
			"A juicy berry that restores health",
			5.0f,           // value
			HealthRestore,  // type
			25.0f,          // effectAmount (25 HP)
			0.0f            // effectDuration (instant)
		));

		ItemRegistry::registerItem(new Consumable(
			ItemID::CHILLI,
			"Chilli",
			"Spicy! Gives a temporary speed boost",
			10.0f,          // value
			SpeedBoost,     // type
			1.5f,           // effectAmount (1.5x speed multiplier)
			5.0f            // effectDuration (5 seconds)
		));

		ItemRegistry::registerItem(new Consumable(
			ItemID::SPINACH,
			"Spinach",
			"Popeye's favorite! Temporary strength buff",
			10.0f,          // value
			StrengthBuff,   // type
			1.3f,           // effectAmount (1.3x damage multiplier)
			8.0f            // effectDuration (8 seconds)
		));

		// Register merged/upgraded items
		ItemRegistry::registerItem(new MeleeWeapon(
			ItemID::GOLDEN_CARROT_SWORD,
			"Golden Carrot Sword",
			"Three carrot swords merged into ultimate veggie power!",
			50.0f,      // value
			45.0f,      // damage (3x stronger)
			1.5f,       // attackSpeed (faster)
			3.0f,       // range (longer)
			Rare,       // rarity (upgraded)
			10.0f       // knockback (stronger)
		));
	}
}