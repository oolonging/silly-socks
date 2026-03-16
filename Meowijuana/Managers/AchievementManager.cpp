#include "AchievementManager.hpp"

AchievementManager& AchievementManager::get() {
	static AchievementManager instance;
	return instance;
}

void AchievementManager::init(const std::string& saveFilepath) {
	this->saveFilepath = saveFilepath;
	LoadAchievements();
}

void AchievementManager::registerAchievement(const std::string& id, const std::string& name, const std::string& description) {
	if (achievements.find(id) == achievements.end()) {
		Achievement achievement = { id, name, description, false };
		achievements[id] = achievement;
	}
}

void AchievementManager::unlockAchievement(const std::string& id) {
	auto it = achievements.find(id);
	if (it != achievements.end() && !it->second.isUnlocked) {
		it->second.isUnlocked = true;

		// Queue the notification
		popupQueue.push({ it->second.name, it->second.description });

		// Save state immediately
		SaveAchievements();

		std::cout << "[Achievement get] " << it->second.name << std::endl;
	}
}

bool AchievementManager::isUnlocked(const std::string& id) const {
	auto it = achievements.find(id);
	if (it != achievements.end()) {
		return it->second.isUnlocked;
	}
	return false;
}

void AchievementManager::update(float dt) {
	if (popupActive) {
		popupTimer -= dt;
		if (popupTimer <= 0.0f) {
			popupActive = false;
		}
	}
	else if (!popupQueue.empty()) {
		popupActive = true;
		popupTimer = POPUP_DURATION;
	}
}

void AchievementManager::draw() {
	if (popupActive && popupQueue.empty()) {
		const Popup& currentPopup = popupQueue.front();

		// ill add the shapes later but i might have graphics I can use instead
		// also modify the update function depending on if i want it to slide up or just vanish
	}
	else if (!popupQueue.empty()) {
		popupQueue.pop();
	}
}

void AchievementManager::SaveAchievements() {
	std::ofstream file(saveFilepath);

	if (file.is_open()) {
		for (const auto& pair : achievements) {
			file << pair.first << " " << (pair.second.isUnlocked ? 1 : 0) << std::endl;
		}
		file.close();
	}
}

void AchievementManager::LoadAchievements() {
	std::ifstream file(saveFilepath);

	if (file.is_open()) {
		std::string id;
		int unlockedFlag;
		while (file >> id >> unlockedFlag) {
			if (achievements.find(id) != achievements.end()) {
				achievements[id].isUnlocked = (unlockedFlag == 1);
			}
		}
		file.close();
	}
}
