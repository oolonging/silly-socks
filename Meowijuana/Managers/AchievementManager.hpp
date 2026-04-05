/**
 * @file AchievementManager.hpp
 * @author Saahil
 * @brief Manages game achievements including registration, unlocking, persistence, and popup display.
 */

#ifndef ACHIEVEMENT_MANAGER_HPP
#define ACHIEVEMENT_MANAGER_HPP

#include "../pch.h"


 /**
  * @struct Achievement
  * @brief Represents a single game achievement with an ID, name, description, and unlock state.
  */
typedef struct Achievement {
	std::string id;					///< Unique ID for the achievement.
	std::string name;				///< Display name of the achievement.
	std::string description;		///< Description of what needs to be done to earn the achievement.
	bool isUnlocked = false;		///< Whether the achievement has been unlocked.

	/**
	 * @brief Default constructor. Creates an invalid placeholder achievement.
	 */
	Achievement(void)
		: Achievement("0000", "Invalid achievement", "You must have messed up really bad to be reading this. well done", false) {
	}

	/**
	 * @brief Parameterized constructor.
	 * @param id Unique achievement ID.
	 * @param name Display name.
	 * @param description Achievement description.
	 * @param isUnlocked Whether the achievement starts unlocked.
	 */
	Achievement(std::string id, std::string name, std::string description, bool isUnlocked)
		: id(id), name(name), description(description), isUnlocked(isUnlocked) {
	}
} Achievement;

/**
 * @class AchievementManager
 * @brief Singleton manager for registering, unlocking, saving/loading, and displaying achievements.
 */
class AchievementManager {
public:
	/**
	 * @brief Returns the singleton instance of AchievementManager.
	 * @return Reference to the AchievementManager instance.
	 */
	static AchievementManager& get();

	/**
	 * @brief Initializes the manager and loads achievements from the save file.
	 * @param saveFilePath Path to the achievement save file.
	 */
	void init(const std::string& saveFilePath);

	/**
	 * @brief Updates popup timers and animation state.
	 * @param dt Delta time in seconds. Defaults to AEFrameRateControllerGetFrameTime().
	 */
	void update(float dt = AEFrameRateControllerGetFrameTime());

	/**
	 * @brief Draws the current achievement popup if one is active.
	 */
	void draw(void);

	/**
	 * @brief Registers a new achievement that can be unlocked later.
	 * @param id Unique achievement ID.
	 * @param name Display name of the achievement.
	 * @param description Description of the achievement.
	 */
	void registerAchievement(const std::string& id, const std::string& name, const std::string& description);

	/**
	 * @brief Unlocks an achievement by ID and triggers a popup notification.
	 * @param id ID of the achievement to unlock.
	 */
	void unlockAchievement(const std::string& id);

	/**
	 * @brief Checks whether an achievement has been unlocked.
	 * @param id ID of the achievement to check.
	 * @return True if the achievement is unlocked.
	 */
	bool isUnlocked(const std::string& id) const;

private:
	AchievementManager(void) = default;		///< Private default constructor (singleton).
	~AchievementManager(void) = default;	///< Private destructor (singleton).

	/// @brief Deleted copy constructor to prevent duplication.
	AchievementManager(const AchievementManager&) = delete;
	/// @brief Deleted copy assignment to prevent duplication.
	AchievementManager& operator=(const AchievementManager&) = delete;

	/**
	 * @brief Saves all achievement states to the save file.
	 */
	void SaveAchievements(void);

	/**
	 * @brief Loads achievement states from the save file.
	 */
	void LoadAchievements(void);

	/// @brief Map of achievement IDs to Achievement objects.
	std::unordered_map<std::string, Achievement> achievements;

	/// @brief Path to the achievement save file.
	std::string saveFilepath;

	/**
	 * @struct Popup
	 * @brief Data for a pending achievement popup notification.
	 */
	typedef struct Popup {
		std::string name;			///< Achievement name to display.
		std::string description;	///< Achievement description to display.
	} Popup;

	/// @brief Queue of popups waiting to be displayed.
	std::queue<Popup> popupQueue;

	bool popupActive = false;				///< Whether a popup is currently being shown.
	float popupTimer = 0.0f;				///< Timer tracking current popup display duration.
	const float POPUP_DURATION = 3.0f;		///< How long each popup lasts in seconds.
};


#endif // ACHIEVEMENT_MANAGER_HPP