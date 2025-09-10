/*==============================================================================

	Manage Enemy Pattern [Enemy_Pattern.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENEMY_PATTERN_H
#define ENEMY_PATTERN_H
#include "Enemy.h"
#include "Palette.h"

static constexpr float ENTER_MAX_SPEED_MULTIPLIER = PALETTE::A_Quintuple;
static constexpr float LEAVE_MAX_SPEED_MULTIPLIER = PALETTE::A_One_And_Half;
static constexpr float ACCELERATION_DURATION = PALETTE::A_Origin;
static float MIN_SPEED = PALETTE::A_Decuple * Game_Scale;

static constexpr int Fixed_Loop_MAX = 4;

void Pattern_Down_And_Up(Enemy& enemy, double elapsed_time);
void Pattern_Down_And_Down(Enemy& enemy, double elapsed_time);
void Pattern_Patrol(Enemy& enemy, double elapsed_time);
void Pattern_Chaser(Enemy& enemy, double elapsed_time);
void Pattern_Fixed(Enemy& enemy, double elapsed_time);

void Pattern_Down_Shoot_Down(Enemy& enemy, double elapsed_time);
void Pattern_Down_Shoot_Up(Enemy& enemy, double elapsed_time);
void Pattern_Down_Shoot_Patrol(Enemy& enemy, double elapsed_time);

#endif // ENEMY_PATTERN_H