#ifndef TUTORIAL_DUNGEON_HPP
#define TUTORIAL_DUNGEON_HPP

namespace Death {
    extern bool dead;
    extern float fade;
    extern int deathCounter;
    extern float opacity;
}

void TutorialDungeon_Load();
void TutorialDungeon_Initialize();
void TutorialDungeon_Update();
void TutorialDungeon_Draw();
void TutorialDungeon_Free();
void TutorialDungeon_Unload();

#endif // TUTORIAL_DUNGEON_HPP
