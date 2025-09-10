/*==============================================================================

	Make Enemy Pattern [Enemy_Spawner.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Enemy_Spawner.h"
#include "Game_Status.h"
#include <DirectXMath.h>
#include <sstream>
using namespace DirectX;

static int Spawner_Count = 0;
static double Time = 0.0;

void Enemy_Spawner_Initialize()
{
	Enemy_Spawner_Reset();
}

void Enemy_Spawner_Finalize()
{

}

void Enemy_Spawner_Update(double elapsed_time)
{
	if (Status_Is_Game_Over())
	{
		return;
	}

	Time += elapsed_time;

	for (int i = 0; i < Spawner_Count; i++)
	{
		if (Enemy_Spawners[i].Is_Ended)
			continue;

		if (Enemy_Spawners[i].Triger_Time > Time)
			continue;

		if (Enemy_Spawners[i].Spawn_Count == 0)
			Enemy_Spawners[i].Spawn_Time = Time - Enemy_Spawners[i].Rate - 0.00001;
		
		if (Time - Enemy_Spawners[i].Spawn_Time >= Enemy_Spawners[i].Rate)
		{
			Enemy_Create(Enemy_Spawners[i].ID,
				Enemy_Spawners[i].Position,
				Enemy_Spawners[i].Pattern,
				Enemy_Spawners[i].Pattern_Data);

			Enemy_Spawners[i].Spawn_Count++;

			if (Enemy_Spawners[i].Spawn_Count >= Enemy_Spawners[i].Count_MAX)
				Enemy_Spawners[i].Is_Ended = true;

			Enemy_Spawners[i].Spawn_Time = Time;
		}
	}
}

void Enemy_Spawner_Reset()
{
	for (int i = 0; i < Enemy_SPAWNER_MAX; ++i)
		Enemy_Spawners[i] = {};

	Spawner_Count = 0;
	Time = 0.0;
}

void Enemy_Spawner_Create(Enemy_Type_ID ID, const DirectX::XMFLOAT2& Position,
	double Spawn_Triger, double Spawn_Rate, int Spawn_Count,
	Enemy_Move_Pattern Pattern,
	const PATTERN_DATA& Data)
{
	if (Spawner_Count >= Enemy_SPAWNER_MAX) return;

	Enemy_Spawn* Enemy = &Enemy_Spawners[Spawner_Count];
	Enemy->ID = ID;
	Enemy->Pattern = Pattern;
	Enemy->Pattern_Data = Data;
	Enemy->Position = Position;
	Enemy->Triger_Time = Spawn_Triger;
	Enemy->Rate = Spawn_Rate;
	Enemy->Count_MAX = Spawn_Count;
	Enemy->Is_Ended = false;
	Enemy->Spawn_Count = 0;
	Enemy->Spawn_Time = 0.0;

	Spawner_Count++;

#if defined(DEBUG) || defined(_DEBUG)
	std::stringstream Debug;
	Debug << "Enemy Pattern : " << static_cast<int>(Pattern) << std::endl;
	Debug << "Enemy POS : " << Position.x << " , " << Position.y << std::endl;
	Debug << "Triger(Spawn) Time : " << Spawn_Triger << std::endl;
	Debug << "Triger Rate Time : " << Spawn_Rate << std::endl;
	Debug << "Count : " << Spawn_Count << std::endl;
	Debug << "Is Ended : " << Enemy->Is_Ended << std::endl;
	OutputDebugStringA(Debug.str().c_str());
#endif	
}

bool Enemy_Spawner_Is_Done()
{
	if (Spawner_Count == 0) return true;

	for (int i = 0; i < Spawner_Count; i++)
	{
		if (!Enemy_Spawners[i].Is_Ended)
			return false;
	}

	return true;
}