/*==============================================================================

	Manage Boss Pattern [Boss_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BOSS_MANAGER_H
#define BOSS_MANAGER_H

enum class NOW_PLAYING_STAGE;

enum class BOSS_STATE
{
	INACTIVE,
	ENTERING,      
	BATTLE,        
	PHASE_CHANGING,
	DEFEATED
};

enum class BOSS_PATTERN_TYPE
{
	NONE,
	STAGE_3_PHASE_1,
	STAGE_3_PHASE_2,
	STAGE_3_PHASE_3,
	STAGE_4_PHASE_1,
	STAGE_4_PHASE_2,
	STAGE_4_PHASE_3,
	STAGE_4_PHASE_4,
	STAGE_4_PHASE_5,
	STAGE_5_PHASE_1,
	STAGE_5_PHASE_2,
	STAGE_5_PHASE_3,
	STAGE_5_PHASE_4,
	STAGE_Final_PHASE_1,
	STAGE_Final_PHASE_2,
	STAGE_Final_PHASE_3,
};

void Boss_Initialize();
void Boss_Finalize();

void Boss_Activate(int bossIndex, NOW_PLAYING_STAGE stage);
void Boss_Start_Pattern(BOSS_PATTERN_TYPE pattern);

void Boss_Update(double elapsed_time);
void Boss_Draw();

BOSS_STATE Get_Boss_State();
void Set_Boss_State(BOSS_STATE state);

bool Is_Current_Phase_Finished();
bool Is_Boss_Entering();

void Set_Boss_Entering_Done(bool Done);
bool Get_Boss_Entering_Done();

int Enemy_Get_Active_Boss_Index();

BOSS_PATTERN_TYPE Boss_Get_Current_Pattern();

#endif // BOSS_MANAGER_H