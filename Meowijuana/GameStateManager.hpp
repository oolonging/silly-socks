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
	GS_FARM,			// landing, farm yet to port over from testing-andrea, (todo: me/drea to draw the npcs and everything after)
	GS_TUTORIAL,
	GS_DUNGEON,
	GS_TUTDUN,			

	// Testing states for individual game mechanics
	GS_TESTING,

	// Legacy states for testing purposes, will be removed soon
	GS_LEVEL1,
	GS_LEVEL2,

	// Utility states
	GS_QUIT,
	GS_RESTART,
	GS_BACK,

	GS_A,				// farming tutorial 
	GS_X,				// to the right of farm, dungeon tutorial for now (remind me to add an option to skip tutorial && ask if dungeon can be removed) 
	GS_NEW,
	GS_RESPAWN

};

typedef void(*FP)(void);

extern int current, previous, next;
extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();

#endif // GAMESTATEMANAGER_HPP
