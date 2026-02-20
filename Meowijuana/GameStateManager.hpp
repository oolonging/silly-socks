#ifndef GAMESTATEMANAGER_HPP
#define GAMESTATEMANAGER_HPP


enum GSM_STATES
{
	// Starting states
	GS_SPLASH,
	GS_MAIN_MENU,

	// Post main menu states
	GS_SETTINGS,
	GS_CREDITS,

	// Gameplay states
	GS_FARM,
	GS_DUNGEON,

	// Testing states for individual game mechanics
	GS_TESTING,

	// Legacy states for testing purposes, will be removed soon
	GS_LEVEL1,
	GS_LEVEL2,

	// Utility states
	GS_QUIT,
	GS_RESTART,
	GS_BACK

};

typedef void(*FP)(void);

extern int current, previous, next;
extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();

#endif // GAMESTATEMANAGER_HPP
