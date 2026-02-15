#ifndef GAMESTATEMANAGER_HPP
#define GAMESTATEMANAGER_HPP


enum GS_STATES
{
	GS_SPLASH_SCREEN,
	GS_MAIN_MENU,

	GS_LEVEL1,
	GS_LEVEL2,

	GS_QUIT,
	GS_RESTART
};

typedef void(*FP)(void);

extern int current, previous, next;

extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();

#endif // GAMESTATEMANAGER_HPP
