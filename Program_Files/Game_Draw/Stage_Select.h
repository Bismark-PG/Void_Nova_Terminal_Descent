/*==============================================================================

	Draw Stage [Stage_Select.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include "Game_Window.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Enemy_Spawner.h"
#include "Effect.h"
#include "Game_Back.h"
#include "Fade.h"
#include "Game_Logic.h"
#include "Stage_Game_Over.h"
#include <DirectXMath.h> 
#include "Boss_Manager.h"

enum class NOW_PLAYING_STAGE
{
	STAGE_NONE = 0,
	STAGE_ONE,
	STAGE_TWO,
	STAGE_THREE,
	STAGE_FOUR,
	STAGE_FIVE,
	STAGE_MAX
};

enum class STAGE_FLOW_STATE
{
	IDLE,
	STORY_PLAYING,
	BOSS_ENTERING,
	PREPARE_GAME,
	GAMEPLAY_ACTIVE,
	BOSS_DEAD,
	GAME_PAUSED,
	PLAYER_DEAD,
	PLAYER_RESPAWNING,
	STAGE_DONE,
	CHECK_REMAINING,
	STAGE_CLEAR,
	STAGE_CLEAR_ANIME,
	STAGE_FAIL,
	GAME_OVER,
	PLAYER_EXIT,
	STAGE_PLAYING_DONE
};

static float Player_Spawn_X;
static float Player_Spawn_Y;
static float Player_Spawn_Speed = 150.0f;

void Stage_Initialize();
void Stage_Finalize();

void Stage_Update(double elapsed_time);
void Stage_Draw();

void Set_Now_Playing_Stage(NOW_PLAYING_STAGE Stage);
void Set_BGM_Current_Stage(NOW_PLAYING_STAGE Stage);
void Set_Power_Current_Stage(NOW_PLAYING_STAGE Stage);
NOW_PLAYING_STAGE Get_Now_Playing_Stage();

void  Set_Now_Stage_Flow(STAGE_FLOW_STATE State);
STAGE_FLOW_STATE  Get_Now_Stage_Flow();

bool Is_Boss_Story_Time();

void Stage_Command_Player_Move(const DirectX::XMFLOAT2& targetPos, float speed);
void Stage_Update_Player_Movement(double elapsed_time);
bool Stage_Is_Player_Moving_By_Script();

// True = Player Lock // False = Player Free
void Stage_Set_Is_Player_Moving_By_Script(bool Is_Move);

bool Are_All_Enemies_Cleared();

void Stage_Reset_For_Retry();

double Get_In_Game_Timer();

void Is_Stage_Paused();

#endif // STAGE_SELECT_H