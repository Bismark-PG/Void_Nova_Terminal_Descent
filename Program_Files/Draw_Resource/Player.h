/*==============================================================================

	Player [Player.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef PLAYER_H
#define PLAYER_H

#include "Game_Window.h"
#include <DirectXMath.h>
#include "direct3d.h"
#include "Texture.h"
#include "Sprite_Animation.h"
#include "KeyLogger.h"
#include "Bullet.h"
#include "Effect.h"
#include "Collision.h"
#include "Palette.h"
#include "Game_Status.h" 

enum class PLAYER_STATE
{
	NONE,
	LEFT,
	RIGHT
};

static float Player_Width;
static float Player_Height;
static float Player_Radius;
static float Player_Item_Radius;
static float Player_Avoid_Radius;

static float Player_Speed;
static float Player_Speed_Original;
static float Player_Speed_Avoid_Mode;

static DirectX::XMFLOAT2 Player_Pre_POS;

static constexpr float PLAYER_WIDTH = 64.0f;
static constexpr float PLAYER_HEIGHT = 96.0f;
static constexpr float PLAYER_SPEED_ORIGINAL  = 225.0f;
static constexpr float PLAYER_SPEED_AVOIDANCE = 75.0f;
static constexpr float PLAYER_ALPHA_ORIGINAL = 1.0f;
static constexpr float PLAYER_ALPHA_RE_SPAWN = 0.5f;

static float Laser_Width_Half;
static float Bullet_Width_Half;

static constexpr float POWER_2_BULLET_L = 0.25f;
static constexpr float POWER_2_BULLET_R = 0.75f;
static constexpr float POWER_4_BULLET_L_1 = 0.2f;
static constexpr float POWER_4_BULLET_L_2 = 0.4f;
static constexpr float POWER_4_BULLET_R_1 = 0.6f;
static constexpr float POWER_4_BULLET_R_2 = 0.8f;
static constexpr float POWER_MAX_BULLET_L_1 = 0.1f;
static constexpr float POWER_MAX_BULLET_L_2 = 0.3f;
static constexpr float POWER_MAX_BULLET_R_1 = 0.7f;
static constexpr float POWER_MAX_BULLET_R_2 = 0.9f;

static constexpr double POWER_MIN_FIRE_INTERVAL = 0.25;
static constexpr double POWER_2_FIRE_INTERVAL = 0.2;
static constexpr double POWER_3_FIRE_INTERVAL = 0.175;
static constexpr double POWER_4_FIRE_INTERVAL = 0.15;
static constexpr double POWER_MAX_BULLET_FIRE_INTERVAL = 0.1;
static constexpr double POWER_MAX_LASER_FIRE_INTERVAL = 0.125;

static constexpr double BOMB_COOLDOWN_TIME = 5.0;

static constexpr float PLAYER_ANGLE = 0;

void Player_Initialize();
void Player_Finalize();

void Player_Update(double elapsed_time);

void Player_Spawn(const DirectX::XMFLOAT2& Position);
void Player_Reset_For_Story();
void Player_Draw();

bool Player_Is_Alive();
void Player_Set_Avoid_State(bool State);
Circle Player_Get_Collision();
Circle Player_Get_Item_Collision();

void Player_Destroy();

void Player_Set_Position(const DirectX::XMFLOAT2& Position);
DirectX::XMFLOAT2 Player_Get_Position();

void Player_Set_State(PLAYER_STATE State);
void Player_Set_Enable(bool is_enable);

double Get_Bomb_Timer();

#endif // PLAYER_H
