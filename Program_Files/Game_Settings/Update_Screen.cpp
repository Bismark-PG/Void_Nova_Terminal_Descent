/*==============================================================================

	Manage Game Screen [Game_Screen_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Update_Screen.h"

void Game_Screen_Manager_Initialize()
{
	Game_M.Change_Screen(Main_Screen::M_WAIT, Sub_Screen::S_WAIT, Game_Select_Screen::G_WAIT);
}

void Game_Screen_Manager_Finalize()
{
	Game_M.Change_Screen(Main_Screen::M_DONE, Sub_Screen::S_DONE, Game_Select_Screen::G_DONE);
}

void Update_Main_Screen(Main_Screen Main_Input)
{
	Game_M.Change_Screen(Main_Input, Game_M.Get_Current_Sub_Screen(), Game_M.Get_Current_Game_Select_Screen());
}

void Update_Sub_Screen(Sub_Screen Sub_Input)
{
	Game_M.Change_Screen(Game_M.Get_Current_Main_Screen(), Sub_Input, Game_M.Get_Current_Game_Select_Screen());
}

void Update_Game_Select_Screen(Game_Select_Screen Game_Select_Input)
{
	Game_M.Change_Screen(Game_M.Get_Current_Main_Screen(), Game_M.Get_Current_Sub_Screen(), Game_Select_Input);
}
