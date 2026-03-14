#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#pragma once
extern bool isPaused;

void PauseMenu_Load();
void PauseMenu_Initialize();
void PauseMenu_Update();
void PauseMenu_Draw();
void PauseMenu_Free();
void PauseMenu_Unload();

#endif