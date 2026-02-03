#pragma once

enum GS_STATES
{
	GS_LEVEL1 = 0,
	GS_LEVEL2,

	GS_QUIT,
	GS_RESTART
};

typedef void(*FP)(void);

extern int current, previous, next;

extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();