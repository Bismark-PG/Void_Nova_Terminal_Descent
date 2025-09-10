/*==============================================================================

	Update Game Screen [Update_Game.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Update_Game.h"
#include <sstream>


void Main_Game_Screen_Update()
{
	std::stringstream Debug;

	switch (GM.Get_Current_Main_Screen())
	{
	case Main_Screen::M_WAIT:
		Logo_Draw();
		break;

	case Main_Screen::MAIN:
		Title_Draw();
		break;

	case Main_Screen::MENU_SELECT:
		Menu_Select_Draw();
		Menu_UI_Draw();
		break;

	case Main_Screen::SELECT_GAME:
		Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
		break;

	case Main_Screen::SELECT_SETTINGS:
		Update_Sub_Screen(Sub_Screen::SETTINGS);
		break;

	case Main_Screen::EXIT:
		Debug << "Exiting Game..." << "\n";
		break;

	case Main_Screen::M_DONE:
		break;
	}

	OutputDebugStringA(Debug.str().c_str());
}

void Sub_Game_Screen_Update()
{
	switch (GM.Get_Current_Sub_Screen())
	{
	case Sub_Screen::S_WAIT:
		if (GM.Get_Current_Main_Screen() == Main_Screen::SELECT_SETTINGS)
			Update_Sub_Screen(Sub_Screen::SETTINGS);
		break;

	case Sub_Screen::SETTINGS:
		Setting_Draw();
		break;

	case Sub_Screen::S_DONE:
		break;
	}
}

void Game_Select_Screen_Update()
{
	switch (GM.Get_Current_Game_Select_Screen())
	{
	case Game_Select_Screen::G_WAIT:
		if (GM.Get_Current_Main_Screen() == Main_Screen::SELECT_GAME)
			Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
		break;

	case Game_Select_Screen::GAME_MENU_SELECT:
		Game_Select_Draw();
		break;

	case Game_Select_Screen::GAME_PLAYING:
		Stage_Draw();
		break;

	case Game_Select_Screen::GAME_ENDING:
		Ending_Draw();
		break;

	case Game_Select_Screen::G_DONE:
		break;
	}
}

void Game_Screen_Update()
{
	Main_Game_Screen_Update();
	Sub_Game_Screen_Update();
	Game_Select_Screen_Update();
}
