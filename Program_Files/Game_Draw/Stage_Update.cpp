/*==============================================================================

	Stage Update Manager [Stage_Update.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Enemy_Spawner.h"
#include "Stage_Update.h"
#include "Stage_Select.h"
#include "Enemy.h"
#include "Game_Back.h"
#include "sprite.h"
#include "Texture_Manager.h"
#include "Palette.h"
#include "Save_Data.h"
#include "Game_Select.h"
#include "Game_Mode_Manager.h"
#include "Story_Script.h"
#include "Story_Manager.h"
#include "Boss_Manager.h"
#include <Item.h>
using namespace DirectX;

static STAGE_1_STATE Stage_1_State = STAGE_1_STATE::NONE;
static STAGE_2_STATE Stage_2_State = STAGE_2_STATE::NONE;
static STAGE_3_STATE Stage_3_State = STAGE_3_STATE::NONE;
static STAGE_4_STATE Stage_4_State = STAGE_4_STATE::NONE;
static STAGE_5_STATE Stage_5_State = STAGE_5_STATE::NONE;

static XMFLOAT2 Player;
static double Stage_Timer = 0.0;
static double Wait_Timer = 0.0;

void Stage_Timer_Reset()
{
	Stage_Timer = 0.0;
	Wait_Timer  = 0.0;
}

void Stage_Timer_Update(double elapsed_time)
{
	Stage_Timer += elapsed_time;
}

double Stage_Get_Timer()
{
	return Stage_Timer;
}

double Stage_Get_Wait_Timer()
{
	return Wait_Timer;
}

void Stage_Update_Reset()
{
	Player.x = Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH * A_Half);
	Player.y = static_cast<float>(SCREEN_HEIGHT) - (PLAYER_HEIGHT * A_One_And_Half);

	Stage_1_Phase_1_Spawned = false;
	Stage_1_Phase_2_Spawned = false;
	Stage_1_Phase_B_Spawned = false;
	Stage_1_Phase_3_Spawned = false;

	Stage_2_Phase_1_Spawned = false;
	Stage_2_Phase_B1_Spawned = false;
	Stage_2_Phase_2_Spawned = false;
	Stage_2_Phase_B2_Spawned = false;

	Stage_3_Normal_1_Spawned = false;
	Stage_3_Boss_Spawned = false;

	Stage_4_Boss_Spawned = false;
	Stage_5_Boss_Spawned = false;
	Stage_Final_Boss_Spawned = false;

	Wait_Timer = 0.0;

	Is_Item_Drop = false;

	Set_Stage_1_State(STAGE_1_STATE::NONE);
	Set_Stage_2_State(STAGE_2_STATE::NONE);
	Set_Stage_3_State(STAGE_3_STATE::NONE);
	Set_Stage_4_State(STAGE_4_STATE::NONE);
	Set_Stage_5_State(STAGE_5_STATE::NONE);

	Is_Story_Played_S1 = false;
	Is_Story_Played_S2 = false;
	Is_Story_Played_S3 = false;
	Is_Story_Played_S4 = false;
	Is_Story_Played_S5 = false;

	Is_Boss_Story_Played_S3 = false;
	Is_Boss_Story_Played_S4 = false;
	Is_Boss_Story_Played_S5_P1 = false;
	Is_Boss_Story_Played_S5_P2 = false;
}


void Stage_One_Update(double elapsed_time)
{
	PATTERN_DATA Data, Data_R, Data_L;
	Data_L.Patrol_Direction = Patrol_Direction::Left;
	Data_R.Patrol_Direction = Patrol_Direction::Right;

	static GameMode Now_Game_Mode = Get_Game_Mode();
	
	switch (Stage_1_State)
	{
	case STAGE_1_STATE::NONE:
		Stage_Updater_Phase_Reset();
		Set_Stage_1_State(STAGE_1_STATE::CHECK_GAME_MODE);
		break;

	case STAGE_1_STATE::CHECK_GAME_MODE:
		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Story_Played_S1)
		{
			if (Is_Story_Played_S1)
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
				Set_Stage_1_State(STAGE_1_STATE::PHASE_NORMAL_1);
			}
			else
			{
				Set_Stage_1_State(STAGE_1_STATE::STAGE_STORY);
			}
		}
		else
		{
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_1_State(STAGE_1_STATE::PHASE_NORMAL_1);
		}
		break;

	case STAGE_1_STATE::STAGE_STORY:
		Story_Update(elapsed_time);

		if (Story_Is_Finished())
		{
			Set_Stage_1_State(STAGE_1_STATE::WAIT_FOR_START);
			Wait_Timer = 0.0;
			Is_Story_Played_S1 = true;
		}

		if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
		{
			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_START_POS), Player_Spawn_Speed);
			Story_Start(NOW_PLAYING_STAGE::STAGE_ONE, STORY_TYPE::INTRO_STAGE, false);
		}
		break;

	case STAGE_1_STATE::WAIT_FOR_START:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > 0.1)
		{
			Stage_Updater_Phase_Reset();
			Stage_Ready_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_1_State(STAGE_1_STATE::PHASE_NORMAL_1);
		}
		break;

	case STAGE_1_STATE::PHASE_NORMAL_1:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_1_Phase_1_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Three) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 3.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Seven) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 3.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Up, Data);

			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 20.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data_L);
			Enemy_Spawner_Create(Enemy_Type_Origin, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Origin, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin, false) }, 20.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 20.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data_R);

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 32.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 32.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Data.Target_Y = Game_Screen_Height * A_P_Three_Five;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 37.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 37.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Data.Target_Y = Game_Screen_Height * A_Half;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 42.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 42.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Stage_1_Phase_1_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			Set_Stage_1_State(STAGE_1_STATE::PHASE_NORMAL_2);
		break;

	case STAGE_1_STATE::PHASE_NORMAL_2:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_1_Phase_2_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Armor_Give_HP, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Armor_Give_HP, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Armor_Give_HP, false) }, 2.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data);
			Enemy_Spawner_Create(Enemy_Type_Armor_Give_HP, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Armor_Give_HP, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Armor_Give_HP, false) }, 2.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 12.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 12.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Half;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 22.0, 5.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 22.0, 5.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Stage_1_Phase_2_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			Set_Stage_1_State(STAGE_1_STATE::PHASE_NORMAL_3);
		break;

	case STAGE_1_STATE::PHASE_NORMAL_3:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_1_Phase_3_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Origin, { Game_Offset.x + (Game_Screen_Width * A_Point_Three) - Enemy_Get_Width(Enemy_Type_Origin, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin, false) }, 7.0, 15.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin, { Game_Offset.x + (Game_Screen_Width * A_Point_Seven) - Enemy_Get_Width(Enemy_Type_Origin, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin, false) }, 7.0, 15.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);

			Data.Target_Y = Game_Screen_Height * A_Two_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 15.0, 15.0, 4, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 15.0, 15.0, 4, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 30.5, 12.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 30.5, 12.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Saber, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Saber, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber, false) }, 40.0, 10.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Saber, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Saber, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber, false) }, 40.0, 10.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);

			Data.Target_Y = Game_Screen_Height * A_Two_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO_ITEM, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO_ITEM, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO_ITEM, false) }, 45.0, 10.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Stage_1_Phase_3_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			Set_Stage_1_State(STAGE_1_STATE::PHASE_BOSS_1);
		break;

	case STAGE_1_STATE::PHASE_BOSS_1:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_1_Phase_B_Spawned)
		{
			Stage_Updater_Phase_Reset();

			PATTERN_DATA Data;
			Data.Target_Y = Game_Screen_Height * A_Point_Fifteen;
			Enemy_Spawner_Create(Enemy_Type_Armor_Boss, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Armor_Boss, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Armor_Boss, false) }, 0.0, 1.0, 1, Enemy_Move_Pattern::Down_Fixed, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 5.0, 12.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 5.0, 12.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 5.0, 12.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 5.0, 12.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Stage_1_Phase_B_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
		{
			Wait_Timer = 0.0;
			Set_Stage_1_State(STAGE_1_STATE::STAGE_CLEAR);
		}
		break;

	case STAGE_1_STATE::STAGE_CLEAR:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > STAGE_CLEAR_FREEZE_TIME)
		{
			GameMode Now_Mode = Get_Game_Mode();
			if (Now_Mode == GameMode::NEW_GAME || Now_Mode == GameMode::CONTINUE)
				Save_Data_Update_Stage(static_cast<int>(NOW_PLAYING_STAGE::STAGE_ONE));

			Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
			Set_Stage_1_State(STAGE_1_STATE::DONE);
		}
		break;

	case STAGE_1_STATE::DONE:
		break;
	}
}

void Stage_Two_Update(double elapsed_time)
{
	PATTERN_DATA Data, Data_R, Data_L;
	Data_L.Patrol_Direction = Patrol_Direction::Left;
	Data_R.Patrol_Direction = Patrol_Direction::Right;

	static GameMode Now_Game_Mode = Get_Game_Mode();

	switch (Stage_2_State)
	{
	case STAGE_2_STATE::NONE:
		Stage_Updater_Phase_Reset();
		Set_Stage_2_State(STAGE_2_STATE::CHECK_GAME_MODE);
		break;


	case STAGE_2_STATE::CHECK_GAME_MODE:
		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Story_Played_S2)
		{
			if (Is_Story_Played_S2)
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
				Set_Stage_2_State(STAGE_2_STATE::PHASE_NORMAL_1);
			}
			else
			{
				Set_Stage_2_State(STAGE_2_STATE::STAGE_STORY);
			}
		}
		else
		{
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_2_State(STAGE_2_STATE::PHASE_NORMAL_1);
		}
		break;

	case STAGE_2_STATE::STAGE_STORY:
		Story_Update(elapsed_time);

		if (Story_Is_Finished())
		{
			Set_Stage_2_State(STAGE_2_STATE::WAIT_FOR_START);
			Wait_Timer = 0.0;
			Is_Story_Played_S2 = true;
		}

		if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
		{
			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_START_POS), Player_Spawn_Speed);
			Story_Start(NOW_PLAYING_STAGE::STAGE_TWO, STORY_TYPE::INTRO_STAGE, false);
		}
		break;

	case STAGE_2_STATE::WAIT_FOR_START:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > 0.1)
		{
			Stage_Updater_Phase_Reset();
			Stage_Ready_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_2_State(STAGE_2_STATE::PHASE_NORMAL_1);
		}
		break;

	case STAGE_2_STATE::PHASE_NORMAL_1:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_2_Phase_1_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 3.0, 8.0, 2, Enemy_Move_Pattern::Down_Patrol, Data_L);
			Enemy_Spawner_Create(Enemy_Type_Origin, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Origin, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin, false) }, 5.0, 7.0, 3, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 3.0, 8.0, 2, Enemy_Move_Pattern::Down_Patrol, Data_R);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 17.0, 3.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 17.0, 3.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Stage_2_Phase_1_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			Set_Stage_2_State(STAGE_2_STATE::PHASE_BOSS_1);
		break;

	case STAGE_2_STATE::PHASE_BOSS_1:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_2_Phase_B1_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Data.Target_Y = Game_Screen_Height * A_Third; // Mini BOSS
			Enemy_Spawner_Create(Enemy_Type_Mini_Boss, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Mini_Boss, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Mini_Boss, false) }, 30.0, 0.5, 1, Enemy_Move_Pattern::Down_Fixed, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 28.0, 2.0, 6, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 28.0, 2.0, 6, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Stage_2_Phase_B1_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			Set_Stage_2_State(STAGE_2_STATE::PHASE_NORMAL_2);
		break;

	case STAGE_2_STATE::PHASE_NORMAL_2:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_2_Phase_2_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 40.0, 8.0, 2, Enemy_Move_Pattern::Down_Shoot_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Three) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 42.0, 8.0, 2, Enemy_Move_Pattern::Down_Shoot_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Seven) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 42.0, 8.0, 2, Enemy_Move_Pattern::Down_Shoot_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 40.0, 8.0, 2, Enemy_Move_Pattern::Down_Shoot_Up, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 60.0, 8.0, 1, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 60.0, 8.0, 1, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 61.0, 8.0, 2, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 61.0, 8.0, 2, Enemy_Move_Pattern::Down_And_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Three_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 62.0, 8.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Six_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 62.0, 8.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Point_Fifteen;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
											-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 63.0, 8.0, 2, Enemy_Move_Pattern::Down_And_Down, Data);
			Stage_2_Phase_2_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			Set_Stage_2_State(STAGE_2_STATE::PHASE_BOSS_2);
		break;

	case STAGE_2_STATE::PHASE_BOSS_2:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_2_Phase_B2_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Data.Target_Y = Game_Screen_Height * A_Quarter; // BOSS
			Enemy_Spawner_Create(Enemy_Type_Armor_Boss, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Armor_Boss, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Armor_Boss, false) }, 75.0, 0.5, 1, Enemy_Move_Pattern::Down_Fixed, Data);

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 78.0, 7.0, 3, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 78.0, 7.0, 3, Enemy_Move_Pattern::Down_And_Down, Data);
			Data.Target_Y = Game_Screen_Height * A_P_Three_Five;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 80.0, 7.0, 3, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 80.0, 7.0, 3, Enemy_Move_Pattern::Down_And_Down, Data);
			Data.Target_Y = Game_Screen_Height * A_Half;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 82.0, 7.0, 3, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 82.0, 7.0, 3, Enemy_Move_Pattern::Down_And_Down, Data);
			Stage_2_Phase_B2_Spawned = true;
		}

		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
		{
			Wait_Timer = 0.0;
			Set_Stage_2_State(STAGE_2_STATE::STAGE_CLEAR);
		}
		break;

	case STAGE_2_STATE::STAGE_CLEAR:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > STAGE_CLEAR_FREEZE_TIME)
		{
			GameMode Now_Mode = Get_Game_Mode();

			if (Now_Mode == GameMode::NEW_GAME || Now_Mode == GameMode::CONTINUE)
				Save_Data_Update_Stage(static_cast<int>(NOW_PLAYING_STAGE::STAGE_TWO));

			Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
			Set_Stage_2_State(STAGE_2_STATE::DONE);
		}
		break;

	case STAGE_2_STATE::DONE:
		break;
	}
}

void Stage_Three_Update(double elapsed_time)
{
	PATTERN_DATA Data, Data_R, Data_L;
	Data_L.Patrol_Direction = Patrol_Direction::Left;
	Data_R.Patrol_Direction = Patrol_Direction::Right;

	static GameMode Now_Game_Mode = Get_Game_Mode();

	switch (Stage_3_State)
	{
	case STAGE_3_STATE::NONE:
		Stage_Updater_Phase_Reset();
		Set_Stage_3_State(STAGE_3_STATE::CHECK_GAME_MODE);
		break;

	case STAGE_3_STATE::CHECK_GAME_MODE:
		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Story_Played_S3)
		{
			if (Is_Story_Played_S3)
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
				Set_Stage_3_State(STAGE_3_STATE::NORMAL_PHASE_1);
			}
			else
			{
				Set_Stage_3_State(STAGE_3_STATE::STAGE_STORY);
			}
		}
		else
		{
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_3_State(STAGE_3_STATE::NORMAL_PHASE_1);
		}
		break;

	case STAGE_3_STATE::STAGE_STORY:
		Story_Update(elapsed_time);

		if (Story_Is_Finished())
		{
			Set_Stage_3_State(STAGE_3_STATE::WAIT_FOR_START);
			Wait_Timer = 0.0;
			Is_Story_Played_S3 = true;
		}

		if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
		{
			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_START_POS), Player_Spawn_Speed);
			Story_Start(NOW_PLAYING_STAGE::STAGE_THREE, STORY_TYPE::INTRO_STAGE, false);
		}
		break;

	case STAGE_3_STATE::WAIT_FOR_START:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > 0.1)
		{
			Stage_Updater_Phase_Reset();
			Stage_Ready_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_3_State(STAGE_3_STATE::NORMAL_PHASE_1);
		}
		break;

	case STAGE_3_STATE::NORMAL_PHASE_1:
		Enemy_Spawner_Update(elapsed_time);

		if (!Stage_3_Normal_1_Spawned)
		{
			Stage_Updater_Phase_Reset();

			Data.Target_Y = Game_Screen_Height * A_Half;
			Enemy_Spawner_Create(Enemy_Type_Saber_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Saber_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber_ZAKO, false) }, 3.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Patrol, Data);
			Enemy_Spawner_Create(Enemy_Type_Saber_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Saber_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber_ZAKO, false) }, 3.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Patrol, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Saber_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Three) - Enemy_Get_Width(Enemy_Type_Saber_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber_ZAKO, false) }, 5.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Patrol, Data);
			Enemy_Spawner_Create(Enemy_Type_Saber_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Seven) - Enemy_Get_Width(Enemy_Type_Saber_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber_ZAKO, false) }, 5.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Patrol, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Saber_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Four_Five) - Enemy_Get_Width(Enemy_Type_Saber_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber_ZAKO, false) }, 7.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Patrol, Data);
			Enemy_Spawner_Create(Enemy_Type_Saber_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Half_Half) - Enemy_Get_Width(Enemy_Type_Saber_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber_ZAKO, false) }, 7.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Patrol, Data);

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 15.0, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Three) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 15.5, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Four_Five) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 16.0, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Six_Tenths) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 16.5, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 17.0, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Nine) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 17.5, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 25.0, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Seven) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 25.5, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Half_Half) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 26.0, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Two_Fifth) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 26.5, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 27.0, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_One_Tenth) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 27.5, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Stage_3_Normal_1_Spawned = true;
		}
	
		if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
		{
			Wait_Timer = 0.0;
			Set_Stage_3_State(STAGE_3_STATE::BOSS_APPEARANCE);
		}
		break;

	case STAGE_3_STATE::BOSS_APPEARANCE:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > CHANGE_TO_BOSS_FREEZE_TIME)
		{
			if (!Stage_3_Boss_Spawned)
			{
				Stage_Updater_Phase_Reset();
				Set_BGM_For_Boss(NOW_PLAYING_STAGE::STAGE_THREE);

				XMFLOAT2 Boss_Target_POS = {
					(Game_Offset.x + (Game_Screen_Width * A_Half) - (Enemy_Get_Width(Enemy_Type_Special_Boss_Stage_3, true) * A_Half)),
					Game_Offset.y + (Enemy_Get_Width(Enemy_Type_Special_Boss_Stage_3, false) * A_Point_Fifteen)
				};

				int Boss_Index = Enemy_Create(Enemy_Type_Special_Boss_Stage_3,
					{ Boss_Target_POS.x, -Enemy_Get_Width(Enemy_Type_Special_Boss_Stage_3, false) },
					Enemy_Move_Pattern::NONE, {}, Boss_Target_POS
				);

				Boss_Activate(Boss_Index, NOW_PLAYING_STAGE::STAGE_THREE);
				Stage_3_Boss_Spawned = true;

				Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_3_BOSS_START_POS), Player_Spawn_Speed);

				Story_Reset();
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_ENTERING);
				Set_Stage_3_State(STAGE_3_STATE::BOSS_ENTERING);
			}
		}
		break;

	case STAGE_3_STATE::BOSS_ENTERING:
	{
		bool Is_Entering_Done = !Is_Boss_Entering();
		bool Is_Story_Done = true;

		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Boss_Story_Played_S3)
		{
			if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
				Story_Start(NOW_PLAYING_STAGE::STAGE_THREE, STORY_TYPE::INTRO_BOSS, true);
	
			Is_Story_Done = Story_Is_Finished();
		}

		if (Is_Entering_Done && Is_Story_Done)
		{
			if (Now_Game_Mode != GameMode::STAGE_SELECT)
				Is_Boss_Story_Played_S3 = true;

			Set_Stage_3_State(STAGE_3_STATE::BOSS_BATTLE_PREPARE);
		}
	}
	break;

	case STAGE_3_STATE::BOSS_BATTLE_PREPARE:
		Story_Reset();
		Stage_Ready_Reset();
		Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
		Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_1_START);
		break;

		// --- PHASE 1 ---
	case STAGE_3_STATE::BOSS_PHASE_1_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_3_PHASE_1);
		Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_1_BATTLE);
		break;

	case STAGE_3_STATE::BOSS_PHASE_1_BATTLE:
		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_1_CLEAR);
		}
		break;

	case STAGE_3_STATE::BOSS_PHASE_1_CLEAR:
	{
		Wait_Timer += elapsed_time;
		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_3_PHASE_2)
				Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_2_START);
		}
	}
		break;

		// --- PHASE 2 ---
	case STAGE_3_STATE::BOSS_PHASE_2_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_3_PHASE_2);
		Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_2_BATTLE);
		break;

	case STAGE_3_STATE::BOSS_PHASE_2_BATTLE:
		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_2_CLEAR);
		}
		break;

	case STAGE_3_STATE::BOSS_PHASE_2_CLEAR:
	{
		Wait_Timer += elapsed_time;
		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_3_PHASE_3)
				Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_3_START);
		}
	}
		break;

		// --- PHASE FINAL ---
	case STAGE_3_STATE::BOSS_PHASE_3_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_3_PHASE_3);
		Set_Stage_3_State(STAGE_3_STATE::BOSS_PHASE_3_BATTLE);
		break;

	case STAGE_3_STATE::BOSS_PHASE_3_BATTLE:
	{
		if (Is_Current_Phase_Finished())
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			if (Next_Pattern == BOSS_PATTERN_TYPE::NONE)
				Set_Stage_3_State(STAGE_3_STATE::BOSS_DEFEATED);
		}
	}
		break;

	case STAGE_3_STATE::BOSS_DEFEATED:
	{
		Sound_M->Play_SFX("Enemy_Boss_Dead");
		int Index = Enemy_Get_Active_Boss_Index();

		if (Index != -1)
		{
			const Enemy* BOSS = Enemy_Get(Index);

			if (!Is_Item_Drop_Done())
			{
				Item_Drop_Start(BOSS);
			}

			Effect_Create(Effect_Type::REAL_EXPLOSION, BOSS->Position, { BOSS->Size.x, BOSS->Size.x });
			Enemy_Destroy(Index);
		}

		Wait_Timer = 0.0;
		Set_Stage_3_State(STAGE_3_STATE::STAGE_CLEAR);
	}
		break;


	case STAGE_3_STATE::STAGE_CLEAR:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > STAGE_CLEAR_FREEZE_TIME)
		{
			Is_Item_Drop_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
			Set_Stage_3_State(STAGE_3_STATE::DONE);
		}
		break;

	case STAGE_3_STATE::DONE:
		break;
	}
}

void Stage_Four_Update(double elapsed_time)
{
	PATTERN_DATA Data, Data_R, Data_L;
	Data_L.Patrol_Direction = Patrol_Direction::Left;
	Data_R.Patrol_Direction = Patrol_Direction::Right;

	static GameMode Now_Game_Mode = Get_Game_Mode();

	switch (Stage_4_State)
	{
	case STAGE_4_STATE::NONE:
		Stage_Updater_Phase_Reset();
		Set_Stage_4_State(STAGE_4_STATE::CHECK_GAME_MODE);
		break;

	case STAGE_4_STATE::CHECK_GAME_MODE:
		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Story_Played_S4)
		{
			if (Is_Story_Played_S4)
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
				Set_Stage_4_State(STAGE_4_STATE::BOSS_APPEARANCE);
			}
			else
			{
				Set_Stage_4_State(STAGE_4_STATE::STAGE_STORY);
			}
		}
		else
		{
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_4_State(STAGE_4_STATE::BOSS_APPEARANCE);
		}
		break;

	case STAGE_4_STATE::STAGE_STORY:
		if (Story_Is_Finished())
		{
			Set_Stage_4_State(STAGE_4_STATE::WAIT_FOR_START);
			Wait_Timer = 0.0;
			Is_Story_Played_S4 = true;
		}

		if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
		{
			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_START_POS), Player_Spawn_Speed);
			Story_Start(NOW_PLAYING_STAGE::STAGE_FOUR, STORY_TYPE::INTRO_STAGE, false);
		}
		break;

	case STAGE_4_STATE::WAIT_FOR_START:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > 0.1)
		{
			Stage_Updater_Phase_Reset();
			Stage_Ready_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_4_State(STAGE_4_STATE::BOSS_APPEARANCE);
		}
		break;

	case STAGE_4_STATE::BOSS_APPEARANCE:
		if (!Stage_4_Boss_Spawned)
		{
			Stage_Updater_Phase_Reset();
			Set_BGM_For_Boss(NOW_PLAYING_STAGE::STAGE_FOUR);

			XMFLOAT2 Boss_Target_POS = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (Enemy_Get_Width(Enemy_Type_Middle_Boss_Stage_4, true) * A_Half)),
				Game_Offset.y + (Enemy_Get_Width(Enemy_Type_Middle_Boss_Stage_4, false) * A_Point_Fifteen) };

			int Boss_Index = Enemy_Create(Enemy_Type_Middle_Boss_Stage_4, { Boss_Target_POS.x, -Enemy_Get_Width(Enemy_Type_Middle_Boss_Stage_4, false) },
				Enemy_Move_Pattern::NONE, {}, Boss_Target_POS);

			Boss_Activate(Boss_Index, NOW_PLAYING_STAGE::STAGE_FOUR);
			Stage_4_Boss_Spawned = true;

			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_4_BOSS_START_POS), Player_Spawn_Speed);

			Story_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_ENTERING);
			Set_Stage_4_State(STAGE_4_STATE::BOSS_ENTERING);
		}
		break;

	case STAGE_4_STATE::BOSS_ENTERING:
	{
		bool Is_Entering_Done = !Is_Boss_Entering();
		bool Is_Story_Done = true;

		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Boss_Story_Played_S4)
		{
			if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
				Story_Start(NOW_PLAYING_STAGE::STAGE_FOUR, STORY_TYPE::INTRO_BOSS, true);
			Is_Story_Done = Story_Is_Finished();
		}

		if (Is_Entering_Done && Is_Story_Done)
		{
			if (Now_Game_Mode != GameMode::STAGE_SELECT)
				Is_Boss_Story_Played_S4 = true;

			Set_Stage_4_State(STAGE_4_STATE::BOSS_BATTLE_PREPARE);
		}
	}
	break;

	case STAGE_4_STATE::BOSS_BATTLE_PREPARE:
		Story_Reset();
		Stage_Ready_Reset();
		Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
		Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_1_START);
		break;

		// --- Battle Phases ---
		// ------ PHASE 1 ------
	case STAGE_4_STATE::BOSS_PHASE_1_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_4_PHASE_1);
		Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_1_BATTLE);
		break;

	case STAGE_4_STATE::BOSS_PHASE_1_BATTLE: 
		(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_1_CLEAR);
		}
		break;

	case STAGE_4_STATE::BOSS_PHASE_1_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_4_PHASE_2)
				Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_2_START);
		}
		break;

		// ------ PHASE 2 ------
	case STAGE_4_STATE::BOSS_PHASE_2_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_4_PHASE_2);
		Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_2_BATTLE);
		break;

	case STAGE_4_STATE::BOSS_PHASE_2_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_2_CLEAR);
		}
		break;

	case STAGE_4_STATE::BOSS_PHASE_2_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_4_PHASE_3)
				Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_3_START);
		}
		break;

		// ------ PHASE 3 ------
	case STAGE_4_STATE::BOSS_PHASE_3_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_4_PHASE_3);
		Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_3_BATTLE);
		break;

	case STAGE_4_STATE::BOSS_PHASE_3_BATTLE:
		Enemy_Spawner_Update(elapsed_time);/* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_3_CLEAR);
		}
		break;

	case STAGE_4_STATE::BOSS_PHASE_3_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_4_PHASE_4)
				Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_4_START);
		}
		break;

		// ------ PHASE 4 ------
	case STAGE_4_STATE::BOSS_PHASE_4_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_4_PHASE_4);
		Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_4_BATTLE);
		break;

	case STAGE_4_STATE::BOSS_PHASE_4_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_4_CLEAR);
		}
		break;

	case STAGE_4_STATE::BOSS_PHASE_4_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_4_PHASE_5)
				Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_5_START);
		}
		break;

		// --- PHASE FINAL ---
	case STAGE_4_STATE::BOSS_PHASE_5_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_4_PHASE_5);
		Set_Stage_4_State(STAGE_4_STATE::BOSS_PHASE_5_BATTLE);
		break;

	case STAGE_4_STATE::BOSS_PHASE_5_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			if (Next_Pattern == BOSS_PATTERN_TYPE::NONE)
				Set_Stage_4_State(STAGE_4_STATE::BOSS_DEFEATED);
		}
		break;

	case STAGE_4_STATE::BOSS_DEFEATED:
	{
		Sound_M->Play_SFX("Enemy_Boss_Dead");
		int Index = Enemy_Get_Active_Boss_Index();

		if (Index != -1)
		{
			const Enemy* BOSS = Enemy_Get(Index);

			if (!Is_Item_Drop_Done())
			{
				Item_Drop_Start(BOSS);
			}

			Effect_Create(Effect_Type::REAL_EXPLOSION, BOSS->Position, { BOSS->Size.x, BOSS->Size.x });
			Enemy_Destroy(Index);
		}

		Wait_Timer = 0.0;
		Set_Stage_4_State(STAGE_4_STATE::STAGE_CLEAR);
	}
	break;

	case STAGE_4_STATE::STAGE_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer > STAGE_CLEAR_FREEZE_TIME)
		{
			Is_Item_Drop_Reset();

			Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
			Set_Stage_4_State(STAGE_4_STATE::DONE);
		}
		break;

	case STAGE_4_STATE::DONE: break;
	}
}

void Stage_Five_Update(double elapsed_time)
{
	PATTERN_DATA Data, Data_R, Data_L;
	Data_L.Patrol_Direction = Patrol_Direction::Left;
	Data_R.Patrol_Direction = Patrol_Direction::Right;

	static GameMode Now_Game_Mode = Get_Game_Mode();

	switch (Stage_5_State)
	{
	case STAGE_5_STATE::NONE:
		Stage_Updater_Phase_Reset();
		Set_Stage_5_State(STAGE_5_STATE::CHECK_GAME_MODE);
		break;

	case STAGE_5_STATE::CHECK_GAME_MODE:
		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Story_Played_S5)
		{
			if (Is_Story_Played_S5)
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
				Set_Stage_5_State(STAGE_5_STATE::BOSS_APPEARANCE);
			}
			else
			{
				Set_Stage_5_State(STAGE_5_STATE::STAGE_STORY);
			}
		}
		else
		{
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_5_State(STAGE_5_STATE::BOSS_APPEARANCE);
		}
		break;

	case STAGE_5_STATE::STAGE_STORY:
		if (Story_Is_Finished())
		{
			Set_Stage_5_State(STAGE_5_STATE::WAIT_FOR_START);
			Wait_Timer = 0.0;
			Is_Story_Played_S5 = true;
		}

		if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
		{
			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_START_POS), Player_Spawn_Speed);
			Story_Start(NOW_PLAYING_STAGE::STAGE_FIVE, STORY_TYPE::INTRO_STAGE, false);
		}
		break;

	case STAGE_5_STATE::WAIT_FOR_START:
		Wait_Timer += elapsed_time;

		if (Wait_Timer > 0.1)
		{
			Stage_Updater_Phase_Reset();
			Stage_Ready_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
			Set_Stage_5_State(STAGE_5_STATE::BOSS_APPEARANCE);
		}
		break;

	case STAGE_5_STATE::BOSS_APPEARANCE:
		if (!Stage_5_Boss_Spawned)
		{
			Stage_Updater_Phase_Reset();
			Set_BGM_For_Boss(NOW_PLAYING_STAGE::STAGE_FIVE);

			XMFLOAT2 Boss_Target_POS = { (Game_Offset.x + (Game_Screen_Width * A_Half) - (Enemy_Get_Width(Enemy_Type_Final_Phase_1_Boss, true) * A_Half)),
						Game_Offset.y + (Enemy_Get_Width(Enemy_Type_Final_Phase_1_Boss, false) * A_One_Tenth) };

			int Boss_Index = Enemy_Create(Enemy_Type_Final_Phase_1_Boss, { Boss_Target_POS.x, -Enemy_Get_Width(Enemy_Type_Final_Phase_1_Boss, false) },
				Enemy_Move_Pattern::NONE, {}, Boss_Target_POS);

			Boss_Activate(Boss_Index, NOW_PLAYING_STAGE::STAGE_FIVE);
			Stage_5_Boss_Spawned = true;

			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_5_BOSS_START_POS), Player_Spawn_Speed);

			Story_Reset();
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_ENTERING);
			Set_Stage_5_State(STAGE_5_STATE::BOSS_ENTERING);
		}
		break;

	case STAGE_5_STATE::BOSS_ENTERING:
	{
		bool Is_Entering_Done = !Is_Boss_Entering();
		bool Is_Story_Done = true;

		if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Boss_Story_Played_S5_P1)
		{
			if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
				Story_Start(NOW_PLAYING_STAGE::STAGE_FIVE, STORY_TYPE::INTRO_BOSS, true);
		
			Is_Story_Done = Story_Is_Finished();
		}

		if (Is_Entering_Done && Is_Story_Done)
		{
			if (Now_Game_Mode != GameMode::STAGE_SELECT)
				Is_Boss_Story_Played_S5_P1 = true;

			Set_Stage_5_State(STAGE_5_STATE::BOSS_BATTLE_PREPARE);
		}
	}
	break;

	case STAGE_5_STATE::BOSS_BATTLE_PREPARE:
		Story_Reset();
		Stage_Ready_Reset();
		Set_Now_Stage_Flow(STAGE_FLOW_STATE::PREPARE_GAME);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_1_START);
		break;

		// --- Battle Phases ---
		// ------ PHASE 1 ------
	case STAGE_5_STATE::BOSS_PHASE_1_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_5_PHASE_1);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_1_BATTLE);
		break;

	case STAGE_5_STATE::BOSS_PHASE_1_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_1_CLEAR);
		}
		break;

	case STAGE_5_STATE::BOSS_PHASE_1_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_5_PHASE_2)
				Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_2_START);
		}
		break;

		// ------ PHASE 2 ------
	case STAGE_5_STATE::BOSS_PHASE_2_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_5_PHASE_2);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_2_BATTLE);
		break;

	case STAGE_5_STATE::BOSS_PHASE_2_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_2_CLEAR);
		}
		break;

	case STAGE_5_STATE::BOSS_PHASE_2_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_5_PHASE_3)
				Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_3_START);
		}
		break;

		// ------ PHASE 3 ------
	case STAGE_5_STATE::BOSS_PHASE_3_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_5_PHASE_3);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_3_BATTLE);
		break;

	case STAGE_5_STATE::BOSS_PHASE_3_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_3_CLEAR);
		}
		break;

	case STAGE_5_STATE::BOSS_PHASE_3_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_5_PHASE_4)
				Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_4_START);
		}
		break;

		// ------ PHASE 4 ------
	case STAGE_5_STATE::BOSS_PHASE_4_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_5_PHASE_4);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_4_BATTLE);
		break;

	case STAGE_5_STATE::BOSS_PHASE_4_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_4_CLEAR);
		}
		break;

	case STAGE_5_STATE::BOSS_PHASE_4_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}

				if (Enemy_IsEnable(Index))
				{
					Sound_M->Play_SFX("Enemy_Boss_Dead");

					XMFLOAT2 Boss_Center = { BOSS->Position.x + ((BOSS->Size.x * 0.5f) - (BOSS->Size.y * 0.5f)), BOSS->Position.y};

					Effect_Create(Effect_Type::REAL_EXPLOSION, Boss_Center, { BOSS->Size.y, BOSS->Size.y });
					Enemy_Destroy(Index);
				}
			}
		}

		if (Wait_Timer > STAGE_CLEAR_FREEZE_TIME)
		{
			Is_Item_Drop_Reset();

			Enemy_Spawner_Reset();
			Story_Reset();
			Player_Status_Reset();
			Set_Stage_5_State(STAGE_5_STATE::FAKE_CLEAR_SEQUENCE);
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_CLEAR);
		}
		break;

	case STAGE_5_STATE::FAKE_CLEAR_SEQUENCE:
		Stage_5_Boss_Spawned = false;
		break;

	case STAGE_5_STATE::BOSS_SWAP:
	{
		Wait_Timer = 0.0;

		XMFLOAT2 Boss_Spawn_POS = {
			(Game_Offset.x + (Game_Screen_Width * A_Half) - (Enemy_Get_Width(Enemy_Type_Final_Phase_2_Boss, true) * A_Half)),
			Game_Offset.y + (Enemy_Get_Width(Enemy_Type_Final_Phase_2_Boss, false) * A_One_Tenth)
		};
		int newBossIndex = Enemy_Create(Enemy_Type_Final_Phase_2_Boss, Boss_Spawn_POS, Enemy_Move_Pattern::NONE, {}, Boss_Spawn_POS);

		if (newBossIndex != -1)
		{
			Stage_Final_Boss_Spawned = true;
			Boss_Activate(newBossIndex, NOW_PLAYING_STAGE::STAGE_FIVE);
			Set_Boss_Entering_Done(true);
			Set_Boss_State(BOSS_STATE::BATTLE);
			Stage_Command_Player_Move(Script_Get_Enum(PLAYER_MOVE_POSITION::STAGE_5_BOSS_START_POS), Player_Spawn_Speed);
		}

		Story_Reset();
		Set_Stage_5_State(STAGE_5_STATE::BOSS_FINAL_PHASE_ENTERING);
	}
	break;

		// ---- FINAL PHASE ----
	case STAGE_5_STATE::BOSS_FINAL_PHASE_ENTERING:
	{
		Wait_Timer += elapsed_time;

		if (Wait_Timer >= 2.0)
		{
			bool Is_Story_Done = true;

			if (Now_Game_Mode != GameMode::STAGE_SELECT && !Is_Boss_Story_Played_S5_P2)
			{
				if (Get_Story_State() == Story_Manager_State::IN_ACTIVE)
				{
					Story_Start(NOW_PLAYING_STAGE::STAGE_FIVE, STORY_TYPE::OUTRO_FINAL_BOSS_LIE, true);
				}
				Is_Story_Done = Story_Is_Finished();
			}

			if (Is_Story_Done)
			{
				if (Now_Game_Mode != GameMode::STAGE_SELECT)
				{
					Is_Boss_Story_Played_S5_P2 = true;
				}
				Set_Stage_5_State(STAGE_5_STATE::BOSS_FINAL_PREPARE);
			}
		}
	}
	break;

	case STAGE_5_STATE::BOSS_FINAL_PREPARE:
		Stage_Ready_Reset();
		Stage_BOSS_Ready_Draw_Start();
		Set_Stage_5_State(STAGE_5_STATE::PREPARE_FINAL_BATTLE);
		break;

	case STAGE_5_STATE::PREPARE_FINAL_BATTLE:
		if (Is_Ready_To_Final_Battle())
			Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_5_START);
		break;

		// ------ PHASE 5 ------
	case STAGE_5_STATE::BOSS_PHASE_5_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_Final_PHASE_1);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_5_BATTLE);
		break;

	case STAGE_5_STATE::BOSS_PHASE_5_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_5_CLEAR);
		}
		break;

	case STAGE_5_STATE::BOSS_PHASE_5_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_Final_PHASE_2)
				Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_6_START);
		}
		break;

		// ------ PHASE 6 ------
	case STAGE_5_STATE::BOSS_PHASE_6_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_Final_PHASE_2);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_6_BATTLE);
		break;

	case STAGE_5_STATE::BOSS_PHASE_6_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			Wait_Timer = 0.0;
			Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_6_CLEAR);
		}
		break;

	case STAGE_5_STATE::BOSS_PHASE_6_CLEAR:
		Wait_Timer += elapsed_time;

		if (Wait_Timer < 0.1)
		{
			int Index = Enemy_Get_Active_Boss_Index();

			if (Index != -1)
			{
				const Enemy* BOSS = Enemy_Get(Index);

				if (!Is_Item_Drop_Done())
				{
					Item_Drop_Start(BOSS);
				}
			}
		}

		if (Wait_Timer > PHASE_CHANGE_FREEZE_TIME)
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			Is_Item_Drop_Reset();

			if (Next_Pattern == BOSS_PATTERN_TYPE::STAGE_Final_PHASE_3)
				Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_7_START);
		}
		break;

		// ------ PHASE FINAL ------
	case STAGE_5_STATE::BOSS_PHASE_7_START:
		Boss_Start_Pattern(BOSS_PATTERN_TYPE::STAGE_Final_PHASE_3);
		Set_Stage_5_State(STAGE_5_STATE::BOSS_PHASE_7_BATTLE);
		break;

	case STAGE_5_STATE::BOSS_PHASE_7_BATTLE:
		Enemy_Spawner_Update(elapsed_time); /* TODO: spawn zako */

		if (Is_Current_Phase_Finished())
		{
			BOSS_PATTERN_TYPE Next_Pattern = Get_Next_Boss_Pattern(Boss_Get_Current_Pattern());
			if (Next_Pattern == BOSS_PATTERN_TYPE::NONE)
				Set_Stage_5_State(STAGE_5_STATE::BOSS_DEFEATED);
		}
		break;

	case STAGE_5_STATE::BOSS_DEFEATED:
	{
		Sound_M->Play_SFX("Enemy_Boss_Dead");
		int Index = Enemy_Get_Active_Boss_Index();
		if (Index != -1)
		{
			const Enemy* BOSS = Enemy_Get(Index);

			if (!Is_Item_Drop_Done())
			{
				Item_Drop_Start(BOSS);
			}

			XMFLOAT2 Boss_Center = { BOSS->Position.x + ((BOSS->Size.x * 0.5f) - (BOSS->Size.y * 0.5f)), BOSS->Position.y };

			Effect_Create(Effect_Type::REAL_EXPLOSION, Boss_Center, { BOSS->Size.y, BOSS->Size.y });
			Enemy_Destroy(Index);
		}

		Wait_Timer = 0.0;
		Set_Stage_5_State(STAGE_5_STATE::STAGE_CLEAR);
	}
	break;

	case STAGE_5_STATE::STAGE_CLEAR:
		Wait_Timer += elapsed_time;
		if (Wait_Timer > STAGE_CLEAR_FREEZE_TIME)
		{
			Is_Item_Drop_Reset();

			Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
			Set_Stage_5_State(STAGE_5_STATE::DONE);
		}
		break;

	case STAGE_5_STATE::DONE: break;
	}
}

void Stage_Updater_Phase_Reset()
{
	Enemy_Spawner_Reset();
	Stage_Timer_Reset();
}

STAGE_1_STATE Get_Stage_1_State()
{
	return Stage_1_State;
}

void Set_Stage_1_State(STAGE_1_STATE State)
{
	Stage_1_State = State;
}

STAGE_2_STATE Get_Stage_2_State()
{
	return Stage_2_State;
}

void Set_Stage_2_State(STAGE_2_STATE State)
{
	Stage_2_State = State;
}

STAGE_3_STATE Get_Stage_3_State()
{
	return Stage_3_State;
}

void Set_Stage_3_State(STAGE_3_STATE State)
{
	Stage_3_State = State;
}

STAGE_4_STATE Get_Stage_4_State()
{
	return Stage_4_State;
}

void Set_Stage_4_State(STAGE_4_STATE State)
{
	Stage_4_State = State;
}

STAGE_5_STATE Get_Stage_5_State()
{
	return Stage_5_State;
}

void Set_Stage_5_State(STAGE_5_STATE State)
{
	Stage_5_State = State;
}

bool Is_Final_Boss_Entered()
{
	return Stage_Final_Boss_Spawned;
}

BOSS_PATTERN_TYPE Get_Next_Boss_Pattern(BOSS_PATTERN_TYPE Current_pattern)
{
	if (Current_pattern == BOSS_PATTERN_TYPE::STAGE_3_PHASE_3)
		return BOSS_PATTERN_TYPE::NONE;
	if (Current_pattern == BOSS_PATTERN_TYPE::STAGE_4_PHASE_5)
		return BOSS_PATTERN_TYPE::NONE;
	if (Current_pattern == BOSS_PATTERN_TYPE::STAGE_5_PHASE_4)
		return BOSS_PATTERN_TYPE::NONE;
	if (Current_pattern == BOSS_PATTERN_TYPE::STAGE_Final_PHASE_3)
		return BOSS_PATTERN_TYPE::NONE;

	return static_cast<BOSS_PATTERN_TYPE>(static_cast<int>(Current_pattern) + 1);
}

void Item_Drop_Start(const Enemy* Index)
{
	XMFLOAT2 Boss_Center = { Index->Position.x + Index->Size.x * 0.5f, Index->Position.y + Index->Size.y * 0.5f };

	switch (Index->Type_ID)
	{
	case Enemy_Type_Special_Boss_Stage_3:

		Item_Create(Boss_Center, Index->Size, Item_Type::POWER_UP, A_Quarter, 5);
		Item_Create(Boss_Center, Index->Size, Item_Type::LIVE);
		Item_Create(Boss_Center, Index->Size, Item_Type::BOMB);
		Score_Create(Boss_Center, Index->Size, Item_Type::SCORE, Enemy_Type_Special_Boss_Phase_Change_Score, 15);
		break;

	case Enemy_Type_Middle_Boss_Stage_4:
		Item_Create(Boss_Center, Index->Size, Item_Type::POWER_UP, A_Quarter, 6);
		Item_Create(Boss_Center, Index->Size, Item_Type::LIVE);
		Item_Create(Boss_Center, Index->Size, Item_Type::BOMB, 0, 2);
		Score_Create(Boss_Center, Index->Size, Item_Type::SCORE, Enemy_Type_Middle_Boss_Phase_Change_Score, 15);
		break;

	case Enemy_Type_Final_Phase_1_Boss:
		Item_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::POWER_UP, A_Quarter, 5);
		Item_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::LIVE);
		Item_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::BOMB);
		Score_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::SCORE, Enemy_Type_Final_Boss_Phase_Change_Score, 18);
		break;

	case Enemy_Type_Final_Phase_2_Boss:
		Item_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::POWER_UP, A_Quarter, 8);
		Item_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::LIVE);
		Item_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::BOMB);
		Score_Create(Boss_Center, { Index->Size.y, Index->Size.y }, Item_Type::SCORE, Enemy_Type_Final_Boss_Phase_2_Change_Score, 20);
		break;
	}

	Is_Item_Drop = true;
}

bool Is_Item_Drop_Done()
{
	return Is_Item_Drop;
}

void Is_Item_Drop_Reset()
{
	Is_Item_Drop = false;
}

void Story_Trigger_Saver()
{
	Story_Trigger_Saver_S1 = Is_Story_Played_S1;
	Story_Trigger_Saver_S2 = Is_Story_Played_S2;
	Story_Trigger_Saver_S3 = Is_Story_Played_S3;
	Story_Trigger_Saver_S4 = Is_Story_Played_S4;
	Story_Trigger_Saver_S5 = Is_Story_Played_S5;
	Boss_Story_Trigger_Saver_S3 = Is_Boss_Story_Played_S3;
	Boss_Story_Trigger_Saver_S4 = Is_Boss_Story_Played_S4;
	Boss_Story_Trigger_Saver_S5_P1 = Is_Boss_Story_Played_S5_P1;
	Boss_Story_Trigger_Saver_S5_P2 = Is_Boss_Story_Played_S5_P2;
}

void Story_Trigger_Loader()
{
	Is_Story_Played_S1 = Story_Trigger_Saver_S1;
	Is_Story_Played_S2 = Story_Trigger_Saver_S2;
	Is_Story_Played_S3 = Story_Trigger_Saver_S3;
	Is_Story_Played_S4 = Story_Trigger_Saver_S4;
	Is_Story_Played_S5 = Story_Trigger_Saver_S5;
	Is_Boss_Story_Played_S3 = Boss_Story_Trigger_Saver_S3;
	Is_Boss_Story_Played_S4 = Boss_Story_Trigger_Saver_S4;
	Is_Boss_Story_Played_S5_P1 = Boss_Story_Trigger_Saver_S5_P1;
	Is_Boss_Story_Played_S5_P2 = Boss_Story_Trigger_Saver_S5_P2;
}