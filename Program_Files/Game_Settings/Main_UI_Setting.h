/*==============================================================================

	Update Manager Title And Main UI [Main_UI_Setting.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef MAIN_UI_H
#define MAIN_UI_H
#include "Game_Menu.h"
#include "Setting.h"
#include "Game_Select.h"
#include "Game_Intro_Manager.h"

void Main_UI_Initialize();
void Main_UI_Finalize();

void Main_UI_Update(double elapsed_time);
#endif // MAIN_UI_H