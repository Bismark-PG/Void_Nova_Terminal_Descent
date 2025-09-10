/*==============================================================================

	Manage Enemy Pattern [Enemy_Pattern.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Enemy_Pattern.h"
#include <DirectXMath.h>
#include "Player.h"

using namespace PALETTE;
using namespace DirectX;


static XMFLOAT2 GetBulletSize(Enemy_Bullet_Type type)
{
	switch (type)
	{
	case Enemy_Bullet_Type::NORMAL:
		return { Enemy_Bullet_Normal_Width, Enemy_Bullet_Normal_Height };

	case Enemy_Bullet_Type::LONG:
		return { Enemy_Bullet_Long_Width, Enemy_Bullet_Long_Height };

	case Enemy_Bullet_Type::SPIRE:
		return { Enemy_Bullet_Spire_Width, Enemy_Bullet_Spire_Height };

	default:
		return { 0.0f, 0.0f };
	}
}

static XMFLOAT2 Set_Bullet_Spawn_Pos(const Enemy& enemy, Enemy_Bullet_Type BulletType)
{
	XMFLOAT2 bulletSize = GetBulletSize(BulletType);

	return{ enemy.Position.x + (enemy.Size.x * A_Half) - (bulletSize.x * A_Half),
			enemy.Position.y + (enemy.Size.y * A_F_Fifths) - (bulletSize.y * A_Half) };
}

void Pattern_Down_And_Up(Enemy& enemy, double elapsed_time)
{
	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else // Arrived at target
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Wait;
			enemy.State_Timer = 0.0;
		}
		break;
	}

	case Enemy_State::Wait:
		if (enemy.State_Timer > 0.5)
		{
			enemy.State = Enemy_State::Attack;
			enemy.State_Timer = 0.0;
			enemy.Fire_Timer = Get_Enemy_Info(enemy.Type_ID).Fire_Interval;
		}
		break;

	case Enemy_State::Attack:
		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;
			enemy.Now_Fire_Count++;

			if (Player_Is_Alive())
			{
				XMFLOAT2 Player_POS = Player_Get_Position();
				float Player_Center_X = Player_POS.x + (Player_Width * A_Half);
				float Player_Center_Y = Player_POS.y + (Player_Height * A_Half);

				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVector2Normalize(XMVectorSet(Player_Center_X - Enemy_Bullet_Spawn_Pos.x, Player_Center_Y - Enemy_Bullet_Spawn_Pos.y, 0, 0));
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);
				float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;

				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
				{
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;
				}
				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}

			if (enemy.Now_Fire_Count >= Get_Enemy_Info(enemy.Type_ID).Fire_Count)
			{
				enemy.State_Timer = 0.0;
				enemy.Target_POS = { enemy.Position.x, -enemy.Size.y };
				enemy.State = Enemy_State::Leave;
			}
		}
		break;

	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);

		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}

	XMStoreFloat2(&enemy.Position, POS);
}

// Enemy Pattern Logic >> Down And Down
void Pattern_Down_And_Down(Enemy& enemy, double elapsed_time)
{
	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else // Arrived at target
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Wait;
			enemy.State_Timer = 0.0;
		}
		break;
	}

	case Enemy_State::Wait:
		if (enemy.State_Timer > 0.5)
		{
			enemy.State = Enemy_State::Attack;
			enemy.State_Timer = 0.0;
			enemy.Fire_Timer = Get_Enemy_Info(enemy.Type_ID).Fire_Interval;
		}
		break;

	case Enemy_State::Attack:
		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;
			enemy.Now_Fire_Count++;

			if (Player_Is_Alive())
			{
				XMFLOAT2 Player_POS = Player_Get_Position();
				float Player_Center_X = Player_POS.x + (PLAYER_WIDTH * A_Half);
				float Player_Center_Y = Player_POS.y + (PLAYER_HEIGHT * A_Half);

				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVector2Normalize(XMVectorSet(Player_Center_X - Enemy_Bullet_Spawn_Pos.x, Player_Center_Y - Enemy_Bullet_Spawn_Pos.y, 0, 0));
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);
				float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
				
				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
				{
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;
				}
				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}

			if (enemy.Now_Fire_Count >= Get_Enemy_Info(enemy.Type_ID).Fire_Count)
			{
				enemy.State_Timer = 0.0;
				enemy.Target_POS = { enemy.Position.x, Game_Screen_Height + enemy.Size.y };
				enemy.State = Enemy_State::Leave;
			}
		}
		break;

	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);

		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}
	XMStoreFloat2(&enemy.Position, POS);
}

// Enemy Pattern Logic >> Patrol
void Pattern_Patrol(Enemy& enemy, double elapsed_time)
{
	if (enemy.State != Enemy_State::Leave && enemy.Now_Patrol_Time >= Get_Enemy_Info(enemy.Type_ID).Patrol_Time)
	{
		enemy.State_Timer = 0.0;
		enemy.Target_POS = { enemy.Position.x, -enemy.Size.y };
		enemy.State = Enemy_State::Leave;
	}

	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Action;
			enemy.State_Timer = 0.0;
			float Initial_Direction = static_cast<float>(enemy.Pattern_Data.Patrol_Direction);
			enemy.Velocity = { enemy.Speed * Initial_Direction, 0.0f };
		}
		break;
	}

	case Enemy_State::Action:
	{
		enemy.Now_Patrol_Time += static_cast<float>(elapsed_time);
		POS += XMLoadFloat2(&enemy.Velocity) * static_cast<float>(elapsed_time);

		XMFLOAT2 Current_POS;
		XMStoreFloat2(&Current_POS, POS);

		if (Current_POS.x < Game_Offset.x || Current_POS.x > Game_Offset.x + Game_Screen_Width - enemy.Size.x)
		{
			enemy.Velocity.x *= -1;
			Current_POS.x = std::max(Game_Offset.x, std::min(Current_POS.x, Game_Offset.x + Game_Screen_Width - enemy.Size.x));
			POS = XMLoadFloat2(&Current_POS);
		}

		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;

			if (Player_Is_Alive())
			{
				XMFLOAT2 Player_POS = Player_Get_Position();
				float Player_Center_X = Player_POS.x + (PLAYER_WIDTH * A_Half);
				float Player_Center_Y = Player_POS.y + (PLAYER_HEIGHT * A_Half);

				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVector2Normalize(XMVectorSet(Player_Center_X - Enemy_Bullet_Spawn_Pos.x, Player_Center_Y - Enemy_Bullet_Spawn_Pos.y, 0, 0));
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);
				float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;

				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
				{
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;
				}
				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}
		}
		break;
	}
	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);

		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}
	XMStoreFloat2(&enemy.Position, POS);
}

void Pattern_Chaser(Enemy& enemy, double elapsed_time)
{
	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else // Arrived at target
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Wait;
			enemy.State_Timer = 0.0;
		}
		break;
	}

	case Enemy_State::Wait:
		if (enemy.State_Timer > 0.5)
		{
			enemy.State = Enemy_State::Attack;
			enemy.State_Timer = 0.0;
			enemy.Fire_Timer = Get_Enemy_Info(enemy.Type_ID).Fire_Interval;
		}
		break;

	case Enemy_State::Attack:
		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;
			enemy.Now_Fire_Count++;

			if (Player_Is_Alive())
			{
				XMFLOAT2 Player_POS = Player_Get_Position();
				float Player_Center_X = Player_POS.x + (PLAYER_WIDTH * A_Half);
				float Player_Center_Y = Player_POS.y + (PLAYER_HEIGHT * A_Half);

				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVector2Normalize(XMVectorSet(Player_Center_X - Enemy_Bullet_Spawn_Pos.x, Player_Center_Y - Enemy_Bullet_Spawn_Pos.y, 0, 0));
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);
				float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;

				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
				{
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;
				}
				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}

			if (enemy.Now_Fire_Count >= Get_Enemy_Info(enemy.Type_ID).Fire_Count)
			{
				enemy.State_Timer = 0.0;

				if (Player_Is_Alive())
				{
					XMFLOAT2 Player_POS = Player_Get_Position();
					XMVECTOR Enemy_Vec = XMLoadFloat2(&enemy.Position);
					XMVECTOR Player_Vec = XMLoadFloat2(&Player_POS);
					XMVECTOR Direcion_Vec = XMVector2Normalize(Player_Vec - Enemy_Vec);
					XMVECTOR Final_Pos_Vec = Enemy_Vec + Direcion_Vec * (Game_Screen_Height * 1.5f);
					XMStoreFloat2(&enemy.Target_POS, Final_Pos_Vec);

					enemy.Angle = atan2f(XMVectorGetY(Direcion_Vec), XMVectorGetX(Direcion_Vec)) + XM_PIDIV2 + XM_PI;
				}
				else
					enemy.Target_POS = { enemy.Position.x, Game_Screen_Height + enemy.Size.y };

				enemy.State = Enemy_State::Leave;
			}
		}
		break;

	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);
		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}
	XMStoreFloat2(&enemy.Position, POS);
}

void Pattern_Fixed(Enemy& enemy, double elapsed_time)
{
	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else // Arrived at target
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Wait;
			enemy.State_Timer = 0.0;
		}
		break;
	}

	case Enemy_State::Wait:
		if (enemy.State_Timer > 0.5)
		{
			enemy.State = Enemy_State::Attack;
			enemy.State_Timer = 0.0;
			enemy.Fire_Timer = Get_Enemy_Info(enemy.Type_ID).Fire_Interval;
		}
		break;

	case Enemy_State::Action:
		if (enemy.State_Timer > (Get_Enemy_Info(enemy.Type_ID).Fire_Interval) * A_Double)
		{
			enemy.State = Enemy_State::Attack;
			enemy.State_Timer = 0.0;
			enemy.Fire_Timer = Get_Enemy_Info(enemy.Type_ID).Fire_Interval;
		}
		break;

	case Enemy_State::Attack:
		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;
			enemy.Now_Fire_Count++;

			if (Player_Is_Alive())
			{
				XMFLOAT2 Player_POS = Player_Get_Position();
				float Player_Center_X = Player_POS.x + (PLAYER_WIDTH * A_Half);
				float Player_Center_Y = Player_POS.y + (PLAYER_HEIGHT * A_Half);

				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVector2Normalize(XMVectorSet(Player_Center_X - Enemy_Bullet_Spawn_Pos.x, Player_Center_Y - Enemy_Bullet_Spawn_Pos.y, 0, 0));
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);
				float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
				
				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
				{
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;
				}
				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}

			if (enemy.Now_Fire_Count >= Get_Enemy_Info(enemy.Type_ID).Fire_Count)
			{
				enemy.State_Timer = 0.0;

				if (enemy.Fix_Pattern_Loop_Count < Fixed_Loop_MAX)
				{
					enemy.Fix_Pattern_Loop_Count++;
					enemy.Now_Fire_Count = 0;
					enemy.State = Enemy_State::Action;
				}
				else
				{
					enemy.Target_POS = { enemy.Position.x, Game_Screen_Height + enemy.Size.y };
					enemy.State = Enemy_State::Leave;
				}
			}
		}
		break;

	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);

		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}
	XMStoreFloat2(&enemy.Position, POS);
}

void Pattern_Down_Shoot_Down(Enemy& enemy, double elapsed_time)
{
	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else // Arrived at target
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Wait;
			enemy.State_Timer = 0.0;
		}
		break;
	}

	case Enemy_State::Wait:
		if (enemy.State_Timer > 0.5)
		{
			enemy.State = Enemy_State::Attack;
			enemy.State_Timer = 0.0;
			enemy.Fire_Timer = Get_Enemy_Info(enemy.Type_ID).Fire_Interval;
		}
		break;

	case Enemy_State::Attack:
		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;
			enemy.Now_Fire_Count++;

			if (Player_Is_Alive())
			{
				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);


				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;

				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}

			if (enemy.Now_Fire_Count >= Get_Enemy_Info(enemy.Type_ID).Fire_Count)
			{
				enemy.State_Timer = 0.0;
				enemy.Target_POS = { enemy.Position.x, Game_Screen_Height + enemy.Size.y };
				enemy.State = Enemy_State::Leave;
			}
		}
		break;

	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);

		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}
	XMStoreFloat2(&enemy.Position, POS);
}

void Pattern_Down_Shoot_Up(Enemy& enemy, double elapsed_time)
{
	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else // Arrived at target
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Wait;
			enemy.State_Timer = 0.0;
		}
		break;
	}

	case Enemy_State::Wait:
		if (enemy.State_Timer > 0.5)
		{
			enemy.State = Enemy_State::Attack;
			enemy.State_Timer = 0.0;
			enemy.Fire_Timer = Get_Enemy_Info(enemy.Type_ID).Fire_Interval;
		}
		break;

	case Enemy_State::Attack:
		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;
			enemy.Now_Fire_Count++;

			if (Player_Is_Alive())
			{
				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);


				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;

				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}

			if (enemy.Now_Fire_Count >= Get_Enemy_Info(enemy.Type_ID).Fire_Count)
			{
				enemy.State_Timer = 0.0;
				enemy.Target_POS = { enemy.Position.x, -enemy.Size.y };
				enemy.State = Enemy_State::Leave;
			}
		}
		break;

	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);

		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}
	XMStoreFloat2(&enemy.Position, POS);
}

void Pattern_Down_Shoot_Patrol(Enemy& enemy, double elapsed_time)
{
	if (enemy.State != Enemy_State::Leave && enemy.Now_Patrol_Time >= Get_Enemy_Info(enemy.Type_ID).Patrol_Time)
	{
		enemy.State_Timer = 0.0;
		enemy.Target_POS = { enemy.Position.x, -enemy.Size.y };
		enemy.State = Enemy_State::Leave;
	}

	XMVECTOR POS = XMLoadFloat2(&enemy.Position);
	enemy.State_Timer += elapsed_time;

	switch (enemy.State)
	{
	case Enemy_State::Enter:
	{
		XMVECTOR Target_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Target = Target_Pos - POS;
		XMVECTOR Distance_V = XMVector2Length(To_Target);
		float Distance = XMVectorGetX(Distance_V);

		if (Distance > A_Origin)
		{
			float Ratio = std::min(Distance / (Game_Screen_Height * A_Half), A_Origin);
			float Now_Speed = enemy.Speed * ENTER_MAX_SPEED_MULTIPLIER * Ratio;
			Now_Speed = std::max(Now_Speed, MIN_SPEED);

			XMVECTOR Velocity = XMVector2Normalize(To_Target) * Now_Speed * static_cast<float>(elapsed_time);
			POS += Velocity;
		}
		else
		{
			POS = Target_Pos;
			enemy.State = Enemy_State::Action;
			enemy.State_Timer = 0.0;
			float Initial_Direction = static_cast<float>(enemy.Pattern_Data.Patrol_Direction);
			enemy.Velocity = { enemy.Speed * Initial_Direction, 0.0f };
		}
		break;
	}

	case Enemy_State::Action:
	{
		enemy.Now_Patrol_Time += static_cast<float>(elapsed_time);
		POS += XMLoadFloat2(&enemy.Velocity) * static_cast<float>(elapsed_time);

		XMFLOAT2 Current_POS;
		XMStoreFloat2(&Current_POS, POS);

		if (Current_POS.x < Game_Offset.x || Current_POS.x > Game_Offset.x + Game_Screen_Width - enemy.Size.x)
		{
			enemy.Velocity.x *= -1;
			Current_POS.x = std::max(Game_Offset.x, std::min(Current_POS.x, Game_Offset.x + Game_Screen_Width - enemy.Size.x));
			POS = XMLoadFloat2(&Current_POS);
		}

		enemy.Fire_Timer += elapsed_time;
		if (enemy.Fire_Timer >= Get_Enemy_Info(enemy.Type_ID).Fire_Interval)
		{
			enemy.Fire_Timer = 0.0;
			
			if (Player_Is_Alive())
			{
				Enemy_Bullet_Type Bullet_Type = Get_Enemy_Info(enemy.Type_ID).Bullet_Type;
				XMFLOAT2 Enemy_Bullet_Spawn_Pos = Set_Bullet_Spawn_Pos(enemy, Bullet_Type);

				XMVECTOR D_Vector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
				float B_Speed = Enemy_Get_Bullet_Speed(Bullet_Type);


				switch (Bullet_Type)
				{
				case Enemy_Bullet_Type::NORMAL:
					XMFLOAT2 N_Velocity;
					XMStoreFloat2(&N_Velocity, D_Vector * B_Speed);
					Enemy_Bullet_Normal_creat(Enemy_Bullet_Spawn_Pos, N_Velocity);
					break;

				case Enemy_Bullet_Type::LONG:
				{
					XMFLOAT2 L_Velocity;
					XMStoreFloat2(&L_Velocity, D_Vector * B_Speed);
					float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
					Enemy_Bullet_Long_creat(Enemy_Bullet_Spawn_Pos, L_Velocity, Angle);
					break;
				}
				case Enemy_Bullet_Type::SPIRE:
				{
					XMFLOAT2 S_Velocity;
					XMStoreFloat2(&S_Velocity, D_Vector * B_Speed);
					float Angle = atan2f(XMVectorGetY(D_Vector), XMVectorGetX(D_Vector)) + XM_PIDIV2;
					Enemy_Bullet_Spire_creat(Enemy_Bullet_Spawn_Pos, S_Velocity, Angle);
					break;
				}
				}
			}
		}
		break;
	}
	case Enemy_State::Leave:
	{
		float Max_Speed = enemy.Speed * LEAVE_MAX_SPEED_MULTIPLIER;
		float Ratio = static_cast<float>(enemy.State_Timer) / ACCELERATION_DURATION;
		Ratio = std::min(Ratio, A_Origin);

		float Now_Speed = Max_Speed * Ratio;

		XMVECTOR Leave_Pos = XMLoadFloat2(&enemy.Target_POS);
		XMVECTOR To_Leave = XMVector2Normalize(Leave_Pos - POS);
		XMVECTOR Velocity = To_Leave * Now_Speed * static_cast<float>(elapsed_time);
		POS += Velocity;
		break;
	}
	}
	XMStoreFloat2(&enemy.Position, POS);
}
