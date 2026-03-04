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

	void draw(const std::string& name) {
		auto it = entities.find(name);
		if (it != entities.end()) {
			it->second->draw();
		}
	}

	void init(void) {
		// Initialize player
		EntityManager::create<Entity::Player>("player", 0.0f, 0.0f, 50.0f, 50.0f, 100.0f, 5.0f, 5.0f);

		// Initialize NPCs
		auto* prasanna = EntityManager::create<Entity::NPC>("prasanna", 200.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* soroor = EntityManager::create<Entity::NPC>("soroor", 400.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* gerald = EntityManager::create<Entity::NPC>("gerald", 000.0f, 200.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);
		auto* smelly = EntityManager::create<Entity::NPC>("smelly", -600.0f, 000.0f, 50.0f, 50.0f, 100.0f, 0.0f, 5.0f);

		// Set NPC sprites
		prasanna->setSprite(AEGfxTextureLoad("Assets/Images/Entities/prasanna.png"));
		gerald->setSprite(AEGfxTextureLoad("Assets/Images/Entities/gerald.png"));
		soroor->setSprite(AEGfxTextureLoad("Assets/Images/Entities/soroor.png"));
		smelly->setSprite(AEGfxTextureLoad("Assets/Images/Entities/smelly.png")); // plus 1 poorly scaled placeholder becauase i dont want my lecturers staring at me while i work

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

		smelly->setDialogLines({
			"Hey man! Welcome to the dungeons! There's 4 levels to each dungeon, each with increasing levels of difficulty. We're on the first floor, so it's relatively safe for now.",
			"Here! I've taken the liberty to pass you a [CARROT SWORD]. I'll walk you through this floor, and then you'll be on your own for the next few!",
			"First off, see those [INSERT ENEMY NAME] over there? They'll attack you on sight. You'll need to get rid of them while you go through the dungeons. There's too many of them, and if left unchecked? One day, they'll leave the dungeons in search of more prey.",
			"Use your [LEFT MOUSE BUTTON] to attack! Go give it a try.",

			"@",

			"Nice! OK, seems like my job here is done. I'm gonna go clock off for the day. You have fun though!",
			"Oh- The next floor? Right, sorry. Just head to that teleporter over there to continue."
			});
	}

	void clear() {
		entities.clear();
	}
}
