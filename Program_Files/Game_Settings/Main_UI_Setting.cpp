/*==============================================================================

	Update Manager Title And Main UI [Main_UI_Setting.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Main_UI_Setting.h"

void Main_UI_Initialize()
{
	Title_Initialize();
	Menu_Initialize();
	Setting_Initialize();
	Game_Select_Initialize();
}

void Main_UI_Finalize()
{
	Game_Select_Finalize();
	Setting_Finalize();
	Menu_Finalize();
	Title_Finalize();
}

void Main_UI_Update(double elapsed_time)
{
	Title_Update();
	Menu_Update();
	Game_Intro_Update();
	Setting_Update();
	Game_Select_Update();
}
