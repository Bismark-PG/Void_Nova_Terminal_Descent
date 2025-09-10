/*==============================================================================

	Manage Game Screen [Game_Screen_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H
#include "Game_Screen_Manager.h"
#include <iostream>

extern Game_Manager GM;

void Game_Screen_Manager_Initialize();
void Game_Screen_Manager_Finalize();

void Update_Main_Screen(Main_Screen Main_Input);
void Update_Sub_Screen(Sub_Screen Sub_Input);
void Update_Game_Select_Screen(Game_Select_Screen Game_Select_Input);

#endif // SCREEN_MANAGER_H
