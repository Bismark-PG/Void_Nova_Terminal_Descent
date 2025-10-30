/*==============================================================================

	Stage Update Manager [Stage_Update.h]

	Author : Choi HyungJoon

==============================================================================*/
#include <Boss_Manager.h>
#ifndef STAGE_UPDATE_H
#define STAGE_UPDATE_H

enum class STAGE_1_STATE
{
	NONE,
	CHECK_GAME_MODE,
	STAGE_STORY,
	WAIT_FOR_START,

	PHASE_NORMAL_1,
	PHASE_NORMAL_2,
	PHASE_NORMAL_3,

	PHASE_BOSS_1,

	STAGE_CLEAR,
	DONE
};

enum class STAGE_2_STATE
{
	NONE,
	CHECK_GAME_MODE,
	STAGE_STORY,
	WAIT_FOR_START,

	PHASE_NORMAL_1,

	PHASE_BOSS_1,

	PHASE_NORMAL_2,
	PHASE_NORMAL_3,

	PHASE_BOSS_2,

	STAGE_CLEAR,
	DONE
};

enum class STAGE_3_STATE
{
	NONE,
	CHECK_GAME_MODE,
	STAGE_STORY,
	WAIT_FOR_START,

	NORMAL_PHASE_1,

	BOSS_APPEARANCE,
	BOSS_ENTERING,
	BOSS_BATTLE_PREPARE,

	BOSS_PHASE_1_START,
	BOSS_PHASE_1_BATTLE,
	BOSS_PHASE_1_CLEAR,

	BOSS_PHASE_2_START,
	BOSS_PHASE_2_BATTLE,
	BOSS_PHASE_2_CLEAR,

	BOSS_PHASE_3_START,
	BOSS_PHASE_3_BATTLE,

	BOSS_DEFEATED,
	STAGE_CLEAR,
	DONE
};

enum class STAGE_4_STATE
{
	NONE,
	CHECK_GAME_MODE,
	STAGE_STORY,
	WAIT_FOR_START,

	BOSS_APPEARANCE,
	BOSS_ENTERING,
	BOSS_BATTLE_PREPARE,

	BOSS_PHASE_1_START,
	BOSS_PHASE_1_BATTLE,
	BOSS_PHASE_1_CLEAR,

	BOSS_PHASE_2_START,
	BOSS_PHASE_2_BATTLE,
	BOSS_PHASE_2_CLEAR,

	BOSS_PHASE_3_START,
	BOSS_PHASE_3_BATTLE,
	BOSS_PHASE_3_CLEAR,

	BOSS_PHASE_4_START,
	BOSS_PHASE_4_BATTLE,
	BOSS_PHASE_4_CLEAR,

	BOSS_PHASE_5_START,
	BOSS_PHASE_5_BATTLE,

	BOSS_DEFEATED,
	STAGE_CLEAR,
	DONE
};

enum class STAGE_5_STATE
{
	NONE,
	CHECK_GAME_MODE,
	STAGE_STORY,
	WAIT_FOR_START,

	BOSS_APPEARANCE,
	BOSS_ENTERING,
	BOSS_BATTLE_PREPARE,

	BOSS_PHASE_1_START,
	BOSS_PHASE_1_BATTLE,
	BOSS_PHASE_1_CLEAR,

	BOSS_PHASE_2_START,
	BOSS_PHASE_2_BATTLE,
	BOSS_PHASE_2_CLEAR,

	BOSS_PHASE_3_START,
	BOSS_PHASE_3_BATTLE,
	BOSS_PHASE_3_CLEAR,

	BOSS_PHASE_4_START,
	BOSS_PHASE_4_BATTLE,
	BOSS_PHASE_4_CLEAR,

	FAKE_CLEAR_SEQUENCE,      
	BOSS_SWAP,                

	BOSS_FINAL_PHASE_ENTERING,
	BOSS_FINAL_PREPARE,
	PREPARE_FINAL_BATTLE,     

	BOSS_PHASE_5_START,
	BOSS_PHASE_5_BATTLE,
	BOSS_PHASE_5_CLEAR,

	BOSS_PHASE_6_START,
	BOSS_PHASE_6_BATTLE,
	BOSS_PHASE_6_CLEAR,

	BOSS_PHASE_7_START,
	BOSS_PHASE_7_BATTLE,

	BOSS_DEFEATED,
	STAGE_CLEAR,
	DONE
};

static bool Stage_1_Phase_1_Spawned = false;
static bool Stage_1_Phase_2_Spawned = false;
static bool Stage_1_Phase_B_Spawned = false;
static bool Stage_1_Phase_3_Spawned = false;

static bool Stage_2_Phase_1_Spawned  = false;
static bool Stage_2_Phase_B1_Spawned = false;
static bool Stage_2_Phase_2_Spawned  = false;
static bool Stage_2_Phase_B2_Spawned = false;

static bool Stage_3_Normal_1_Spawned = false;
static bool Stage_3_Boss_Spawned = false;

static bool Stage_4_Boss_Spawned = false;
static bool Stage_5_Boss_Spawned = false;
static bool Stage_Final_Boss_Spawned = false;

constexpr double CHANGE_TO_BOSS_FREEZE_TIME = 5.0;
constexpr double PHASE_CHANGE_FREEZE_TIME = 2.0;
constexpr double FINAL_PHASE_FREEZE_TIME = 8.0;
constexpr double STAGE_CLEAR_FREEZE_TIME = 5.0;

static bool Is_Story_Played_S1 = false;
static bool Is_Story_Played_S2 = false;
static bool Is_Story_Played_S3 = false;
static bool Is_Story_Played_S4 = false;
static bool Is_Story_Played_S5 = false;
static bool Is_Boss_Story_Played_S3 = false;
static bool Is_Boss_Story_Played_S4 = false;
static bool Is_Boss_Story_Played_S5_P1 = false;
static bool Is_Boss_Story_Played_S5_P2 = false;

static bool Story_Trigger_Saver_S1 = false;
static bool Story_Trigger_Saver_S2 = false;
static bool Story_Trigger_Saver_S3 = false;
static bool Story_Trigger_Saver_S4 = false;
static bool Story_Trigger_Saver_S5 = false;
static bool Boss_Story_Trigger_Saver_S3 = false;
static bool Boss_Story_Trigger_Saver_S4 = false;
static bool Boss_Story_Trigger_Saver_S5_P1 = false;
static bool Boss_Story_Trigger_Saver_S5_P2 = false;

static bool Is_Item_Drop = false;

void Stage_Timer_Reset();
void Stage_Timer_Update(double elapsed_time);
double Stage_Get_Timer();
double Stage_Get_Wait_Timer();

void Stage_Update_Reset();

void Stage_One_Update(double elapsed_time);
void Stage_Two_Update(double elapsed_time);
void Stage_Three_Update(double elapsed_time);
void Stage_Four_Update(double elapsed_time);
void Stage_Five_Update(double elapsed_time);

void Stage_Updater_Phase_Reset();

STAGE_1_STATE Get_Stage_1_State();
void Set_Stage_1_State(STAGE_1_STATE State);
STAGE_2_STATE Get_Stage_2_State();
void Set_Stage_2_State(STAGE_2_STATE State);
STAGE_3_STATE Get_Stage_3_State();
void Set_Stage_3_State(STAGE_3_STATE State);
STAGE_4_STATE Get_Stage_4_State();
void Set_Stage_4_State(STAGE_4_STATE State);
STAGE_5_STATE Get_Stage_5_State();
void Set_Stage_5_State(STAGE_5_STATE State);
bool Is_Final_Boss_Entered();

BOSS_PATTERN_TYPE Get_Next_Boss_Pattern(BOSS_PATTERN_TYPE current_pattern);

void Item_Drop_Start(const Enemy* Index);
bool Is_Item_Drop_Done();
void Is_Item_Drop_Reset();

void Story_Trigger_Saver();
void Story_Trigger_Loader();

#endif // STAGE_UPDATE_H