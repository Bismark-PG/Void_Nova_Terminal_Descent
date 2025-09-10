/*==============================================================================

	Story Manager [Story_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Stage_Select.h"
#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

enum Script_Page : int
{
	Stage_1_Max = 5,
	Stage_2_Max = 7,
	Stage_3_Max = 6,
	Stage_3_Boss_Max = 4,
	Stage_4_Max = 3,     
	Stage_4_Boss_Max = 5,
	Stage_5_Max = 2,
	Stage_5_Boss_Phase_1_Max = 5,
	Stage_5_Boss_Phase_2_Max = 1 
};

enum class Story_Manager_State
{
	IN_ACTIVE,
	SCRIPT_PLAYING,
	OUTRO_PLAYING,
	DONE
};

void Story_Initialize();
void Story_Finalize();
void Story_Update(double elapsed_time);
void Story_Draw();

void Story_Start(NOW_PLAYING_STAGE stage);

Story_Manager_State Get_Story_State();
void Set_Story_State(Story_Manager_State State);

bool Story_Is_Finished();

void Story_Force_Next_Page();

#endif // STORY_MANAGER_H