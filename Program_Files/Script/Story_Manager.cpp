/*==============================================================================

	Story Manager [Story_Manager.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Story_Manager.h"
#include "Story_Script.h"
#include "Stage_Select.h"
#include "Boss_Manager.h" 
#include <Stage_Update.h>

struct StoryContext {
	NOW_PLAYING_STAGE Now_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
	STORY_TYPE Now_Story = STORY_TYPE::NONE;
	bool Is_Boss_Story = false;
	bool Is_Auto_Next = false;
	int Now_Page = 0;
	int Max_Page = 0;
}; 

static StoryContext Now_Story_Info;

static Story_Manager_State S_M_State;

void Story_Initialize()
{
	Script_Initialize();

	Now_Story_Info = {};
	Set_Story_State(Story_Manager_State::IN_ACTIVE);
}

void Story_Finalize()
{
	Script_Finalize();

	Now_Story_Info = {};
	Set_Story_State(Story_Manager_State::IN_ACTIVE);
}

void Story_Update(double elapsed_time)
{
	if (Get_Story_State() == Story_Manager_State::IN_ACTIVE || Get_Story_State() == Story_Manager_State::DONE)
		return;

	bool Page_Finished = Script_Update(elapsed_time, Now_Story_Info.Is_Auto_Next, Now_Story_Info.Is_Boss_Story);

	if (Page_Finished)
		Story_Force_Next_Page();

	if (Get_Story_State() == Story_Manager_State::OUTRO_PLAYING)
	{
		if (Is_Script_Outro_Finished())
		{
			Set_Story_State(Story_Manager_State::DONE);
		}
	}

	if (Get_Story_State() == Story_Manager_State::SCRIPT_PLAYING || Get_Story_State() == Story_Manager_State::OUTRO_PLAYING)
		Stage_Update_Player_Movement(elapsed_time);
}

void Story_Draw()
{
	if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
		return;

	Script_Animation_Draw(Now_Story_Info.Is_Boss_Story);

	if (Get_Story_State() == Story_Manager_State::SCRIPT_PLAYING && Get_Script_State() == SCRIPT_ANIMATION_STATE::TEXT_ACTIVE)
		Script_Text_Draw(Now_Story_Info.Now_Stage, Now_Story_Info.Now_Page, Now_Story_Info.Is_Boss_Story, Now_Story_Info.Now_Story);
}


void Story_Start(NOW_PLAYING_STAGE Stage, STORY_TYPE Story, bool IsAuto)
{
	Script_Reset();

	if (S_M_State != Story_Manager_State::IN_ACTIVE && S_M_State != Story_Manager_State::DONE) return;

	Now_Story_Info.Now_Stage = Stage;
	Now_Story_Info.Now_Story = Story;
	Now_Story_Info.Is_Auto_Next = IsAuto;
	Now_Story_Info.Is_Boss_Story = (Story == STORY_TYPE::INTRO_BOSS || Story == STORY_TYPE::OUTRO_FINAL_BOSS_LIE || Story == STORY_TYPE::OUTRO_FINAL_BOSS_TRUE);
	Now_Story_Info.Now_Page = 0;
	Now_Story_Info.Max_Page = 0;

	switch (Stage)
	{
	case NOW_PLAYING_STAGE::STAGE_ONE:
	{
		switch (Story)
		{
		case STORY_TYPE::INTRO_STAGE:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_1_Max;
			break;

		case STORY_TYPE::OUTRO_STAGE:
			// Now_Story_Info.Max_Page = Script_Page::Stage_Outro_1_Max;
			break;
		}
		break;
	}

	case NOW_PLAYING_STAGE::STAGE_TWO:
	{
		switch (Story)
		{
		case STORY_TYPE::INTRO_STAGE:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_2_Max;
			break;

		case STORY_TYPE::OUTRO_STAGE:
			// Now_Story_Info.Max_Page = Script_Page::Stage_Outro_2_Max;
			break;
		}
		break;
	}

	case NOW_PLAYING_STAGE::STAGE_THREE:
	{
		switch (Story)
		{
		case STORY_TYPE::INTRO_STAGE:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_3_Max;
			break;
		case STORY_TYPE::INTRO_BOSS:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_Boss_3_Max;
			break;
		case STORY_TYPE::OUTRO_STAGE:
			// Now_Story_Info.Max_Page = Script_Page::Stage_Outro_3_Max;
			break;
		}
		break;
	}

	case NOW_PLAYING_STAGE::STAGE_FOUR:
	{
		switch (Story)
		{
		case STORY_TYPE::INTRO_STAGE:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_4_Max;
			break;
		case STORY_TYPE::INTRO_BOSS:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_Boss_4_Max;
			break;
		case STORY_TYPE::OUTRO_STAGE:
			// Now_Story_Info.Max_Page = Script_Page::Stage_Outro_4_Max;
			break;
		}
		break;
	}

	case NOW_PLAYING_STAGE::STAGE_FIVE:
	{
		switch (Story)
		{
		case STORY_TYPE::INTRO_STAGE:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_5_Max;
			break;

		case STORY_TYPE::INTRO_BOSS:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_Boss_5_Phase_1_Max;
			break;

		case STORY_TYPE::OUTRO_FINAL_BOSS_LIE:
			Now_Story_Info.Max_Page = Script_Page::Stage_Intro_Boss_5_Phase_2_Max;
			break;

		case STORY_TYPE::OUTRO_FINAL_BOSS_TRUE:
			// Now_Story_Info.Max_Page = Script_Page::Stage_Outro_5_Max;
			break;
		}
		break;
	}
	}

	if (Now_Story_Info.Max_Page > 0)
	{
		Set_Story_State(Story_Manager_State::SCRIPT_PLAYING);
		Script_Begin(Now_Story_Info.Is_Auto_Next, Now_Story_Info.Is_Boss_Story);
	}
	else
	{
		Set_Story_State(Story_Manager_State::DONE);
	}
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

void Story_Reset()
{
	Set_Story_State(Story_Manager_State::IN_ACTIVE);
}

void Story_Force_Next_Page()
{
	Now_Story_Info.Now_Page++;

	if (Now_Story_Info.Now_Page >= Now_Story_Info.Max_Page)
	{
		Set_Story_State(Story_Manager_State::OUTRO_PLAYING);
		// Tell the Art Director to start the outro.
		Script_Start_Outro(Now_Story_Info.Is_Boss_Story);
	}
}