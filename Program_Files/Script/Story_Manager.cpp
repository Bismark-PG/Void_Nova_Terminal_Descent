/*==============================================================================

	Story Manager [Story_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Story_Manager.h"
#include "Story_Script.h"
#include "Stage_Select.h"
#include "Boss_Manager.h" 

static Story_Manager_State S_M_State;
static NOW_PLAYING_STAGE Now_Stage;
static int Now_Page = 0;
static int Max_Page = 0;

void Story_Initialize()
{
	Script_Initialize();

	Set_Story_State(Story_Manager_State::IN_ACTIVE);
	Now_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
}

void Story_Finalize()
{
	Script_Finalize();

	Set_Story_State(Story_Manager_State::IN_ACTIVE);
	Now_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
}

void Story_Update(double elapsed_time)
{
	if (Get_Story_State() == Story_Manager_State::IN_ACTIVE || Get_Story_State() == Story_Manager_State::DONE)
		return;

	if (Is_Boss_Story_Time())
		Script_Boss_Update(elapsed_time);
	else
	{
		bool Page_Finished = Script_Update(elapsed_time);
		if (Page_Finished)
		{
			Story_Force_Next_Page();
		}
	}

	if (Get_Story_State() == Story_Manager_State::SCRIPT_PLAYING)
	{
		Stage_Update_Player_Movement(elapsed_time);
	}
	else if (Get_Story_State() == Story_Manager_State::OUTRO_PLAYING)
	{
		if (Is_Script_Outro_Finished(Is_Boss_Story_Time()))
		{
			Set_Story_State(Story_Manager_State::DONE);
			Script_Reset();
			Script_Boss_Reset();
		}
	}
}

void Story_Draw()
{
	if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
		return;

	if (Get_Story_State() == Story_Manager_State::SCRIPT_PLAYING || Get_Story_State() == Story_Manager_State::OUTRO_PLAYING)
	{
		if (Is_Boss_Story_Time())
			Script_Boss_Animation_Draw();
		else
			Script_Animation_Draw();
	}

	if (Get_Story_State() == Story_Manager_State::SCRIPT_PLAYING)
		Script_Text_Draw(Now_Stage, Now_Page, Is_Boss_Story_Time());
}


void Story_Start(NOW_PLAYING_STAGE stage)
{
	if (S_M_State != Story_Manager_State::IN_ACTIVE) return;

	Now_Stage = stage;
	Now_Page = 0;

	if (Is_Boss_Story_Time())
	{
		switch (stage)
		{
		case NOW_PLAYING_STAGE::STAGE_THREE:
			Max_Page = Stage_3_Boss_Max;
			break;
		case NOW_PLAYING_STAGE::STAGE_FOUR:
			Max_Page = Stage_4_Boss_Max;
			break;
		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Max_Page = Stage_5_Boss_Phase_1_Max;
			break;
		default:
			Max_Page = 0;
			break;
		}
	}
	else
	{
		switch (stage)
		{
		case NOW_PLAYING_STAGE::STAGE_ONE:
			Max_Page = Stage_1_Max;
			break;

		case NOW_PLAYING_STAGE::STAGE_TWO:
			Max_Page = Stage_2_Max;
			break;

		case NOW_PLAYING_STAGE::STAGE_THREE:
			Max_Page = Stage_3_Max;
			break;

		case NOW_PLAYING_STAGE::STAGE_FOUR:
			Max_Page = Stage_4_Max;
			break;

		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Max_Page = Stage_5_Max;
			break;

		default:
			Max_Page = 0;
			break;
		}
	}


	if (Max_Page > 0)
	{
		Set_Story_State(Story_Manager_State::SCRIPT_PLAYING);
		if (Is_Boss_Story_Time())
			Script_Boss_Start();
		else
			Script_Start();
	}
	else
		Set_Story_State(Story_Manager_State::DONE);
}

Story_Manager_State Get_Story_State()
{
	return S_M_State;
}

void Set_Story_State(Story_Manager_State State)
{
	S_M_State = State;
}

bool Story_Is_Finished()
{
	return Get_Story_State() == Story_Manager_State::DONE;
}

void Story_Force_Next_Page()
{
	Now_Page++;

	if (Now_Page >= Max_Page)
	{
		Set_Story_State(Story_Manager_State::OUTRO_PLAYING);
		if (Is_Boss_Story_Time())
			Script_Boss_Start_Outro();
		else
			Script_Start_Outro();
	}
}