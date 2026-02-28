#include "EntityManager.hpp"

namespace EntityManager {
	std::unordered_map<std::string, std::unique_ptr<Entity::Entity>> entities;

	Entity::Entity* get(const std::string& name) {
		auto it = entities.find(name);
		return (it != entities.end()) ? it->second.get() : nullptr;
	}

	Entity::Player* getPlayer(const std::string& name) {
		return dynamic_cast<Entity::Player*>(get(name));
	}

	Entity::NPC* getNPC(const std::string& name) {
		return dynamic_cast<Entity::NPC*>(get(name));
	}

	Entity::Enemy* getEnemy(const std::string& name) {
		return dynamic_cast<Entity::Enemy*>(get(name));
	}

	// Entities dont have an update function yet
	//void updateAll() {
	//	for (auto& pair : entities) {
	//		pair.second->update();
	//	}
	//}

	void drawAll() {
		for (auto& pair : entities) {
			pair.second->draw();
		}
	}

	void init(void) {
		// Initialize player
		EntityManager::create<Entity::Player>("player", 0.0f, 0.0f, 50.0f, 50.0f, 100.0f, 5.0f, 5.0f);

		// Initialize NPCs
		auto* prasanna = EntityManager::create<Entity::NPC>("prasanna", 200.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* soroor = EntityManager::create<Entity::NPC>("soroor", 400.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* gerald = EntityManager::create<Entity::NPC>("gerald", 600.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);

		// Set NPC sprites
		prasanna->setSprite(AEGfxTextureLoad("Assets/Images/Entities/prasanna.png"));
		gerald->setSprite(AEGfxTextureLoad("Assets/Images/Entities/gerald.png"));
		soroor->setSprite(AEGfxTextureLoad("Assets/Images/Entities/soroor.png"));

		// Set prasanna dialog
		prasanna->setDialogLines({
			"Hey there, I'm Prasanna! Welcome to Meowijuana!",
			"Feel free to explore around and talk to everyone!",
			"Also, if you need any help, just ask me!"
			});

		// Set soroor dialog
		soroor->setDialogLines({
			"Sup, I'm Soroor. I heard you just got here.",
			"You should talk to Prasanna if you need any help, he's a cool dude.",
			"Anyway, I'm just chillin here, so have fun exploring!"
			});
	}

	void clear() {
		entities.clear();
	}
}
