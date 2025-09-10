/*==============================================================================

	Make Bullet [Bullet.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef BULLET_H
#define BULLET_H

#include "Game_Window.h"
#include <DirectXMath.h>
#include "direct3d.h"
#include "texture.h"
#include "sprite.h"
#include "Collision.h"
#include "Palette.h"

struct Bullet
{
	XMFLOAT2 position;
	XMFLOAT2 size;
	XMFLOAT2 velocity;
	double lifeTime;
	bool isEnable;
	Circle Collision;
};

constexpr unsigned int BULLET_MAX = 2048;

static float Bullet_Width;
static float Bullet_Height;
static float Bullet_Radius;
static float Bullet_Speed;

static float Laser_Width;
static float Laser_Height;
static float Laser_Radius;
static float Laser_Speed;

static constexpr float BULLET_WIDTH  = 8.0f;
static constexpr float BULLET_HEIGHT = 16.0f;
static constexpr float BULLET_SPEED  = 600.0f;

static constexpr float LASER_WIDTH  = 8.0f;
static constexpr float LASER_HEIGHT = 40.0f;
static constexpr float LASER_SPEED  = 800.0f;

static float Bullet_Damage_For_Power;
static constexpr float BULLET_DAMAGE_Lv_1 = 0.75f;
static constexpr float BULLET_DAMAGE_Lv_2 = 0.4f;
static constexpr float BULLET_DAMAGE_Lv_3 = 0.3f;
static constexpr float BULLET_DAMAGE_LV_4 = 0.2f;
static constexpr float BULLET_DAMAGE_MAX = 0.25f;
static constexpr float LASER_DAMAGE  = 0.5f;
static constexpr float BOMB_DAMAGE   = 10.0f;


void Bullet_Initialize();
void Bullet_Finalize();

void Bullet_Update(double elapsed_time);

void Bullet_Draw();
void Laser_Draw();

void Bullet_Create(const DirectX::XMFLOAT2& position);
void Laser_Create(const DirectX::XMFLOAT2& position);

bool Bullet_IsEnable(int Index);
Circle Bullet_GetCollision(int Index);

void Bullet_Destroy(int Index);
void Bullet_Destroy_All_Enemy_Bullets();

float Bullet_Get_Now_Damage();

#endif  //BULLET_H