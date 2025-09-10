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
#include "texture.h"
#include "Palette.h"
#include "Save_Data.h"
#include "Game_Select.h"
#include "Game_Mode_Manager.h"
#include "Story_Script.h"
#include "Story_Manager.h"

static STAGE_PROGRESS	Stage_1_Progress;
static STAGE_PROGRESS	Stage_2_Progress;
static STAGE_3_STATE	Stage_3_State = STAGE_3_STATE::NONE;
static STAGE_5_STATE	Stage_5_State = STAGE_5_STATE::NONE;

static double Stage_Timer = 0.0;
static double Wait_Timer = 0.0;

static int Stage_3_Boss_Index = -1;

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
	Is_Stage_1_Spawned = false;
	Is_Stage_1_Enemy_Left = false;

	Is_Stage_2_Spawned = false;
	Is_Stage_2_Enemy_Left = false;

	Is_Stage_3_Spawned = false;
	Is_Stage_3_Enemy_Left = false;

	Stage_1_Progress = STAGE_PROGRESS::SPAWNING;
	Stage_2_Progress = STAGE_PROGRESS::SPAWNING;
	Stage_3_State = STAGE_3_STATE::NONE;
	Stage_5_State = STAGE_5_STATE::NONE;
}

void Stage_One_Update(double elapsed_time)
{
	Stage_Timer_Update(elapsed_time);

	switch (Stage_1_Progress)
	{
	case STAGE_PROGRESS::SPAWNING:
	{
		if (Is_Stage_1_Spawned)
		{
			if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			{
				Wait_Timer = 0.0;
				Is_Stage_1_Enemy_Left = false;
				Stage_1_Progress = STAGE_PROGRESS::WAIT_FOR_CLEAR;
				break;
			}
		}

		if (Stage_Get_Timer() > STAGE_1_DONE_TIME)
		{
			if (Are_All_Enemies_Cleared())
			{
				Wait_Timer = 0.0;
				Is_Stage_1_Enemy_Left = false;
				Stage_1_Progress = STAGE_PROGRESS::WAIT_FOR_CLEAR;
			}
			else
			{
				Is_Stage_1_Enemy_Left = true;
				Stage_1_Progress = STAGE_PROGRESS::TIME_LIMIT_REACHED;
			}
		}

		PATTERN_DATA Data, Data_R, Data_L;
		Data_L.Patrol_Direction = Patrol_Direction::Left;
		Data_R.Patrol_Direction = Patrol_Direction::Right;

		if (!Is_Stage_1_Spawned)
		{
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Three) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
														-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 3.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Seven) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
														-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 3.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Up, Data);

			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
													-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 10.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data_L);
			Enemy_Spawner_Create(Enemy_Type_Origin, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Origin, true) * A_Half,
													-Enemy_Get_Width(Enemy_Type_Origin, false) }, 10.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
													-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 10.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data_R);

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 20.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 20.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Data.Target_Y = Game_Screen_Height * A_P_Three_Five;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 25.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 25.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Data.Target_Y = Game_Screen_Height * A_Half;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 30.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 30.0, 0.5, 1, Enemy_Move_Pattern::Down_And_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Armor_Give_HP, { Game_Offset.x + (Game_Screen_Width * A_Quarter)	- Enemy_Get_Width(Enemy_Type_Armor_Give_HP, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Armor_Give_HP, false) }, 32.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data);
			Enemy_Spawner_Create(Enemy_Type_Armor_Give_HP, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Armor_Give_HP, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Armor_Give_HP, false) }, 32.0, 1.0, 1, Enemy_Move_Pattern::Down_Patrol, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 37.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 37.0, 1.0, 1, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Half;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Quarter)	- Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 45.0, 5.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 45.0, 5.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Data.Target_Y = Game_Screen_Height * A_Point_Fifteen; // BOSS
			Enemy_Spawner_Create(Enemy_Type_Armor_Boss, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Armor_Boss, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Armor_Boss, false) }, 47.0, 1.0, 1, Enemy_Move_Pattern::Down_Fixed, Data);

			Data.Target_Y = Game_Screen_Height * A_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Origin, { Game_Offset.x + (Game_Screen_Width * A_Point_Three) - Enemy_Get_Width(Enemy_Type_Origin, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin, false) }, 52.0, 10.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin, { Game_Offset.x + (Game_Screen_Width * A_Point_Seven) - Enemy_Get_Width(Enemy_Type_Origin, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin, false) }, 52.0, 10.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);

			Data.Target_Y = Game_Screen_Height * A_Two_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 55.0, 7.0, 4, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 55.0, 7.0, 4, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 70.0, 8.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Spear, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear, false) }, 70.0, 8.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Saber, { Game_Offset.x + (Game_Screen_Width * A_Quarter)	- Enemy_Get_Width(Enemy_Type_Saber, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber, false) }, 80.0, 5.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);
			Enemy_Spawner_Create(Enemy_Type_Saber, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Saber, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Saber, false) }, 80.0, 5.0, 2, Enemy_Move_Pattern::Down_And_Up, Data);

			Data.Target_Y = Game_Screen_Height * A_Two_Fifth;
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO_ITEM, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO_ITEM, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO_ITEM, false) }, 83.0, 10.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 90.0, 7.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 90.0, 7.0, 2, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Data.Target_Y = Game_Screen_Height * A_Third;
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 90.0, 7.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Origin_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_T_Quarters) - Enemy_Get_Width(Enemy_Type_Origin_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Origin_ZAKO, false) }, 90.0, 7.0, 2, Enemy_Move_Pattern::Down_Shoot_Down, Data);

			Is_Stage_1_Spawned = true;
		}
	}
	break;

	case STAGE_PROGRESS::TIME_LIMIT_REACHED:
		if (Are_All_Enemies_Cleared())
		{
			Wait_Timer = 0.0;
			Stage_1_Progress = STAGE_PROGRESS::WAIT_FOR_CLEAR;
		}
		break;

	case STAGE_PROGRESS::WAIT_FOR_CLEAR:
	{
		Wait_Timer += elapsed_time;

		double Clear_Delay = Is_Stage_1_Enemy_Left ? 5.0 : 3.0;

		if (Wait_Timer > Clear_Delay)
		{
			if (Player_Is_Alive())
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
				Stage_1_Progress = STAGE_PROGRESS::DONE;
			}
		}
		break;
	}

	case STAGE_PROGRESS::DONE:
		break;
	}
}

void Stage_Two_Update(double elapsed_time)
{
	Stage_Timer_Update(elapsed_time);

	switch (Stage_2_Progress)
	{
	case STAGE_PROGRESS::SPAWNING:
	{
		if (Is_Stage_2_Spawned)
		{
			if (Enemy_Spawner_Is_Done() && Are_All_Enemies_Cleared())
			{
				Wait_Timer = 0.0;
				Is_Stage_2_Enemy_Left = false;
				Stage_2_Progress = STAGE_PROGRESS::WAIT_FOR_CLEAR;
				break;
			}
		}


		if (Stage_Get_Timer() > STAGE_2_DONE_TIME)
		{
			if (Are_All_Enemies_Cleared())
			{
				Wait_Timer = 0.0;
				Is_Stage_2_Enemy_Left = false;
				Stage_2_Progress = STAGE_PROGRESS::WAIT_FOR_CLEAR;
			}
			else
			{
				Is_Stage_2_Enemy_Left = true;
				Stage_2_Progress = STAGE_PROGRESS::TIME_LIMIT_REACHED;
			}
		}

		PATTERN_DATA Data, Data_R, Data_L;
		Data_L.Patrol_Direction = Patrol_Direction::Left;
		Data_R.Patrol_Direction = Patrol_Direction::Right;

		if (!Is_Stage_2_Spawned)
		{
			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Data_L.Target_Y = Game_Screen_Height * A_Quarter;
			Data_R.Target_Y = Game_Screen_Height * A_Quarter;
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

			Data.Target_Y = Game_Screen_Height * A_Third; // Mini BOSS
			Enemy_Spawner_Create(Enemy_Type_Mini_Boss, { Game_Offset.x + (Game_Screen_Width * A_Half) - Enemy_Get_Width(Enemy_Type_Mini_Boss, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Mini_Boss, false) }, 30.0, 0.5, 1, Enemy_Move_Pattern::Down_Fixed, Data);

			Data.Target_Y = Game_Screen_Height * A_Quarter;
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Point_Fifteen) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 28.0, 2.0, 6, Enemy_Move_Pattern::Down_Shoot_Down, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_P_Eight_Five) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 28.0, 2.0, 6, Enemy_Move_Pattern::Down_Shoot_Down, Data);

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

			Is_Stage_2_Spawned = true;
		}
	}
	break;

	case STAGE_PROGRESS::TIME_LIMIT_REACHED:
		if (Are_All_Enemies_Cleared())
		{
			Wait_Timer = 0.0;
			Stage_2_Progress = STAGE_PROGRESS::WAIT_FOR_CLEAR;
		}
		break;

	case STAGE_PROGRESS::WAIT_FOR_CLEAR:
	{
		Wait_Timer += elapsed_time;

		double Clear_Delay = Is_Stage_2_Enemy_Left ? 5.0 : 3.0;

		if (Wait_Timer > Clear_Delay)
		{
			if (Player_Is_Alive())
			{
				Set_Now_Stage_Flow(STAGE_FLOW_STATE::STAGE_DONE);
				Stage_2_Progress = STAGE_PROGRESS::DONE;
			}
		}
		break;
	}

	case STAGE_PROGRESS::DONE:
		break;
	}
}

void Stage_Three_Update(double elapsed_time)
{
	Stage_Timer_Update(elapsed_time);

	switch (Stage_3_State)
	{
	case STAGE_3_STATE::NONE:
		Set_Stage_3_State(STAGE_3_STATE::NORMAL_PHASE);
		break;

	case STAGE_3_STATE::NORMAL_PHASE:
	{
		if (Is_Stage_3_Spawned && (Stage_Get_Timer() > STAGE_3_DONE_TIME))
		{
			if (Are_All_Enemies_Cleared())
			{
				Wait_Timer = 0.0;
				Player_Set_State(PLAYER_STATE::NONE);

				float Speed = Player_Spawn_Speed * A_One_And_Half;
				Stage_Command_Player_Move(Stage_3_Boss_Start_Pos, Speed);

				Set_Stage_3_State(STAGE_3_STATE::BOSS_PREPARE);
			}
		}

		PATTERN_DATA Data;

		if (!Is_Stage_3_Spawned)
		{
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
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_Quarter)	- Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 27.0, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);
			Enemy_Spawner_Create(Enemy_Type_Spear_ZAKO, { Game_Offset.x + (Game_Screen_Width * A_One_Tenth) - Enemy_Get_Width(Enemy_Type_Spear_ZAKO, true) * A_Half,
												-Enemy_Get_Width(Enemy_Type_Spear_ZAKO, false) }, 27.5, 1.0, 1, Enemy_Move_Pattern::Down_And_Chase_Player, Data);

			Is_Stage_3_Spawned = true;
		}
	}
	break;

	case STAGE_3_STATE::BOSS_PREPARE:
	{
		Stage_Set_Is_Player_Moving_By_Script(true);

		Wait_Timer += elapsed_time;

		if (Wait_Timer <= STAGE_3_BOSS_WAIT_TIME)
		{
			int Now_Volume = static_cast<int>(SOUND_MAX * (1.0 - (Wait_Timer / STAGE_3_BOSS_WAIT_TIME)));
			if (Now_Volume < 0)
				Now_Volume = 0;
			SM->Set_BGM_Volume(Now_Volume);
		}

		if (Wait_Timer > STAGE_3_BOSS_WAIT_TIME)
		{
			SM->Stop_BGM();
			SM->Set_BGM_Volume(SOUND_MAX);
			Set_Now_Stage_Flow(STAGE_FLOW_STATE::BOSS_ENTERING);
			Set_Stage_3_State(STAGE_3_STATE::DONE);
			Stage_Set_Is_Player_Moving_By_Script(false);
		}	
	}
	break;
	case STAGE_3_STATE::DONE:
		break;
	}
}

STAGE_3_STATE Get_Stage_3_State()
{
	return Stage_3_State;
}

void Set_Stage_3_State(STAGE_3_STATE State)
{
	Stage_3_State = State;
}

STAGE_5_STATE Get_Stage_5_State()
{
	return Stage_5_State;
}

void Set_Stage_5_State(STAGE_5_STATE State)
{
	Stage_5_State = State;
}
