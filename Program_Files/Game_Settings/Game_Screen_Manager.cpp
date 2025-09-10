#include "Game_Screen_Manager.h"

void Game_Manager::Change_Screen(Main_Screen M_Screen, Sub_Screen S_Screen, Game_Select_Screen G_Screen)
{
	Current_Main_Screen = M_Screen;
	Current_Sub_Screen = S_Screen;
	Current_Game_Screen = G_Screen;
}

Main_Screen Game_Manager::Get_Current_Main_Screen() const
{
	return Current_Main_Screen;
}

Sub_Screen Game_Manager::Get_Current_Sub_Screen() const
{
	return Current_Sub_Screen;
}

Game_Select_Screen Game_Manager::Get_Current_Game_Select_Screen() const
{
	return Current_Game_Screen;
}