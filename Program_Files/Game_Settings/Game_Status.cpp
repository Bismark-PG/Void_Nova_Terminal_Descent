/*==============================================================================

	Manage Player Lives, Score, Bombs, Power, etc [Game_Status.h]

	Author : Choi HyungJoon

==============================================================================*/
#include <algorithm>
#include "Game_Status.h"
#include "Score.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Audio_Manager.h"

static int	 Player_Lives;
static float Player_Power;
static int   Player_Bombs;

static bool Is_Bomb_Triggerd = false;
static double Player_Invincible_Timer = 0.0;

static constexpr int Score_Increse = 1000;

void Status_Initialize()
{
	Player_Lives = PLAYER_LIVES_DEFAULT;
	Player_Power = PLAYER_POWER_DEFAULT;
	Player_Bombs = PLAYER_BOMBS_DEFAULT;
	Player_Invincible_Timer = 0.0;
	Is_Bomb_Triggerd = false;
}

void Status_Finalize()
{
	Player_Lives = PLAYER_LIVES_DEFAULT;
	Player_Power = PLAYER_POWER_DEFAULT;
	Player_Bombs = PLAYER_BOMBS_DEFAULT;
	Player_Invincible_Timer = 0.0;
	Is_Bomb_Triggerd = false;
}

void Status_Update(double elapsed_time)
{
	if (Player_Invincible_Timer > 0.0)
	{
		Player_Invincible_Timer -= elapsed_time;
		if (Player_Invincible_Timer <= 0.0)
			Is_Bomb_Triggerd = false;
	}
}

// Lives Logic
int Status_Get_Lives()
{
	return Player_Lives;
}

void Status_Add_Lives(int amount)
{
	Player_Lives += amount;
	Player_Lives = std::min(PLAYER_LIVES_MAX, Player_Lives);
}

void Status_Lose_Life()
{
	if (Status_Is_Player_Invincible()) return;

	if (Player_Lives > 0)
		Player_Lives--;

	Status_Reset_Power();
	Status_Reset_Bomb();
}

bool Status_Is_Game_Over()
{
	return Player_Lives <= 0;
}

// Power Logic
float Status_Get_Power()
{
	return Player_Power;
}

void Status_Add_Power(float amount)
{
	if (Player_Power >= PLAYER_POWER_MAX)
	{
		Score_Increase(static_cast<int>(Score_Increse * amount));
		return;
	}

	int Power_Before = static_cast<int>(Player_Power);

	Player_Power += amount;
	Player_Power = std::max(PLAYER_POWER_MIN, std::min(PLAYER_POWER_MAX, Player_Power));

	int Power_After = static_cast<int>(Player_Power);

	if (Power_After > Power_Before && Power_After == static_cast<int>(PLAYER_POWER_MAX))
		SM->Play_SFX("Player_Get_Power_Max");
}

void Status_Reset_Power()
{
	Player_Power = PLAYER_POWER_DEFAULT;
}

// Bombs Logic
int Status_Get_Bombs()
{
	return Player_Bombs;
}

void Status_Add_Bomb(int amount)
{
	Player_Bombs += amount;
	Player_Bombs = std::min(PLAYER_BOMBS_MAX, Player_Bombs);
}

bool Status_Use_Bomb()
{
	if (Player_Bombs > 0)
	{
		Player_Bombs--;
		Player_Invincible_Timer = PLAYER_BOMB_INVINCIBLE_TIME;
		Is_Bomb_Triggerd = true;
		return true;
	}

	return false;
}

void Status_Reset_Bomb()
{
	Player_Bombs = PLAYER_BOMBS_DEFAULT;
}

bool Status_Is_Player_Invincible()
{
	return Player_Invincible_Timer > 0.0;
}

bool Status_Is_Bomb_Invincible()
{
	return Status_Is_Player_Invincible() && Is_Bomb_Triggerd;
}

double Status_Get_Invincible_Time()
{
	return Player_Invincible_Timer;
}

void Status_Start_Respawn_Invincibility()
{
	Player_Invincible_Timer = PLAYER_RESPAWN_INVINCIBLE_TIME;
	Is_Bomb_Triggerd = false;
}
