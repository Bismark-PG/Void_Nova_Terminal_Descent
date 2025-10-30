/*==============================================================================

	Manager Boss Pattern [Boss_Pattern.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BOSS_PATTERN_H
#define BOSS_PATTERN_H

#include "Enemy.h"
#include <Boss_Manager.h>

constexpr float HOVER_AMPLITUDE = 8.0f;
constexpr float HOVER_SPEED = 1.2f;

bool Boss_Pattern_Enter(Enemy& boss, double elapsed_time);
void Boss_Pattern_Initialize(Enemy& boss);

void Boss_Pattern_Stage_3_Update(Enemy& boss, double elapsed_time, BOSS_PATTERN_TYPE Type);
void Boss_Pattern_Stage_4_Update(Enemy& boss, double elapsed_time, BOSS_PATTERN_TYPE Type);
void Boss_Pattern_Stage_5_Update(Enemy& boss, double elapsed_time, BOSS_PATTERN_TYPE Type);

#endif // BOSS_PATTERN_H
