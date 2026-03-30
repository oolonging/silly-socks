#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include "../Entity.hpp"

namespace EntityManager {
	// Entity storage (Entities go here)
	extern std::unordered_map<std::string, std::unique_ptr<Entity::Entity>> entities;

	// hoisted get
	Entity::Entity* get(const std::string& name);

	// Template function to create and register entities
	template<typename T>
	T* create(const std::string& name, float x, float y, float width, float height, float hp, float speed, float armor) {
		// check if the NPC with the specified name exists. if it does then dont make a new one with a unique ptr since the old one will be abandoned and dangling 
		auto* existing = get(name);
		if (existing != nullptr) {
			printf("YOU TRIED TO MAKE AN NPC THAT ALREADY EXISTS\n");
			return dynamic_cast<T*>(existing);
		}

		auto entity = std::make_unique<T>(x, y, width, height, hp, speed, armor);
		T* ptr = entity.get();
		entities[name] = std::move(entity);
		return ptr;
	}

	// Get entity by name
	//Entity::Entity* get(const std::string& name);

	// Type-safe getters (just in case)
	Entity::Player* getPlayer(const std::string& name);
	Entity::NPC* getNPC(const std::string& name);
	Entity::Enemy* getEnemy(const std::string& name);

	// Update and draw all entities

	// TODO: entities dont have update yet
	//void updateAll();
	void draw(const std::string& name);
	void drawAll();

	// init
	void init(void);

	// Cleanup
	void clear();



	// ENEMY STUFF

	void spawnEnemies(int count, float areaW, float areaH, float difficultyMult = 1.0f);
	void clearEnemies();
	void updateEnemies(Entity::Player& player);
	void attackEnemies(Entity::Player& player);
	void drawEnemies(Entity::Player& player, World::worldGrid& Griddy, bool pause);
	void weaponEnemies(Inventory::Weapon* weapon);
	bool allEnemiesDead();
}

#endif // ENTITY_MANAGER_HPP
