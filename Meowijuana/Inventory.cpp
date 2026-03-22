#include "pch.h"
#include "Inventory.hpp"

#include "Entity.hpp"

UI_Elements::PlayerInventory inv;
bool showInventory;

namespace Inventory {
	
	namespace Textures {

		// Icons for seeds
		AEGfxTexture* carrotSeedIcon = nullptr;
		AEGfxTexture* cherrySeedIcon = nullptr;
		AEGfxTexture* potatoSeedIcon = nullptr;

		// Icons for plants
		AEGfxTexture* carrotIcon = nullptr;
		AEGfxTexture* cherryIcon = nullptr;
		AEGfxTexture* potatoIcon = nullptr;

		AEGfxTexture* carrotSword = nullptr;
	}

	void load(void)
	{
		Textures::carrotSeedIcon = AEGfxTextureLoad("Assets/Images/Items/CarrotSeeds.png");
		Textures::cherrySeedIcon = AEGfxTextureLoad("Assets/Images/Items/CherrySeeds.png");
		Textures::potatoSeedIcon = AEGfxTextureLoad("Assets/Images/Items/PotatoSeeds.png");

		Textures::carrotIcon = AEGfxTextureLoad("Assets/Images/Items/Carrot.png");
		Textures::cherryIcon = AEGfxTextureLoad("Assets/Images/Items/Cherry.png");
		Textures::potatoIcon = AEGfxTextureLoad("Assets/Images/Items/Potato.png");
		Textures::carrotSword = AEGfxTextureLoad("Assets/Images/Items/CarrotSword.png");
	}

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
			Item* item = pair.second;
			if (item->getIcon()) {
				AEGfxTextureUnload(item->getIcon());
			}
			delete item;
		}
		prototypes.clear();
	}

	Item* getItemPrototype(int itemID) {
		if (!ItemRegistry::hasItem(itemID)) {
			return nullptr;
		}

		return getItemPrototype(itemID);
	}

	void Weapon::onAttack(Entity::Entity& attacker, Entity::Entity& target) {

		float finalDamage = this->getDamage();

		if (attacker.isPlayer())
		{
			Entity::Player* p = dynamic_cast<Entity::Player*>(&attacker);
			if (p->getSwingCount() < 3 && p->findItem(Inventory::ItemID::CARROT) >= 0 && p)
			{
				p->increaseSwingCount();
				int slot = p->findItem(Inventory::ItemID::CARROT);
				if (slot < 0)
				{
					target.takeDamage(finalDamage);
					return;
				}
				
				else
				{
					int carrotCount = p->getInventoryItem(slot)->getCount();
					finalDamage = this->getDamage() + (carrotCount * 5.f);
				}
			}
		}

		target.takeDamage(finalDamage);
	}

	void init(void) {
		// Register all weapons
		ItemRegistry::registerItem(new MeleeWeapon(
			ItemID::CARROT_SWORD,
			"Carrot Sword",
			"A sharp veggie blade forged from the finest carrots",
			10.0f,      // value
			35.0f,      // damage
			2.0f,       // attackSpeed
			85.0f,       // range
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


		// for enemies (basic)
		ItemRegistry::registerItem(new MeleeWeapon(
			ItemID::WOODEN_SWORD,
			"Wooden Sword",
			"Commonly seen wielded by monsters and kids alike",
			10.0f,      // value
			15.0f,      // damage
			1.5f,       // attackSpeed
			85.0f,       // range
			Common,     // rarity
			5.0f        // knockback
		));

		ItemRegistry::registerItem(new Item(
			ItemID::CARROT_SEEDS, 
			"Carrot Seeds", 
			"Plant to grow carrots", 
			5.0f, 
			Textures::carrotSeedIcon, 
			0
		));

		ItemRegistry::registerItem(new Item(
			ItemID::CHERRY_SEEDS, 
			"Cherry Seeds", 
			"Plant to grow cherries", 
			5.0f, 
			Textures::cherrySeedIcon, 
			0
		));

		ItemRegistry::registerItem(new Item(
			ItemID::POTATO_SEEDS,
			"Potato Seeds",
			"Plant to grow potatoes",
			5.0f,
			Textures::potatoSeedIcon,
			0
		));

		ItemRegistry::registerItem(new Item(
			ItemID::CARROT,
			"Carrot Crop",
			"Carrot grown with care!",
			5.0f,
			Textures::carrotIcon,
			0
		));
		
		ItemRegistry::registerItem(new Item(
			ItemID::CHERRY,
			"Cherry Crop",
			"Cherry grown with care!",
			5.0f,
			Textures::cherryIcon,
			0
		));

		ItemRegistry::registerItem(new Item(
			ItemID::POTATO,
			"Potato Crop",
			"Potato grown with care!",
			5.0f,
			Textures::potatoIcon,
			0
		));
		
		ItemRegistry::registerItem(new Item(
			ItemID::CARROT_SWORD_INV,
			"Carrot Sword",
			"Carrot sword surprisingly sharp",
			5.0f,
			Textures::carrotSword,
			0
		));

	}

	void unload(void)
	{
		// Then free prototypes + their textures
		ItemRegistry::cleanup(); 
	}

}
