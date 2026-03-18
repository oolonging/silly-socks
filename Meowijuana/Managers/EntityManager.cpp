#include "../pch.h"
#include "EntityManager.hpp"

namespace EntityManager {
	std::unordered_map<std::string, std::unique_ptr<Entity::Entity>> entities;

	// dumping this here for now so i can test it
	SpriteManager::SpriteSheet* playerAttackSpritesheet = nullptr;
	
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

	void draw(const std::string& name) {
		auto it = entities.find(name);
		if (it != entities.end()) {
			it->second->draw();
		}
	}

	void init(void) {
		// initialize the sprite sheet to attack
		playerAttackSpritesheet = SpriteManager::loadSpriteSheet("playerAttackSpritesheet", "Assets/Images/Items/-sword.png", 297.0f, 117.0f, 99.0f, 117.0f);

		// Initialize player
		EntityManager::create<Entity::Player>("player", 0.0f, 0.0f, 50.0f, 50.0f, 100.0f, 5.0f, 5.0f);

		auto* player = EntityManager::getPlayer("player");
		player->setSpritesheet("playerSpritesheet", "Assets/Images/SpriteSheets/player_spritesheet.png",
			160.0f, 160.0f,
			40.0f, 40.0f
		);
		player->setWalkAnimation("playerWalk", "playerSpritesheet", 0, 1, 4, 0.4);
		player->setAttackAnimation("playerAttack", "playerAttackSpritesheet", 0, 0, 3, 0.4f, true);


		// Initialize NPCs
		auto* prasanna = EntityManager::create<Entity::NPC>("prasanna", 200.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* soroor = EntityManager::create<Entity::NPC>("soroor", 400.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* gerald = EntityManager::create<Entity::NPC>("gerald", 000.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* smelly = EntityManager::create<Entity::NPC>("smelly", -500.0f, 50.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);

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



	// enemies stored separately so we can clear per-room
	std::vector<std::unique_ptr<Entity::Enemy>> enemies;

	void spawnEnemies(int count, float areaW, float areaH) {
		enemies.clear();
		for (int i = 0; i < count; i++) {

			// random position within room bounds, with some padding
			float x = AERandFloat() * areaW - (areaW / 2.0f);
			float y = AERandFloat() * areaH - (areaH / 2.0f);
			enemies.push_back(std::make_unique<Entity::Enemy>(
				x, y, 50.0f, 50.0f, 100.0f, 2.0f, 0.0f 
			));
		}
	}

	void clearEnemies() {
		enemies.clear();
	}

	void updateEnemies(Entity::Player& player) {
		for (auto& e : enemies) {
			if (e->isAlive()) {
				e->tickAttackTimer();
				if (e->canAttack()) {
					e->attack(player);
					e->resetAttackTimer();
				}
			}
		}
	}

	void weaponEnemies(Inventory::Weapon* weapon) { 
		for (auto& e : enemies) { 
			e->setWeapon(weapon);
		} 
	}

	void attackEnemies(Entity::Player& player) {
		for (auto& e : enemies) {
			if (e->isAlive()) {
				player.attack(*e);
				if (e->getHp() <= 0) e->setHp(0);
			}
		}
	}

	void drawEnemies(Entity::Player& player, World::worldGrid& Griddy, bool pause) {
		for (auto& e : enemies) {
			if (e->getHp() > 0)
				e->draw(player, Griddy, pause);
		}
	}

	bool allEnemiesDead() {
		for (auto& e : enemies)
			if (e->isAlive()) return false;
		return true;
	}

	void clear() {
		entities.clear();
	}
}


