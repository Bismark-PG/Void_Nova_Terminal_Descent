/*==============================================================================

	Manager Boss Pattern [Boss_Pattern.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BOSS_PATTERN_H
#define BOSS_PATTERN_H

#include "Enemy.h"

constexpr float HOVER_AMPLITUDE = 8.0f;
constexpr float HOVER_SPEED = 1.2f;
constexpr double PHASE_CHANGE_FREEZE_TIME = 2.0;

void Boss_Pattern_Enter(Enemy& boss, double elapsed_time);
void Boss_Pattern_Initialize(Enemy& boss);

void Boss_Pattern_Stage_3_Update(Enemy& boss, double elapsed_time);
void Boss_Pattern_Stage_4_Update(Enemy& boss, double elapsed_time);
void Boss_Pattern_Stage_5_Update(Enemy& boss, double elapsed_time);

#endif // BOSS_PATTERN_H
