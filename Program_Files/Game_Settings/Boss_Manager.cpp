/*==============================================================================

	Manage Boss Pattern [Boss_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Boss_Manager.h"
#include "Game_Mode_Manager.h"
#include "Enemy.h"
#include "Stage_Select.h" 
#include "Boss_Pattern.h"
#include "Item.h"

static BOSS_STATE Current_Boss_State;
static NOW_PLAYING_STAGE Current_Stage;
static BOSS_PATTERN_TYPE Current_Pattern;

static int Active_Boss_Index = -1;
static bool Is_Phase_Complete = false;
static bool Is_Entering_Finished = false;

void Boss_Initialize()
{
	Current_Boss_State = BOSS_STATE::INACTIVE;
	Current_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
	Current_Pattern = BOSS_PATTERN_TYPE::NONE;
	Active_Boss_Index = -1;
	Is_Phase_Complete = false;
	Is_Entering_Finished = false;
}

void Boss_Finalize()
{

}

void Boss_Activate(int bossIndex, NOW_PLAYING_STAGE stage)
{
	if (Current_Boss_State != BOSS_STATE::INACTIVE) return;

	Active_Boss_Index = bossIndex;
	Current_Stage = stage;
	Current_Boss_State = BOSS_STATE::ENTERING;
	Is_Phase_Complete = false;
	Is_Entering_Finished = false;
}

void Boss_Start_Pattern(BOSS_PATTERN_TYPE pattern)
{
	if (Active_Boss_Index == -1) return;
	
	Enemy* Boss = Enemy_Get_Editable(Active_Boss_Index);
	if (!Boss) return;

	Current_Pattern = pattern;
	Is_Phase_Complete = false;
	Current_Boss_State = BOSS_STATE::BATTLE;

	Boss->HP = Get_Enemy_Info(Boss->Type_ID).HP;
	Boss->State = Enemy_State::Attack;
	Boss_Pattern_Initialize(*Boss);
}

void Boss_Update(double elapsed_time)
{
	if (Current_Boss_State == BOSS_STATE::INACTIVE || Current_Boss_State == BOSS_STATE::DEFEATED)
		return;

	Enemy* Boss = Enemy_Get_Editable(Active_Boss_Index);
	if (!Boss)
	{
		if (Current_Boss_State != BOSS_STATE::INACTIVE)
		{
			Current_Boss_State = BOSS_STATE::DEFEATED;
			Is_Phase_Complete = true;
		}
		return;
	}

	switch (Current_Boss_State)
	{
	case BOSS_STATE::ENTERING:
		if (Boss_Pattern_Enter(*Boss, elapsed_time))
		{
			Is_Entering_Finished = true;
		}
		break;

	case BOSS_STATE::BATTLE:
		switch (Current_Stage)
		{
		case NOW_PLAYING_STAGE::STAGE_THREE:
			Boss_Pattern_Stage_3_Update(*Boss, elapsed_time, Current_Pattern);
			break;
		case NOW_PLAYING_STAGE::STAGE_FOUR:
			Boss_Pattern_Stage_4_Update(*Boss, elapsed_time, Current_Pattern);
			break;
		case NOW_PLAYING_STAGE::STAGE_FIVE:
			Boss_Pattern_Stage_5_Update(*Boss, elapsed_time, Current_Pattern);
			break;
		}

		if (Boss->State == Enemy_State::DESTRUCTION)
		{
			Is_Phase_Complete = true;
			Current_Boss_State = BOSS_STATE::PHASE_CHANGING;
			Sound_M->Play_SFX("Enemy_Mini_Boss_Dead");
		}
		break;

	case BOSS_STATE::PHASE_CHANGING:
		// Do nothing and wait for the next command from Stage_Update
		break;
	}
}

void Boss_Draw()
{
	// effect ??
}

BOSS_STATE Get_Boss_State()
{
	return Current_Boss_State;
}

void Set_Boss_State(BOSS_STATE state)
{
	Current_Boss_State = state;
}

bool Is_Current_Phase_Finished()
{
	if (Is_Phase_Complete)
	{
		Is_Phase_Complete = false;
		return true;
	}
	return false;
}

bool Is_Boss_Entering()
{
	return (Current_Boss_State == BOSS_STATE::ENTERING && !Is_Entering_Finished);
}

void Set_Boss_Entering_Done(bool Done)
{
	Is_Entering_Finished = Done;
}

bool Get_Boss_Entering_Done()
{
	return Is_Entering_Finished;
}

int Enemy_Get_Active_Boss_Index()
{
	return Active_Boss_Index;
}

BOSS_PATTERN_TYPE Boss_Get_Current_Pattern()
{
	return Current_Pattern;
}