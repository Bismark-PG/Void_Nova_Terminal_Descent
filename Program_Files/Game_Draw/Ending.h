/*==============================================================================

	Game Ending Screen [Ending.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENDING_SCREEN_H
#define ENDING_SCREEN_H

#include "Game_State.h"

enum class ENDING_SEQUENCE
{
	IDLE,				
	ENDING_START,				
	FADE_IN,               
	PLAYER_RISE,           
	CREDIT_ROLL,		
	BASE_APPEAR,		
	PLAYER_MOVE_TO_BASE,   
	COMMANDER_ANIM_START,  
	COMMANDER_APPEAR,	
	COMMANDER_BG_WAIT,
	COMMANDER_MESSAGE,	
	FINAL_FADE_OUT,		
	RESET_TO_MAIN,		
	DONE				
};

void Ending_Initialize();
void Ending_Finalize();

void Update_Ending_Player_Movement(double elapsed_time);
void Ending_Update(double elapsed_time);

void Ending_Draw();
void Ending_Get_Text_POS(float x, float y, float w, float h);

ENDING_SEQUENCE Get_Ending_Status();
void Set_Ending_Status(ENDING_SEQUENCE Status);

#endif // ENDING_SCREEN_H