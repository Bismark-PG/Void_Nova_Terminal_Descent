/*==============================================================================

	Game Mode Manager [Game_Mode_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_MODE_MANAGER_H
#define GAME_MODE_MANAGER_H
#include "Game_Select.h" 

enum class GameMode
{
	NONE,		
	NEW_GAME,	
	CONTINUE,	
	STAGE_SELECT
};

void Set_Game_Mode(GameMode mode);
GameMode Get_Game_Mode();


void Save_Last_Cleared_Stage(STAGE_NUM Stage_Number);
STAGE_NUM Load_Last_Cleared_Stage();


#endif // GAME_MODE_MANAGER_H