/*==============================================================================

	Game State [Game_State.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_STATE_H
#define GAME_STATE_H

enum class Main_Screen
{
    M_WAIT,
    MAIN,
    MENU_SELECT,
    SELECT_GAME,
    SELECT_SETTINGS,
    EXIT,
    M_DONE
};

enum class Sub_Screen
{
    S_WAIT,
	SETTINGS,
    S_DONE
};

enum class Game_Select_Screen 
{
    G_WAIT,
    GAME_MENU_SELECT,
    GAME_PLAYING,
    GAME_ENDING,
	G_DONE
};

#endif // GAME_STATE_H