/**
 * @file AchievementManager.cpp
 * @author Saahil
 * @brief Manages game achievements including registration, unlocking, persistence, and popup display.
 */

#include "AchievementManager.hpp"

#include "../Graphics.hpp"

AchievementManager& AchievementManager::get() {
	static AchievementManager instance;
	return instance;
}

void AchievementManager::init(const std::string& _saveFilepath) {
	this->saveFilepath = _saveFilepath;
	
	// Future Implementation: add some achievements here
	achievements.insert(std::pair<std::string, Achievement>(
		"Start the game",
		Achievement("0001", "Start the game", "Run the game for the first time", false)
	));


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
	if (popupActive && !popupQueue.empty()) {
		const Popup& currentPopup = popupQueue.front();

		// range from (0.0f - 1.0f)
		float percentageLeft = popupTimer / POPUP_DURATION;

		// Future implementation: play a sound when an achievement is earned

		// Future implementation: show an animation
		// Placeholder rectangular overlay for now
		Color::fill(255.0f, 0.0f, 0.0f, percentageLeft);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		Shapes::rect(0.0f, 0.0f, 500.0f, 500.0f, Shapes::CENTER);
		Color::textFill(Color::Preset::White);
		Text::text(currentPopup.name.c_str(), 0.0f, 0.0f, Text::CENTER_H, Text::TOP);
		Text::text(currentPopup.description.c_str(), 0.0f, 0.0f, Text::CENTER_H, Text::BOTTOM);

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
