#ifndef ENTITY_MANAGER_HPP
#define ENTITY_MANAGER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include "../Entity.hpp"

namespace EntityManager {
	// Entity storage (Entities go here)
	extern std::unordered_map<std::string, std::unique_ptr<Entity::Entity>> entities;

	// Template function to create and register entities
	template<typename T>
	T* create(const std::string& name, float x, float y, float width, float height, float hp, float speed, float armor) {
		auto entity = std::make_unique<T>(x, y, width, height, hp, speed, armor);
		T* ptr = entity.get();
		entities[name] = std::move(entity);
		return ptr;
	}

	// Get entity by name
	Entity::Entity* get(const std::string& name);

	// Type-safe getters (just in case)
	Entity::Player* getPlayer(const std::string& name);
	Entity::NPC* getNPC(const std::string& name);
	Entity::Enemy* getEnemy(const std::string& name);

	// Update and draw all entities

	// TODO: entities dont have update yet
	//void updateAll();
	void drawAll();

	// init
	void init(void);

	// Cleanup
	void clear();
}

#endif // ENTITY_MANAGER_HPP
