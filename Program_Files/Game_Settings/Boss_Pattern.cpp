/*==============================================================================

	Manager Boss Pattern [Boss_Pattern.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Boss_Pattern.h"
#include "Player.h" 
#include <cmath>
#include "Audio_Manager.h"
#include "Stage_Select.h"

using namespace PALETTE;
using namespace DirectX;

static double patternTimer = 0.0;
static double U_T_Fire_Timer = 0.0;
static double D_T_Fire_Timer = 0.0;
static double Caging_Fire_Timer = 0.0;
static double Burst_Fire_Timer = 0.0;

bool Boss_Pattern_Enter(Enemy& boss, double elapsed_time)
{
	XMVECTOR Current_POS = XMLoadFloat2(&boss.Position);
	XMVECTOR Target_POS = XMLoadFloat2(&boss.Target_POS);

	XMVECTOR To_Target = Target_POS - Current_POS;
	float Distance = XMVectorGetX(XMVector2Length(To_Target));

	float Amount = boss.Speed * static_cast<float>(elapsed_time);

	if (Distance > Amount)
	{
		XMVECTOR Direction = XMVector2Normalize(To_Target);
		Current_POS += Direction * Amount;
		XMStoreFloat2(&boss.Position, Current_POS);
		return false;
	}
	else
	{
		boss.Position = boss.Target_POS;
		return true;
	}
}

void Boss_Pattern_Initialize(Enemy& boss)
{
	patternTimer = 0.0;
	boss.State = Enemy_State::Attack;
	boss.Fire_Timer = 0.0;
	U_T_Fire_Timer = 0.0;
	D_T_Fire_Timer = 0.0;
	Caging_Fire_Timer = 0.0;
	Burst_Fire_Timer = 0.0;
}

void Boss_Pattern_Stage_3_Update(Enemy& boss, double elapsed_time, BOSS_PATTERN_TYPE Type)
{
	patternTimer += elapsed_time;

	float y_offset = sinf(static_cast<float>(patternTimer) * HOVER_SPEED) * HOVER_AMPLITUDE;
	boss.Position.y = boss.Target_POS.y + y_offset;

	const Enemy_Type& Boss_Type = Get_Enemy_Info(boss.Type_ID);

	if (Get_Now_Stage_Flow() != STAGE_FLOW_STATE::GAMEPLAY_ACTIVE || !Player_Is_Alive())
		return;

	switch (Type)
	{
	case BOSS_PATTERN_TYPE::STAGE_3_PHASE_1:
	{
		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval)
		{
			boss.Fire_Timer = 0.0;

			XMFLOAT2 Bullet_Size = { Enemy_Bullet_Normal_Width, Enemy_Bullet_Normal_Height };
			float Spawn_L  = boss.Position.x + (boss.Size.x * A_One_Tenth) - (Bullet_Size.x * A_Half);
			float Spawn_R  = boss.Position.x + (boss.Size.x * A_P_Eight_Five) - (Bullet_Size.x * A_Half);
			float Center_Y = boss.Position.y + (boss.Size.y * A_Half) - (Bullet_Size.y * A_Half);

			XMFLOAT2 B_POS[2] = { { Spawn_L, Center_Y }, { Spawn_R, Center_Y } };
			XMFLOAT2 Player_Center = Player_Get_Collision().Center;

			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_Vel = XMVector2Normalize(XMVectorSet(Player_Center.x - B_POS[i].x, Player_Center.y - B_POS[i].y, 0, 0));
				float Speed = Enemy_Get_Bullet_Speed(Boss_Type.Bullet_Type);
				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_Vel * Speed);
				Enemy_Bullet_Normal_creat(B_POS[i], Vel);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}
		break;
	}
	case BOSS_PATTERN_TYPE::STAGE_3_PHASE_2:
	case BOSS_PATTERN_TYPE::STAGE_3_PHASE_3:
	{
		boss.Fire_Timer += elapsed_time;
		double Fire_Interval_Rate = (Type == BOSS_PATTERN_TYPE::STAGE_3_PHASE_2) ? (1.0 * (A_Third * A_Double)) : (1.0 * A_Half);
		if (boss.Fire_Timer >= (Boss_Type.Fire_Interval * Fire_Interval_Rate))
		{
			boss.Fire_Timer = 0.0;
			XMFLOAT2 Normal_Bullet_Size = { Enemy_Bullet_Normal_Width, Enemy_Bullet_Normal_Height };
			XMFLOAT2 Long_Bullet_Size = { Enemy_Bullet_Long_Width, Enemy_Bullet_Long_Height };

			float Spawn_L_X = boss.Position.x + (boss.Size.x * A_One_Tenth);
			float Spawn_R_X = boss.Position.x + (boss.Size.x * A_P_Eight_Five);
			float Center_Y  = boss.Position.y + (boss.Size.y * A_Four_Five);

			XMFLOAT2 Spawn_Points[2] = { {Spawn_L_X, Center_Y}, {Spawn_R_X, Center_Y} };
			XMFLOAT2 Player_Center = Player_Get_Collision().Center;

			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - Spawn_Points[i].x, Player_Center.y - Spawn_Points[i].y, 0, 0));

				XMMATRIX Rot_L = XMMatrixRotationZ(XMConvertToRadians(-R_Quarter_Pie));
				XMMATRIX Rot_R = XMMatrixRotationZ(XMConvertToRadians(R_Quarter_Pie));
				XMVECTOR Dir_L = XMVector2TransformCoord(Dir_To_Player, Rot_L);
				XMVECTOR Dir_R = XMVector2TransformCoord(Dir_To_Player, Rot_R);

				XMFLOAT2 Vel_L, Vel_R, Vel_Center;
				XMStoreFloat2(&Vel_L, Dir_L * Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL));
				XMStoreFloat2(&Vel_R, Dir_R * Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL));
				XMStoreFloat2(&Vel_Center, Dir_To_Player * Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG));

				XMFLOAT2 Spawn_pos_N = { Spawn_Points[i].x - (Normal_Bullet_Size.x * A_Half), Spawn_Points[i].y - (Normal_Bullet_Size.y * A_Half) };
				XMFLOAT2 Spawn_pos_L = { Spawn_Points[i].x - (Long_Bullet_Size.x * A_Half), Spawn_Points[i].y - (Long_Bullet_Size.y * A_Half) };

				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R);

				float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}
		break;
	}
	}
}

void Boss_Pattern_Stage_4_Update(Enemy& boss, double elapsed_time, BOSS_PATTERN_TYPE Type)
{
	patternTimer += elapsed_time;

	float y_offset = sinf(static_cast<float>(patternTimer) * HOVER_SPEED) * HOVER_AMPLITUDE;
	boss.Position.y = boss.Target_POS.y + y_offset;

	const Enemy_Type& Boss_Type = Get_Enemy_Info(boss.Type_ID);

	if (Get_Now_Stage_Flow() != STAGE_FLOW_STATE::GAMEPLAY_ACTIVE || !Player_Is_Alive())
		return;

	XMFLOAT2 U_Turret_L = { boss.Position.x + boss.Size.x * A_Fifth, boss.Position.y + boss.Size.y * A_Point_Seven };
	XMFLOAT2 U_Turret_R = { boss.Position.x + boss.Size.x * A_T_Quarters, boss.Position.y + boss.Size.y * A_Point_Seven };
	XMFLOAT2 D_Turret_L = { boss.Position.x + boss.Size.x * A_P_Three_Five, boss.Position.y + boss.Size.y * A_F_Fifths };
	XMFLOAT2 D_Turret_R = { boss.Position.x + boss.Size.x * A_Six_Tenths, boss.Position.y + boss.Size.y * A_F_Fifths };
	XMFLOAT2 Turrets[4] = { U_Turret_L, U_Turret_R, D_Turret_L, D_Turret_R };

	XMFLOAT2 Player_Center = Player_Get_Collision().Center;

	switch (Type)
	{
	case BOSS_PATTERN_TYPE::STAGE_4_PHASE_1:
	case BOSS_PATTERN_TYPE::STAGE_4_PHASE_2:
	{
		boss.Fire_Timer += elapsed_time;
		double Fire_Interval_Rate = (Type == BOSS_PATTERN_TYPE::STAGE_4_PHASE_1) ? (1.0) : (1.0 * A_Half);
		if (boss.Fire_Timer >= (Boss_Type.Fire_Interval * Fire_Interval_Rate))
		{
			boss.Fire_Timer = 0.0;

			for (int i = 0; i < 4; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - Turrets[i].x, Player_Center.y - Turrets[i].y, 0, 0));
				float Speed = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);

				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_To_Player * Speed);

				XMFLOAT2 spawnPos = { Turrets[i].x - (Enemy_Bullet_Normal_Width * 0.5f), Turrets[i].y - (Enemy_Bullet_Normal_Height * 0.5f) };
				Enemy_Bullet_Normal_creat(spawnPos, Vel);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}
		break;
	}

	case BOSS_PATTERN_TYPE::STAGE_4_PHASE_3:
	{
		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval * 0.75)
		{
			boss.Fire_Timer = 0.0;

			for (int i = 0; i < 4; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - Turrets[i].x, Player_Center.y - Turrets[i].y, 0, 0));

				XMMATRIX Rot_L = XMMatrixRotationZ(XMConvertToRadians(-R_Quarter_Pie));
				XMMATRIX Rot_R = XMMatrixRotationZ(XMConvertToRadians(R_Quarter_Pie));

				XMVECTOR Dir_L = XMVector2TransformCoord(Dir_To_Player, Rot_L);
				XMVECTOR Dir_R = XMVector2TransformCoord(Dir_To_Player, Rot_R);

				float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
				float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

				XMFLOAT2 Vel_L, Vel_R, Vel_Center;
				XMStoreFloat2(&Vel_L, Dir_L * Speed_Normal);
				XMStoreFloat2(&Vel_R, Dir_R * Speed_Normal);
				XMStoreFloat2(&Vel_Center, Dir_To_Player * Speed_Long);

				XMFLOAT2 Spawn_pos_N = { Turrets[i].x - (Enemy_Bullet_Normal_Width * A_Half), Turrets[i].y - (Enemy_Bullet_Normal_Height * A_Half) };
				XMFLOAT2 Spawn_pos_L = { Turrets[i].x - (Enemy_Bullet_Long_Width * A_Half), Turrets[i].y - (Enemy_Bullet_Long_Height * A_Half) };

				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R);
				float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");
		}
		break;
	}
	case BOSS_PATTERN_TYPE::STAGE_4_PHASE_4:
	{
		U_T_Fire_Timer += elapsed_time;
		if (U_T_Fire_Timer >= 3.0)
		{
			U_T_Fire_Timer = 0.0;
			XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
			XMVECTOR Dir_Down = XMVectorSet(0.0f, 1.0f, 0, 0);
			float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);
			XMFLOAT2 Vel;
			XMStoreFloat2(&Vel, Dir_Down * Speed_Long);

			for (int i = 0; i < 2; ++i)
			{
				XMFLOAT2 Spawn_pos_L = { U_T[i].x - (Enemy_Bullet_Long_Width * A_Half), U_T[i].y - (Enemy_Bullet_Long_Height * A_Half) };
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel, 180.0f);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");
		}

		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval * 0.8)
		{
			boss.Fire_Timer = 0.0;

			XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - U_T[i].x, Player_Center.y - U_T[i].y, 0, 0));

				XMMATRIX Rot_L = XMMatrixRotationZ(XMConvertToRadians(-R_Quarter_Pie));
				XMMATRIX Rot_R = XMMatrixRotationZ(XMConvertToRadians(R_Quarter_Pie));
				XMVECTOR Dir_L = XMVector2TransformCoord(Dir_To_Player, Rot_L);
				XMVECTOR Dir_R = XMVector2TransformCoord(Dir_To_Player, Rot_R);

				float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
				float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

				XMFLOAT2 Vel_L, Vel_R, Vel_Center;
				XMStoreFloat2(&Vel_L, Dir_L * Speed_Normal);
				XMStoreFloat2(&Vel_R, Dir_R * Speed_Normal);
				XMStoreFloat2(&Vel_Center, Dir_To_Player * Speed_Long);

				XMFLOAT2 Spawn_pos_N = { U_T[i].x - (Enemy_Bullet_Normal_Width * A_Half), U_T[i].y - (Enemy_Bullet_Normal_Height * A_Half) };
				XMFLOAT2 Spawn_pos_L = { U_T[i].x - (Enemy_Bullet_Long_Width * A_Half), U_T[i].y - (Enemy_Bullet_Long_Height * A_Half) };

				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R);
				float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");

			float Speed_Spire = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::SPIRE);
			XMVECTOR D_Dir_To_Player_L = XMVector2Normalize(XMVectorSet(Player_Center.x - D_Turret_L.x, Player_Center.y - D_Turret_L.y, 0, 0));
			XMVECTOR D_Dir_To_Player_R = XMVector2Normalize(XMVectorSet(Player_Center.x - D_Turret_R.x, Player_Center.y - D_Turret_R.y, 0, 0));

			XMFLOAT2 Vel_Spire_L, Vel_Spire_R;
			XMStoreFloat2(&Vel_Spire_L, D_Dir_To_Player_L * Speed_Spire);
			XMStoreFloat2(&Vel_Spire_R, D_Dir_To_Player_R * Speed_Spire);

			float Angle_Spire_L = (atan2f(XMVectorGetY(D_Dir_To_Player_L), XMVectorGetX(D_Dir_To_Player_L)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
			float Angle_Spire_R = (atan2f(XMVectorGetY(D_Dir_To_Player_R), XMVectorGetX(D_Dir_To_Player_R)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

			XMFLOAT2 spawnPos_Spire_L = { D_Turret_L.x - (Enemy_Bullet_Spire_Width * 0.5f), D_Turret_L.y - (Enemy_Bullet_Spire_Height * 0.5f) };
			XMFLOAT2 spawnPos_Spire_R = { D_Turret_R.x - (Enemy_Bullet_Spire_Width * 0.5f), D_Turret_R.y - (Enemy_Bullet_Spire_Height * 0.5f) };

			Enemy_Bullet_Spire_creat(spawnPos_Spire_L, Vel_Spire_L, Angle_Spire_L);
			Enemy_Bullet_Spire_creat(spawnPos_Spire_R, Vel_Spire_R, Angle_Spire_R);

			float Speed_Long_Corner = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);
			XMFLOAT2 Corner_L = { Game_Offset.x, static_cast<float>(Game_Screen_Height) };
			XMFLOAT2 Corner_R = { Game_Offset.x + Game_Screen_Width, static_cast<float>(Game_Screen_Height) };

			XMVECTOR Dir_To_Corner_L = XMVector2Normalize(XMVectorSet(Corner_L.x - D_Turret_L.x, Corner_L.y - D_Turret_L.y, 0, 0));
			XMVECTOR Dir_To_Corner_R = XMVector2Normalize(XMVectorSet(Corner_R.x - D_Turret_R.x, Corner_R.y - D_Turret_R.y, 0, 0));

			XMFLOAT2 Vel_Corner_L, Vel_Corner_R;
			XMStoreFloat2(&Vel_Corner_L, Dir_To_Corner_L * Speed_Long_Corner);
			XMStoreFloat2(&Vel_Corner_R, Dir_To_Corner_R * Speed_Long_Corner);

			float Angle_Corner_L = atan2f(XMVectorGetY(Dir_To_Corner_L), XMVectorGetX(Dir_To_Corner_L));
			float Angle_Corner_R = atan2f(XMVectorGetY(Dir_To_Corner_R), XMVectorGetX(Dir_To_Corner_R));

			XMFLOAT2 S_Pos_Long_L = { D_Turret_L.x - (Enemy_Bullet_Long_Width * 0.5f), D_Turret_L.y - (Enemy_Bullet_Long_Height * 0.5f) };
			XMFLOAT2 S_Pos_Long_R = { D_Turret_R.x - (Enemy_Bullet_Long_Width * 0.5f), D_Turret_R.y - (Enemy_Bullet_Long_Height * 0.5f) };

			Enemy_Bullet_Long_creat(S_Pos_Long_L, Vel_Corner_L, Angle_Corner_L);
			Enemy_Bullet_Long_creat(S_Pos_Long_R, Vel_Corner_R, Angle_Corner_R);

			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}
		break;
	}
	case BOSS_PATTERN_TYPE::STAGE_4_PHASE_5:
	{
		U_T_Fire_Timer += elapsed_time;
		if (U_T_Fire_Timer >= 0.5)
		{
			U_T_Fire_Timer = 0.0;

			XMFLOAT2 Turrets[2] = { U_Turret_L, U_Turret_R };
			XMVECTOR Dir_Down = XMVectorSet(0.0f, 1.0f, 0, 0);
			float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

			XMFLOAT2 Vel;
			XMStoreFloat2(&Vel, Dir_Down * Speed_Long);

			for (int i = 0; i < 2; ++i)
			{
				XMFLOAT2 Spawn_pos_L = { Turrets[i].x - (Enemy_Bullet_Long_Width * A_Half), Turrets[i].y - (Enemy_Bullet_Long_Height * A_Half) };
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel, 180.0f);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");
		}

		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval)
		{
			boss.Fire_Timer = 0.0;

			XMFLOAT2 Turrets[2] = { D_Turret_L, D_Turret_R };
			float Speed_Spire = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::SPIRE);

			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - Turrets[i].x, Player_Center.y - Turrets[i].y, 0, 0));

				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_To_Player * Speed_Spire);
				float Angle_Spire = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

				XMFLOAT2 spawnPos = { Turrets[i].x - (Enemy_Bullet_Spire_Width * 0.5f), Turrets[i].y - (Enemy_Bullet_Spire_Height * 0.5f) };
				Enemy_Bullet_Spire_creat(spawnPos, Vel, Angle_Spire);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}
		break;
	}
	}
}

void Boss_Pattern_Stage_5_Update(Enemy& boss, double elapsed_time, BOSS_PATTERN_TYPE Type)
{
	patternTimer += elapsed_time;

	float y_offset = sinf(static_cast<float>(patternTimer) * HOVER_SPEED) * HOVER_AMPLITUDE;
	boss.Position.y = boss.Target_POS.y + y_offset;

	const Enemy_Type& Boss_Type = Get_Enemy_Info(boss.Type_ID);

	if (Get_Now_Stage_Flow() != STAGE_FLOW_STATE::GAMEPLAY_ACTIVE || !Player_Is_Alive())
		return;

	XMFLOAT2 U_Turret_L = { boss.Position.x + boss.Size.x * 0.25f, boss.Position.y + boss.Size.y * 0.6f };
	XMFLOAT2 U_Turret_R = { boss.Position.x + boss.Size.x * 0.75f, boss.Position.y + boss.Size.y * 0.6f };
	XMFLOAT2 D_Turret_L = { boss.Position.x + boss.Size.x * 0.4f,  boss.Position.y + boss.Size.y * 0.8f };
	XMFLOAT2 D_Turret_R = { boss.Position.x + boss.Size.x * 0.6f,  boss.Position.y + boss.Size.y * 0.8f };

	XMFLOAT2 Turrets[4] = { U_Turret_L, U_Turret_R, D_Turret_L, D_Turret_R };

	XMFLOAT2 Player_Center = Player_Get_Collision().Center;

	switch (Type)
	{
	case BOSS_PATTERN_TYPE::STAGE_5_PHASE_1:
	{
		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval)
		{
			boss.Fire_Timer = 0.0;

			for (int i = 0; i < 4; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - Turrets[i].x, Player_Center.y - Turrets[i].y, 0, 0));
				float Speed = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);

				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_To_Player * Speed);

				XMFLOAT2 spawnPos = { Turrets[i].x - (Enemy_Bullet_Normal_Width * 0.5f), Turrets[i].y - (Enemy_Bullet_Normal_Height * 0.5f) };
				Enemy_Bullet_Normal_creat(spawnPos, Vel);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}
		break;
	}
	case BOSS_PATTERN_TYPE::STAGE_5_PHASE_2:
	case BOSS_PATTERN_TYPE::STAGE_5_PHASE_3:
	{
		double Interval_Rate = (Type == BOSS_PATTERN_TYPE::STAGE_5_PHASE_2) ? 1.2 : 1.5;

		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval * Interval_Rate)
		{
			boss.Fire_Timer = 0.0;

			XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - U_T[i].x, Player_Center.y - U_T[i].y, 0, 0));

				XMMATRIX Rot_L = XMMatrixRotationZ(XMConvertToRadians(-R_Quarter_Pie));
				XMMATRIX Rot_R = XMMatrixRotationZ(XMConvertToRadians(R_Quarter_Pie));
				XMVECTOR Dir_L = XMVector2TransformCoord(Dir_To_Player, Rot_L);
				XMVECTOR Dir_R = XMVector2TransformCoord(Dir_To_Player, Rot_R);

				float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
				float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

				XMFLOAT2 Vel_L, Vel_R, Vel_Center;
				XMStoreFloat2(&Vel_L, Dir_L * Speed_Normal);
				XMStoreFloat2(&Vel_R, Dir_R * Speed_Normal);
				XMStoreFloat2(&Vel_Center, Dir_To_Player * Speed_Long);

				XMFLOAT2 Spawn_pos_N = { U_T[i].x - (Enemy_Bullet_Normal_Width * A_Half), U_T[i].y - (Enemy_Bullet_Normal_Height * A_Half) };
				XMFLOAT2 Spawn_pos_L = { U_T[i].x - (Enemy_Bullet_Long_Width * A_Half), U_T[i].y - (Enemy_Bullet_Long_Height * A_Half) };

				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R);
				float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");
		}

		D_T_Fire_Timer += elapsed_time;
		if (D_T_Fire_Timer >= 3.0)
		{
			D_T_Fire_Timer = 0.0;

			XMFLOAT2 D_T[2] = { D_Turret_L, D_Turret_R };
			float Speed_Spire = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::SPIRE);

			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - D_T[i].x, Player_Center.y - D_T[i].y, 0, 0));

				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_To_Player * Speed_Spire);
				float Angle_Spire = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

				XMFLOAT2 spawnPos = { D_T[i].x - (Enemy_Bullet_Spire_Width * 0.5f), D_T[i].y - (Enemy_Bullet_Spire_Height * 0.5f) };
				Enemy_Bullet_Spire_creat(spawnPos, Vel, Angle_Spire);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Ligth");
		}
		break;
	}
	case BOSS_PATTERN_TYPE::STAGE_5_PHASE_4:
	{
		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval * 1.5)
		{
			boss.Fire_Timer = 0.0;

			XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - U_T[i].x, Player_Center.y - U_T[i].y, 0, 0));

				XMMATRIX Rot_L = XMMatrixRotationZ(XMConvertToRadians(-R_Quarter_Pie));
				XMMATRIX Rot_R = XMMatrixRotationZ(XMConvertToRadians(R_Quarter_Pie)); 
				XMVECTOR Dir_L = XMVector2TransformCoord(Dir_To_Player, Rot_L);
				XMVECTOR Dir_R = XMVector2TransformCoord(Dir_To_Player, Rot_R);

				float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
				float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

				XMFLOAT2 Vel_L, Vel_R, Vel_Center;
				XMStoreFloat2(&Vel_L, Dir_L * Speed_Normal);
				XMStoreFloat2(&Vel_R, Dir_R * Speed_Normal);
				XMStoreFloat2(&Vel_Center, Dir_To_Player * Speed_Long);

				XMFLOAT2 Spawn_pos_N = { U_T[i].x - (Enemy_Bullet_Normal_Width * A_Half), U_T[i].y - (Enemy_Bullet_Normal_Height * A_Half) };
				XMFLOAT2 Spawn_pos_L = { U_T[i].x - (Enemy_Bullet_Long_Width * A_Half), U_T[i].y - (Enemy_Bullet_Long_Height * A_Half) };

				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R);
				float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");
		}

		U_T_Fire_Timer += elapsed_time;
		if (U_T_Fire_Timer >= Boss_Type.Fire_Interval / 1.5)
		{
			U_T_Fire_Timer = 0.0;

			XMFLOAT2 Corner_L = { Game_Offset.x, static_cast<float>(Game_Screen_Height) };
			XMFLOAT2 Corner_R = { Game_Offset.x + Game_Screen_Width, static_cast<float>(Game_Screen_Height) };

			XMVECTOR Dir_To_Corner_L = XMVector2Normalize(XMVectorSet(Corner_L.x - U_Turret_L.x, Corner_L.y - U_Turret_L.y, 0, 0));
			XMVECTOR Dir_To_Corner_R = XMVector2Normalize(XMVectorSet(Corner_R.x - U_Turret_R.x, Corner_R.y - U_Turret_R.y, 0, 0));

			float Speed_Long_Corner = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);
			XMFLOAT2 Vel_Corner_L, Vel_Corner_R;
			XMStoreFloat2(&Vel_Corner_L, Dir_To_Corner_L * Speed_Long_Corner);
			XMStoreFloat2(&Vel_Corner_R, Dir_To_Corner_R * Speed_Long_Corner);

			float Angle_Corner_L = (atan2f(XMVectorGetY(Dir_To_Corner_L), XMVectorGetX(Dir_To_Corner_L)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
			float Angle_Corner_R = (atan2f(XMVectorGetY(Dir_To_Corner_R), XMVectorGetX(Dir_To_Corner_R)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

			XMFLOAT2 S_Pos_Long_L = { U_Turret_L.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_L.y - (Enemy_Bullet_Long_Height * 0.5f) };
			XMFLOAT2 S_Pos_Long_R = { U_Turret_R.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_R.y - (Enemy_Bullet_Long_Height * 0.5f) };

			Enemy_Bullet_Long_creat(S_Pos_Long_L, Vel_Corner_L, Angle_Corner_L);
			Enemy_Bullet_Long_creat(S_Pos_Long_R, Vel_Corner_R, Angle_Corner_R);

			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}

		D_T_Fire_Timer += elapsed_time;
		if (D_T_Fire_Timer >= 3.0)
		{
			D_T_Fire_Timer = 0.0;

			XMFLOAT2 d_turrets[2] = { D_Turret_L, D_Turret_R };
			float Speed_Spire = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::SPIRE);

			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - d_turrets[i].x, Player_Center.y - d_turrets[i].y, 0, 0));

				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_To_Player * Speed_Spire);
				float Angle_Spire = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

				XMFLOAT2 spawnPos = { d_turrets[i].x - (Enemy_Bullet_Spire_Width * 0.5f), d_turrets[i].y - (Enemy_Bullet_Spire_Height * 0.5f) };
				Enemy_Bullet_Spire_creat(spawnPos, Vel, Angle_Spire);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Ligth");
		}
		break;
	}
	case BOSS_PATTERN_TYPE::STAGE_Final_PHASE_1:
		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval * 1.5)
		{
			boss.Fire_Timer = 0.0;

			XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - U_T[i].x, Player_Center.y - U_T[i].y, 0, 0));

				XMMATRIX Rot_L_30 = XMMatrixRotationZ(XMConvertToRadians(-30.0f));
				XMMATRIX Rot_R_30 = XMMatrixRotationZ(XMConvertToRadians(30.0f));
				XMMATRIX Rot_L_60 = XMMatrixRotationZ(XMConvertToRadians(-60.0f));
				XMMATRIX Rot_R_60 = XMMatrixRotationZ(XMConvertToRadians(60.0f));

				XMVECTOR Dir_L_30 = XMVector2TransformCoord(Dir_To_Player, Rot_L_30);
				XMVECTOR Dir_R_30 = XMVector2TransformCoord(Dir_To_Player, Rot_R_30);
				XMVECTOR Dir_L_60 = XMVector2TransformCoord(Dir_To_Player, Rot_L_60);
				XMVECTOR Dir_R_60 = XMVector2TransformCoord(Dir_To_Player, Rot_R_60);

				float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
				float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

				XMFLOAT2 Vel_L_30, Vel_R_30, Vel_L_60, Vel_R_60, Vel_Center;
				XMStoreFloat2(&Vel_L_30, Dir_L_30 * Speed_Normal);
				XMStoreFloat2(&Vel_R_30, Dir_R_30 * Speed_Normal);
				XMStoreFloat2(&Vel_L_60, Dir_L_60 * Speed_Normal);
				XMStoreFloat2(&Vel_R_60, Dir_R_60 * Speed_Normal);
				XMStoreFloat2(&Vel_Center, Dir_To_Player * Speed_Long);

				XMFLOAT2 Spawn_pos_N = { U_T[i].x - (Enemy_Bullet_Normal_Width * A_Half), U_T[i].y - (Enemy_Bullet_Normal_Height * A_Half) };
				XMFLOAT2 Spawn_pos_L = { U_T[i].x - (Enemy_Bullet_Long_Width * A_Half), U_T[i].y - (Enemy_Bullet_Long_Height * A_Half) };

				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L_30);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R_30);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L_60);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R_60);

				float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");
		}

		U_T_Fire_Timer += elapsed_time;
		if (U_T_Fire_Timer >= Boss_Type.Fire_Interval / 2.0)
		{
			U_T_Fire_Timer = 0.0;

			XMFLOAT2 Corner_L = { 0.0f, static_cast<float>(Game_Screen_Height) };
			XMFLOAT2 Corner_R = { static_cast<float>(SCREEN_WIDTH), static_cast<float>(Game_Screen_Height) };

			XMVECTOR Dir_To_Corner_L = XMVector2Normalize(XMVectorSet(Corner_L.x - U_Turret_L.x, Corner_L.y - U_Turret_L.y, 0, 0));
			XMVECTOR Dir_To_Corner_R = XMVector2Normalize(XMVectorSet(Corner_R.x - U_Turret_R.x, Corner_R.y - U_Turret_R.y, 0, 0));

			float Speed_Long_Corner = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG) * 2.0f;
			XMFLOAT2 Vel_Corner_L, Vel_Corner_R;
			XMStoreFloat2(&Vel_Corner_L, Dir_To_Corner_L * Speed_Long_Corner);
			XMStoreFloat2(&Vel_Corner_R, Dir_To_Corner_R * Speed_Long_Corner);

			float Angle_Corner_L = (atan2f(XMVectorGetY(Dir_To_Corner_L), XMVectorGetX(Dir_To_Corner_L)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
			float Angle_Corner_R = (atan2f(XMVectorGetY(Dir_To_Corner_R), XMVectorGetX(Dir_To_Corner_R)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

			XMFLOAT2 S_Pos_Long_L = { U_Turret_L.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_L.y - (Enemy_Bullet_Long_Height * 0.5f) };
			XMFLOAT2 S_Pos_Long_R = { U_Turret_R.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_R.y - (Enemy_Bullet_Long_Height * 0.5f) };

			Enemy_Bullet_Long_creat(S_Pos_Long_L, Vel_Corner_L, Angle_Corner_L);
			Enemy_Bullet_Long_creat(S_Pos_Long_R, Vel_Corner_R, Angle_Corner_R);

			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}

		D_T_Fire_Timer += elapsed_time;
		if (D_T_Fire_Timer >= 2.5)
		{
			D_T_Fire_Timer = 0.0;

			XMFLOAT2 D_T[2] = { D_Turret_L, D_Turret_R };
			float Speed_Spire = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::SPIRE);

			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - D_T[i].x, Player_Center.y - D_T[i].y, 0, 0));

				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_To_Player * Speed_Spire);
				float Angle_Spire = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

				XMFLOAT2 spawnPos = { D_T[i].x - (Enemy_Bullet_Spire_Width * 0.5f), D_T[i].y - (Enemy_Bullet_Spire_Height * 0.5f) };
				Enemy_Bullet_Spire_creat(spawnPos, Vel, Angle_Spire);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Ligth");
		}
		break;
	
	case BOSS_PATTERN_TYPE::STAGE_Final_PHASE_2:
	{
		boss.Fire_Timer += elapsed_time;
		if (boss.Fire_Timer >= Boss_Type.Fire_Interval * 1.5)
		{
			boss.Fire_Timer = 0.0;

			XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - U_T[i].x, Player_Center.y - U_T[i].y, 0, 0));

				XMMATRIX Rot_L_30 = XMMatrixRotationZ(XMConvertToRadians(-30.0f));
				XMMATRIX Rot_R_30 = XMMatrixRotationZ(XMConvertToRadians(30.0f));
				XMMATRIX Rot_L_60 = XMMatrixRotationZ(XMConvertToRadians(-60.0f));
				XMMATRIX Rot_R_60 = XMMatrixRotationZ(XMConvertToRadians(60.0f));

				XMVECTOR Dir_L_30 = XMVector2TransformCoord(Dir_To_Player, Rot_L_30);
				XMVECTOR Dir_R_30 = XMVector2TransformCoord(Dir_To_Player, Rot_R_30);
				XMVECTOR Dir_L_60 = XMVector2TransformCoord(Dir_To_Player, Rot_L_60);
				XMVECTOR Dir_R_60 = XMVector2TransformCoord(Dir_To_Player, Rot_R_60);

				float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
				float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

				XMFLOAT2 Vel_L_30, Vel_R_30, Vel_L_60, Vel_R_60, Vel_Center;
				XMStoreFloat2(&Vel_L_30, Dir_L_30 * Speed_Normal);
				XMStoreFloat2(&Vel_R_30, Dir_R_30 * Speed_Normal);
				XMStoreFloat2(&Vel_L_60, Dir_L_60 * Speed_Normal);
				XMStoreFloat2(&Vel_R_60, Dir_R_60 * Speed_Normal);
				XMStoreFloat2(&Vel_Center, Dir_To_Player * Speed_Long);

				XMFLOAT2 Spawn_pos_N = { U_T[i].x - (Enemy_Bullet_Normal_Width * A_Half), U_T[i].y - (Enemy_Bullet_Normal_Height * A_Half) };
				XMFLOAT2 Spawn_pos_L = { U_T[i].x - (Enemy_Bullet_Long_Width * A_Half), U_T[i].y - (Enemy_Bullet_Long_Height * A_Half) };

				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L_30);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R_30);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L_60);
				Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R_60);

				float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Big");
		}

		U_T_Fire_Timer += elapsed_time;
		if (U_T_Fire_Timer >= Boss_Type.Fire_Interval / 2.25)
		{
			U_T_Fire_Timer = 0.0;

			XMFLOAT2 Corner_L = { 0.0f, static_cast<float>(Game_Screen_Height) };
			XMFLOAT2 Corner_R = { static_cast<float>(SCREEN_WIDTH), static_cast<float>(Game_Screen_Height) };

			XMVECTOR Dir_To_Corner_L = XMVector2Normalize(XMVectorSet(Corner_L.x - U_Turret_L.x, Corner_L.y - U_Turret_L.y, 0, 0));
			XMVECTOR Dir_To_Corner_R = XMVector2Normalize(XMVectorSet(Corner_R.x - U_Turret_R.x, Corner_R.y - U_Turret_R.y, 0, 0));

			float Speed_Long_Corner = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG) * 2.0f;
			XMFLOAT2 Vel_Corner_L, Vel_Corner_R;
			XMStoreFloat2(&Vel_Corner_L, Dir_To_Corner_L * Speed_Long_Corner);
			XMStoreFloat2(&Vel_Corner_R, Dir_To_Corner_R * Speed_Long_Corner);

			float Angle_Corner_L = (atan2f(XMVectorGetY(Dir_To_Corner_L), XMVectorGetX(Dir_To_Corner_L)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
			float Angle_Corner_R = (atan2f(XMVectorGetY(Dir_To_Corner_R), XMVectorGetX(Dir_To_Corner_R)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

			XMFLOAT2 S_Pos_Long_L = { U_Turret_L.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_L.y - (Enemy_Bullet_Long_Height * 0.5f) };
			XMFLOAT2 S_Pos_Long_R = { U_Turret_R.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_R.y - (Enemy_Bullet_Long_Height * 0.5f) };

			Enemy_Bullet_Long_creat(S_Pos_Long_L, Vel_Corner_L, Angle_Corner_L);
			Enemy_Bullet_Long_creat(S_Pos_Long_R, Vel_Corner_R, Angle_Corner_R);

			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}

		D_T_Fire_Timer += elapsed_time;
		if (D_T_Fire_Timer >= 2.5)
		{
			D_T_Fire_Timer = 0.0;

			XMFLOAT2 d_turrets[2] = { D_Turret_L, D_Turret_R };
			float Speed_Spire = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::SPIRE);

			for (int i = 0; i < 2; ++i)
			{
				XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - d_turrets[i].x, Player_Center.y - d_turrets[i].y, 0, 0));

				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir_To_Player * Speed_Spire);
				float Angle_Spire = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

				XMFLOAT2 spawnPos = { d_turrets[i].x - (Enemy_Bullet_Spire_Width * 0.5f), d_turrets[i].y - (Enemy_Bullet_Spire_Height * 0.5f) };
				Enemy_Bullet_Spire_creat(spawnPos, Vel, Angle_Spire);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Ligth");
		}

		Caging_Fire_Timer += elapsed_time;
		if (Caging_Fire_Timer >= 2.0)
		{
			Caging_Fire_Timer = 0.0;

			XMFLOAT2 Turrets[2] = { U_Turret_L, U_Turret_R };
			XMVECTOR Dir_Down = XMVectorSet(0.0f, 1.0f, 0, 0);
			float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

			XMFLOAT2 Vel;
			XMStoreFloat2(&Vel, Dir_Down * Speed_Long);

			for (int i = 0; i < 2; ++i)
			{
				XMFLOAT2 Spawn_pos_L = { Turrets[i].x - (Enemy_Bullet_Long_Width * A_Half), Turrets[i].y - (Enemy_Bullet_Long_Height * A_Half) };
				Enemy_Bullet_Long_creat(Spawn_pos_L, Vel, 180.0f);
			}
			Sound_M->Play_SFX("Enemy_Shoot_Normal");
		}
		break;
	}

case BOSS_PATTERN_TYPE::STAGE_Final_PHASE_3:
{
	boss.Fire_Timer += elapsed_time;
	if (boss.Fire_Timer >= Boss_Type.Fire_Interval)
	{
		boss.Fire_Timer = 0.0;

		XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
		for (int i = 0; i < 2; ++i)
		{
			XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - U_T[i].x, Player_Center.y - U_T[i].y, 0, 0));

			XMMATRIX Rot_L_30 = XMMatrixRotationZ(XMConvertToRadians(-30.0f));
			XMMATRIX Rot_R_30 = XMMatrixRotationZ(XMConvertToRadians(30.0f));
			XMMATRIX Rot_L_60 = XMMatrixRotationZ(XMConvertToRadians(-60.0f));
			XMMATRIX Rot_R_60 = XMMatrixRotationZ(XMConvertToRadians(60.0f));

			XMVECTOR Dir_L_30 = XMVector2TransformCoord(Dir_To_Player, Rot_L_30);
			XMVECTOR Dir_R_30 = XMVector2TransformCoord(Dir_To_Player, Rot_R_30);
			XMVECTOR Dir_L_60 = XMVector2TransformCoord(Dir_To_Player, Rot_L_60);
			XMVECTOR Dir_R_60 = XMVector2TransformCoord(Dir_To_Player, Rot_R_60);

			float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
			float Speed_Long = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG);

			XMFLOAT2 Vel_L_30, Vel_R_30, Vel_L_60, Vel_R_60, Vel_Center;
			XMStoreFloat2(&Vel_L_30, Dir_L_30 * Speed_Normal);
			XMStoreFloat2(&Vel_R_30, Dir_R_30 * Speed_Normal);
			XMStoreFloat2(&Vel_L_60, Dir_L_60 * Speed_Normal);
			XMStoreFloat2(&Vel_R_60, Dir_R_60 * Speed_Normal);
			XMStoreFloat2(&Vel_Center, Dir_To_Player * Speed_Long);

			XMFLOAT2 Spawn_pos_N = { U_T[i].x - (Enemy_Bullet_Normal_Width * A_Half), U_T[i].y - (Enemy_Bullet_Normal_Height * A_Half) };
			XMFLOAT2 Spawn_pos_L = { U_T[i].x - (Enemy_Bullet_Long_Width * A_Half), U_T[i].y - (Enemy_Bullet_Long_Height * A_Half) };

			Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L_30);
			Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R_30);
			Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_L_60);
			Enemy_Bullet_Normal_creat(Spawn_pos_N, Vel_R_60);

			float Angle = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
			Enemy_Bullet_Long_creat(Spawn_pos_L, Vel_Center, Angle);
		}
		Sound_M->Play_SFX("Enemy_Shoot_Normal");
	}

	U_T_Fire_Timer += elapsed_time;
	if (U_T_Fire_Timer >= Boss_Type.Fire_Interval / 2.25)
	{
		U_T_Fire_Timer = 0.0;

		XMFLOAT2 Corner_L = { 0.0f, static_cast<float>(Game_Screen_Height) };
		XMFLOAT2 Corner_R = { static_cast<float>(SCREEN_WIDTH), static_cast<float>(Game_Screen_Height) };

		XMVECTOR Dir_To_Corner_L = XMVector2Normalize(XMVectorSet(Corner_L.x - U_Turret_L.x, Corner_L.y - U_Turret_L.y, 0, 0));
		XMVECTOR Dir_To_Corner_R = XMVector2Normalize(XMVectorSet(Corner_R.x - U_Turret_R.x, Corner_R.y - U_Turret_R.y, 0, 0));

		float Speed_Long_Corner = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::LONG) * 2.0f;
		XMFLOAT2 Vel_Corner_L, Vel_Corner_R;
		XMStoreFloat2(&Vel_Corner_L, Dir_To_Corner_L * Speed_Long_Corner);
		XMStoreFloat2(&Vel_Corner_R, Dir_To_Corner_R * Speed_Long_Corner);

		float Angle_Corner_L = (atan2f(XMVectorGetY(Dir_To_Corner_L), XMVectorGetX(Dir_To_Corner_L)) + XM_PIDIV2) * (R_One_Pie / XM_PI);
		float Angle_Corner_R = (atan2f(XMVectorGetY(Dir_To_Corner_R), XMVectorGetX(Dir_To_Corner_R)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

		XMFLOAT2 S_Pos_Long_L = { U_Turret_L.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_L.y - (Enemy_Bullet_Long_Height * 0.5f) };
		XMFLOAT2 S_Pos_Long_R = { U_Turret_R.x - (Enemy_Bullet_Long_Width * 0.5f), U_Turret_R.y - (Enemy_Bullet_Long_Height * 0.5f) };

		Enemy_Bullet_Long_creat(S_Pos_Long_L, Vel_Corner_L, Angle_Corner_L);
		Enemy_Bullet_Long_creat(S_Pos_Long_R, Vel_Corner_R, Angle_Corner_R);

		Sound_M->Play_SFX("Enemy_Shoot_Normal");
	}

	D_T_Fire_Timer += elapsed_time;
	if (D_T_Fire_Timer >= 2.0)
	{
		D_T_Fire_Timer = 0.0;

		XMFLOAT2 d_turrets[2] = { D_Turret_L, D_Turret_R };
		float Speed_Spire = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::SPIRE);

		for (int i = 0; i < 2; ++i)
		{
			XMVECTOR Dir_To_Player = XMVector2Normalize(XMVectorSet(Player_Center.x - d_turrets[i].x, Player_Center.y - d_turrets[i].y, 0, 0));

			XMFLOAT2 Vel;
			XMStoreFloat2(&Vel, Dir_To_Player * Speed_Spire);
			float Angle_Spire = (atan2f(XMVectorGetY(Dir_To_Player), XMVectorGetX(Dir_To_Player)) + XM_PIDIV2) * (R_One_Pie / XM_PI);

			XMFLOAT2 spawnPos = { d_turrets[i].x - (Enemy_Bullet_Spire_Width * 0.5f), d_turrets[i].y - (Enemy_Bullet_Spire_Height * 0.5f) };
			Enemy_Bullet_Spire_creat(spawnPos, Vel, Angle_Spire);
		}
		Sound_M->Play_SFX("Enemy_Shoot_Ligth");
	}

	Burst_Fire_Timer += elapsed_time;
	if (Burst_Fire_Timer >= 3.0)
	{
		Burst_Fire_Timer = 0.0;
		XMFLOAT2 U_T[2] = { U_Turret_L, U_Turret_R };
		float Speed_Normal = Enemy_Get_Bullet_Speed(Enemy_Bullet_Type::NORMAL);
		XMFLOAT2 Spawn_POS = {};

		for (int turret_idx = 0; turret_idx < 2; ++turret_idx)
		{
			Spawn_POS = { U_T[turret_idx].x - (Enemy_Bullet_Normal_Width * A_Half), U_T[turret_idx].y - (Enemy_Bullet_Normal_Height * A_Half) };

			for (int i = 0; i < 8; ++i)
			{
				float angle_rad = XMConvertToRadians(static_cast<float>(i * 45));

				XMVECTOR Dir = XMVectorSet(sinf(angle_rad), -cosf(angle_rad), 0, 0);
				XMFLOAT2 Vel;
				XMStoreFloat2(&Vel, Dir * Speed_Normal);
				Enemy_Bullet_Normal_creat(Spawn_POS, Vel);
			}
		}
		Sound_M->Play_SFX("Enemy_Shoot_Big");
	}
	break;
}
	}
}