/*==============================================================================

	Game Mode Manager [Game_Mode_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Game_Mode_Manager.h"

GameMode Selected_Mode;
STAGE_NUM Last_Cleared_Stage = STAGE_NUM::STAGE_NONE;

void Set_Game_Mode(GameMode mode)
{
	Selected_Mode = mode;
}

GameMode Get_Game_Mode()
{
	return Selected_Mode;
}

void Save_Last_Cleared_Stage(STAGE_NUM Stage_Number)
{
	Last_Cleared_Stage = Stage_Number;
}

STAGE_NUM Load_Last_Cleared_Stage()
{
	return Last_Cleared_Stage;
}
