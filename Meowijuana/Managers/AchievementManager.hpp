#ifndef ACHIEVEMENT_MANAGER_HPP
#define ACHIEVEMENT_MANAGER_HPP

#include "../pch.h"


struct Achievement {
	std::string id;					// Unique ID for achievements
	std::string name;				// Achievement name
	std::string description;		// Achievement description, What needs to be done to get the achievement
	bool isUnlocked = false;		// Tracks if the achievement is unlocked
};

class AchievementManager {
	public:
		// Return self
		static AchievementManager& get();
		
		// init the manager with the savefile path 
		void init(const std::string& saveFilePath);

		// To handle popup timers
		void update(float dt = AEFrameRateControllerGetFrameTime());
		
		// Draw the achievement popup
		void draw(void);

		// create a new achievement throughout the game
		void registerAchievement(const std::string& id, const std::string& name, const std::string& description);

		// Unlock achievement
		void unlockAchievement(const std::string& id);

		// Check if an achivement is unlocked (for the achievements page)
		bool isUnlocked(const std::string& id) const;

	private:
		AchievementManager(void) = default;
		~AchievementManager(void) = default;

		// Prevent copying
		AchievementManager(const AchievementManager&) = delete;
		AchievementManager& operator=(const AchievementManager&) = delete;

		// Loading and saving to file
		void SaveAchievements(void);
		void LoadAchievements(void);

		// Contain achievements
		std::unordered_map<std::string, Achievement> achievements;
		
		// Contains where the file is saved
		std::string saveFilepath;

		// Data for popup
		typedef struct Popup {
			std::string name;
			std::string description;
		} Popup;

		// Add a queue in case there are  multiple achievements unlocked at once
		std::queue<Popup> popupQueue;

		// control vars for the popup
		bool popupActive = false;			// False by default
		float popupTimer = 0.0f;			// control for popup
		const float POPUP_DURATION = 3.0f;	// how long the popup should last (in seconds)
};


#endif // ACHIEVEMENT_MANAGER_HPP
