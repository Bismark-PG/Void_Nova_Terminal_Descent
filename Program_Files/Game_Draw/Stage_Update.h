/*==============================================================================

	Stage Update Manager [Stage_Update.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef STAGE_UPDATE_H
#define STAGE_UPDATE_H
#include "Boss_Manager.h"

enum class STAGE_PROGRESS
{
	SPAWNING,          
	TIME_LIMIT_REACHED,
	WAIT_FOR_CLEAR,    
	DONE               
};

enum class STAGE_3_STATE
{
	NONE,
	NORMAL_PHASE,
	BOSS_PREPARE,
	DONE
};

enum class STAGE_5_STATE
{
	NONE,
	NORMAL_PHASE,
	BOSS_PHASE_1_BATTLE,
	BOSS_DEFEATED,
	BOSS_APPEAR_STORY,
	BOSS_PHASE_1_INTRO,
	PHASE_CHANGE_STORY,
	BOSS_PHASE_2_INTRO,
	BOSS_PHASE_2_BATTLE,
	DONE
};

static bool Is_Stage_1_Spawned;
static bool Is_Stage_1_Enemy_Left;

static bool Is_Stage_2_Spawned;
static bool Is_Stage_2_Enemy_Left;

static bool Is_Stage_3_Spawned;
static bool Is_Stage_3_Enemy_Left;

static constexpr double STAGE_1_DONE_TIME = 110.0;

static constexpr double STAGE_2_DONE_TIME = 120.0;

static constexpr double STAGE_3_DONE_TIME = 35.0;
static constexpr double STAGE_3_BOSS_WAIT_TIME = 3.0;

void Stage_Timer_Reset();
void Stage_Timer_Update(double elapsed_time);
double Stage_Get_Timer();
double Stage_Get_Wait_Timer();

void Stage_Update_Reset();

void Stage_One_Update(double elapsed_time);
void Stage_Two_Update(double elapsed_time);
void Stage_Three_Update(double elapsed_time);

STAGE_3_STATE Get_Stage_3_State();
void Set_Stage_3_State(STAGE_3_STATE State);
STAGE_5_STATE Get_Stage_5_State();
void Set_Stage_5_State(STAGE_5_STATE State);

#endif // STAGE_UPDATE_H