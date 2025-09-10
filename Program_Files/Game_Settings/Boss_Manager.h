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
	STORY,          
	BATTLE,         
	DEATH_ANIMATION,
	DEFEATED        
};

void Boss_Initialize();
void Boss_Finalize();

void Boss_Activate(NOW_PLAYING_STAGE stage);

void Boss_Update(double elapsed_time);
void Boss_Draw();

BOSS_STATE Get_Boss_State();
void Set_Boss_State(BOSS_STATE state);
bool Is_Boss_Defeated();

int Enemy_Get_Active_Boss_Index();
void Set_Active_Boss_Index(int index);

#endif // BOSS_MANAGER_H