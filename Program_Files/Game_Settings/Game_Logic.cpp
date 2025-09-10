/*==============================================================================

	Game Logic [Game_Logic.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Game_Logic.h"
#include "Game_Status.h" 
#include "Bullet.h"
#include "Enemy.h"
#include "Enemy_Bullet.h"
#include "Player.h"
#include "Game_Back.h"
#include "Enemy_Spawner.h"
#include "Score.h" 
#include "Debug_Collision.h"
#include "Audio_Manager.h"
#include "Item.h"
#include "Stage_Update.h"

void Game_Logic_Initialize()
{
	Game_Back_Initialize();
	Status_Initialize();
	Item_Initialize();

	Player_Initialize();
	Bullet_Initialize();

	Enemy_Initialize();
	Enemy_Bullet_Initialize();
	Enemy_Spawner_Initialize();

	Effect_Initialize();
	Debug_Collision_Initialize();
}

void Game_Logic_Finalize()
{
	Debug_Collision_Finalize();
	Effect_Finalize();

	Enemy_Spawner_Finalize();
	Enemy_Bullet_Finalize();
	Enemy_Finalize();

	Bullet_Finalize();
	Player_Finalize();

	Item_Finalize();
	Status_Finalize();
	Game_Back_Finalize();
}

void Game_Logic_Update(double elapsed_time)
{
	Score_Update(elapsed_time);
	Enemy_Spawner_Update(elapsed_time);
	Enemy_Update(elapsed_time);
	Enemy_Bullet_Update(elapsed_time);
	Game_Boss_UI_Update(elapsed_time);

	Item_Update(elapsed_time);
	Bullet_Update(elapsed_time);
	AABB_Bullet_VS_Enemy();

	Game_UI_Update_Bomb_Effect(elapsed_time);

	if (Player_Is_Alive())
	{
		Player_Update(elapsed_time);
		AABB_Player_VS_EnemyBullet();
		AABB_Player_VS_Enemy();
	}

	Effect_Update(elapsed_time);
}

void Game_Logic_Playing_Story_Update(double elapsed_time)
{
	Enemy_Update(elapsed_time);
	Enemy_Bullet_Update(elapsed_time);
	Game_Boss_UI_Update(elapsed_time);
	Game_UI_Update_Bomb_Effect(elapsed_time);

	Item_Update(elapsed_time);
	Bullet_Update(elapsed_time);

	Effect_Update(elapsed_time);
}

void Game_Logic_Player_Respawn_Update(double elapsed_time)
{
	Enemy_Spawner_Update(elapsed_time);
	Enemy_Update(elapsed_time);
	Enemy_Bullet_Update(elapsed_time);
	Game_Boss_UI_Update(elapsed_time);
	Game_UI_Update_Bomb_Effect(elapsed_time);

	if (Get_Boss_State() != BOSS_STATE::INACTIVE)
		Boss_Update(elapsed_time);

	Item_Update(elapsed_time);
	Bullet_Update(elapsed_time);
	AABB_Bullet_VS_Enemy();

	Effect_Update(elapsed_time);
}

void Game_Logic_Draw()
{
	Bullet_Draw();
	Laser_Draw();
	Item_Draw();
	Player_Draw();

	Enemy_Draw();
	Enemy_Bullet_Long_Draw();
	Enemy_Bullet_Normal_Draw();
	Enemy_Bullet_Spire_Draw();

	Effect_Draw();
	Game_Boss_UI_Draw();
}

void AABB_Bullet_VS_Enemy()
{
	for (int B_Index = 0; B_Index < BULLET_MAX; B_Index++)
	{
		// AABB For Bullet
		if (!Bullet_IsEnable(B_Index)) continue;

		for (int E_Index = 0; E_Index < ENEMY_MAX; E_Index++)
		{
			// AABB For Enemy
			if (!Enemy_IsEnable(E_Index)) continue;

			// IF AABB Success (True)
			if (Collision_IsOverlapCircle
			(Bullet_GetCollision(B_Index), Enemy_GetCollision(E_Index)))
			{
				Bullet_Destroy(B_Index);
				Enemy_Damage(E_Index);

				SM->Play_SFX("Bullet_Hit");

				if (!Enemy_IsEnable(E_Index))
					Enemy_Give_Score(E_Index);
				break;
			}
		}
	}
}

void AABB_Player_VS_Enemy()
{
	if (!Player_Is_Alive() || Status_Is_Player_Invincible()) return;

	for (int E_Index = 0; E_Index < ENEMY_MAX; E_Index++)
	{
		if (!Enemy_IsEnable(E_Index))
			continue;

		if (Collision_IsOverlapCircle
		(Player_Get_Collision(), Enemy_GetCollision(E_Index)))
		{
			Status_Lose_Life();
			Player_Destroy();
			break;
		}
	}
}

void AABB_Player_VS_EnemyBullet()
{
	for (int i = 0; i < ENEMY_BULLET_MAX; i++)
	{
		if (Enemy_Bullet_CheckCollision(i, Player_Get_Collision()))
		{
			if (!Player_Is_Alive() || Status_Is_Player_Invincible())
			{
				Enemy_Bullet_Destroy(i);
				return;
			}

			Enemy_Bullet_Destroy(i);
			Status_Lose_Life();
			Player_Destroy();
			break;
		}
	}
}