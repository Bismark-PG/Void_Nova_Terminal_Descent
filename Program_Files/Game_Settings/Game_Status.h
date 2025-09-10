/*==============================================================================

	Manage Player Lives, Score, Bombs, Power, etc [Game_Status.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_STATUS_H
#define GAME_STATUS_H

static constexpr int PLAYER_LIVES_DEFAULT = 3;
static constexpr int PLAYER_LIVES_MAX = 5;

static constexpr int PLAYER_BOMBS_DEFAULT = 2;
static constexpr int PLAYER_BOMBS_MAX = 5;

static constexpr float PLAYER_POWER_DEFAULT = 0.0f;
static constexpr float PLAYER_POWER_MAX = 4.0f;
static constexpr float PLAYER_POWER_MIN = 0.0f;

static constexpr double PLAYER_BOMB_INVINCIBLE_TIME = 2.0;
static constexpr double PLAYER_RESPAWN_INVINCIBLE_TIME = 4.0;

void Status_Initialize();
void Status_Finalize();
void Status_Update(double elapsed_time);

int Status_Get_Lives();
void Status_Add_Lives(int amount);
void Status_Lose_Life();
bool Status_Is_Game_Over();

float Status_Get_Power();
void Status_Add_Power(float amount);
void Status_Reset_Power();

int Status_Get_Bombs();
void Status_Add_Bomb(int amount);
bool Status_Use_Bomb();
void Status_Reset_Bomb();

bool Status_Is_Player_Invincible();
bool Status_Is_Bomb_Invincible();
void Status_Start_Respawn_Invincibility();
double Status_Get_Invincible_Time();

#endif // GAME_STATUS_H
