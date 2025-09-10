/*==============================================================================

	Make Enemy Pattern [Enemy_Spawner.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENEMY_SPAWNER_H
#define ENEMY_SPAWNER_H

#include <DirectXMath.h>
#include "Enemy.h"

struct Enemy_Spawn
{
	Enemy_Type_ID ID;
	Enemy_Move_Pattern Pattern;

	PATTERN_DATA Pattern_Data;

	XMFLOAT2 Position;
	double Triger_Time;
	double Rate;

	int Count_MAX;
	bool Is_Ended;

	int Spawn_Count;
	double Spawn_Time;
};

constexpr unsigned int Enemy_SPAWNER_MAX = 100;
static Enemy_Spawn Enemy_Spawners[Enemy_SPAWNER_MAX]{};

void Enemy_Spawner_Initialize();
void Enemy_Spawner_Finalize();

void Enemy_Spawner_Update(double elapsed_time);
void Enemy_Spawner_Reset();

void Enemy_Spawner_Create(Enemy_Type_ID ID, const DirectX::XMFLOAT2& position,
	double Spawn_Triger, double Spawn_Rate, int Spawn_Count,
	Enemy_Move_Pattern Pattern = Enemy_Move_Pattern::Down_And_Up,
	const PATTERN_DATA& data = PATTERN_DATA());

bool Enemy_Spawner_Is_Done();

#endif  //ENEMY_SPAWNER_H