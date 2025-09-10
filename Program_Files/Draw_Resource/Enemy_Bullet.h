/*==============================================================================

	Make Enemy's Bullet [Enemy_Bullet.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H
#include "Game_Window.h"
#include <DirectXMath.h>
#include "Collision.h"
#include "Palette.h"
#include <vector>

enum class Enemy_Bullet_Type;

struct Enemy_Bullet
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 size;
	DirectX::XMFLOAT2 velocity;
	bool isEnable;
	std::vector<Circle> Collisions;
	float Angle;
	Enemy_Bullet_Type Type;
};

constexpr unsigned int ENEMY_BULLET_MAX = 4096;


static float Enemy_Bullet_Normal_Width;
static float Enemy_Bullet_Normal_Height;
static float Enemy_Bullet_Normal_Radius;
static float Enemy_Bullet_Normal_Speed;

static float Enemy_Bullet_Long_Width;
static float Enemy_Bullet_Long_Height;
static float Enemy_Bullet_Long_Speed;
static constexpr int LONG_BULLET_COLLISION_COUNT = 3;

static float Enemy_Bullet_Spire_Width;
static float Enemy_Bullet_Spire_Height;
static float Enemy_Bullet_Spire_Speed;
static constexpr int SPIRE_BULLET_COLLISION_COUNT = 3;

static constexpr float ENEMY_BULLET_NORMAL_WIDTH  = 16.0f;
static constexpr float ENEMY_BULLET_NORMAL_HEIGHT = 16.0f;
static constexpr float ENEMY_BULLET_NORMAL_SPEED  = 450.0f;

static constexpr float ENEMY_BULLET_LONG_WIDTH  = 8.0f;
static constexpr float ENEMY_BULLET_LONG_HEIGHT = 40.0f;
static constexpr float ENEMY_BULLET_LONG_SPEED  = 450.0f;

static constexpr float ENEMY_BULLET_SPIRE_WIDTH = 16.0f;
static constexpr float ENEMY_BULLET_SPIRE_HEIGHT = 32.0f;
static constexpr float ENEMY_BULLET_SPIRE_SPEED = 600.0f;

void Enemy_Bullet_Initialize();
void Enemy_Bullet_Finalize();

void Enemy_Bullet_Update(double elapsed_time);
void Enemy_Bullet_Normal_Draw();
void Enemy_Bullet_Long_Draw();
void Enemy_Bullet_Spire_Draw();

void Enemy_Bullet_Normal_creat(const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& velocity);
void Enemy_Bullet_Long_creat(const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& velocity, float angle = PALETTE::A_Zero);
void Enemy_Bullet_Spire_creat(const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& velocity, float angle = PALETTE::A_Zero);

void Enemy_Bullet_Destroy(int Index);
void Enemy_Bullet_Destroy_All();

bool Enemy_Bullet_IsEnable(int Index);
bool Enemy_Bullet_CheckCollision(int Index, const Circle& target_collision);

const std::vector<Circle>& Enemy_Bullet_GetCollisions(int Index);
float Enemy_Get_Bullet_Speed(Enemy_Bullet_Type Type);

#endif  //ENEMY_BULLET_H
