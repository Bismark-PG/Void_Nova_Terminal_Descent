/*==============================================================================

	Game Logic [Game_Logic.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

void Game_Logic_Initialize();
void Game_Logic_Finalize();

void Game_Logic_Update(double elapsed_time);
void Game_Logic_Playing_Story_Update(double elapsed_time);
void Game_Logic_Player_Respawn_Update(double elapsed_time);

void Game_Logic_Draw();

void AABB_Bullet_VS_Enemy();
void AABB_Player_VS_Enemy();
void AABB_Player_VS_EnemyBullet();

#endif // GAME_LOGIC_H