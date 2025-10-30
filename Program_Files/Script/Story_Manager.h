/*==============================================================================

	Story Manager [Story_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Stage_Select.h"
#ifndef STORY_MANAGER_H
#define STORY_MANAGER_H

enum class STORY_TYPE
{
	NONE,
	INTRO_STAGE,
	INTRO_BOSS,
	OUTRO_FINAL_BOSS_LIE,
	OUTRO_FINAL_BOSS_TRUE,
	OUTRO_STAGE
};

enum Script_Page : int
{
	Stage_Intro_1_Max = 5,
	Stage_Intro_2_Max = 7,
	Stage_Intro_3_Max = 6,
	Stage_Intro_Boss_3_Max = 4,
	Stage_Intro_4_Max = 3,     
	Stage_Intro_Boss_4_Max = 5,
	Stage_Intro_5_Max = 2,
	Stage_Intro_Boss_5_Phase_1_Max = 5,
	Stage_Intro_Boss_5_Phase_2_Max = 6 
	// Stage_Outro_1_Max = 0,
	// Stage_Outro_2_Max = 0,
	// Stage_Outro_3_Max = 0,
	// Stage_Outro_4_Max = 0,
	// Stage_Outro_5_Max = 0
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

void Story_Start(NOW_PLAYING_STAGE Stage, STORY_TYPE Story, bool IsAuto);

Story_Manager_State Get_Story_State();
void Set_Story_State(Story_Manager_State State);

bool Story_Is_Finished();
void Story_Reset();

void Story_Force_Next_Page();

#endif // STORY_MANAGER_H