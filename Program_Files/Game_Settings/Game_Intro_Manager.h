/*==============================================================================

	Manage Game Intro [Game_Intro_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_INTRO_MANAGER_H
#define GAME_INTRO_MANAGER_H

enum class GAME_INTRO_STATE
{
	NONE,
	G_INTRO_WAIT,
	G_INTRO_PLAY,
	G_INTRO_BACK,
	G_INTRO_DONE
};

void Game_Intro_Update();

void Set_Game_Intro_State(GAME_INTRO_STATE state);
GAME_INTRO_STATE Get_Game_Intro_State();

#endif// GAME_INTRO_MANAGER_H