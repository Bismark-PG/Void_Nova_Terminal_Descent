/*==============================================================================

	Game Ending Screen [Ending.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENDING_SCREEN_H
#define ENDING_SCREEN_H

#include "Game_State.h"

enum class ENDING_SEQUENCE
{
	FADE_IN,					
	PLAYER_RISE,				
	SHOW_TEXT_1,				
	SHOW_TEXT_2,				
	SHOW_TEXT_3,				
	SHOW_TEXT_4,				
	SHOW_TEXT_5,				
	SHOW_TEXT_6,				
	BASE_DESCEND,				
	PLAYER_APPROACH_BASE,		
	COMMANDER_APPEAR,			
	COMMANDER_MESSAGE,			
	FINAL_FADE_OUT,				
	THANKS_MESSAGE,				
	RESET_TO_MAIN,				
	DONE						
};


void Ending_Initialize();
void Ending_Set_Position(float X, float Y, float W, float H);

void Ending_Finalize();

void Ending_Update(double elapsed_time);

void Ending_Draw();

#endif // ENDING_SCREEN_H
