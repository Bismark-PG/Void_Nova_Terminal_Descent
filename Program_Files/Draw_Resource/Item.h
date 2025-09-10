/*==============================================================================

	Manage Item [Item.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ITEM_H
#define ITEM_H

#include <DirectXMath.h>
#include "Collision.h"

constexpr unsigned int ITEM_MAX = 512;

static constexpr int STATUS_UP = 1;
static constexpr int DEFAULT_SCORE_UP = 50;
static constexpr float DEFAULT_POWER_VALUE = 0.1f;
static constexpr int PLAYER_DROP_POWER_LIMIT = 6;
static constexpr float PLAYER_DROP_POWER_VALUE = 1.5;
static constexpr double PLAYER_DROP_LIFE_TIME = 7.0;

static constexpr float ITEM_SIZE = 32.0f;
static constexpr float ITEM_SPEED = 100.0f;
static constexpr float SCORE_SPEED = 200.0f;

static constexpr float ITEM_ATTRACT_ACCELERATION = 1500.0f;
static constexpr float ITEM_ATTRACT_MAX_SPEED = 1000.0f;

static float Item_Size;
static float Item_Speed;
static float Item_Radius;

static float Score_HP_Size;
static float Score_HP_Speed;
static float Score_HP_Radius;

enum class Item_Type
{
	POWER_UP,
	SCORE,
	BOMB,
	LIVE
};

enum class Item_State
{
	NONE,
	FALLING
};

struct Item
{
	bool isEnable;
	Item_Type Type;
	Item_State State;
	float Get_Power;
	int Get_Score;

	float Life_Time;

	DirectX::XMFLOAT2 Size;

	DirectX::XMFLOAT2 Position;
	DirectX::XMFLOAT2 Velocity;

	Circle Collision;
};

void Item_Initialize();
void Item_Finalize();

void Item_Update(double elapsed_time);
void Item_Draw();

void Item_Create(const DirectX::XMFLOAT2& Enemy_Center_Pos, const DirectX::XMFLOAT2& Enemy_Size,
	Item_Type Type, float Power_Value = DEFAULT_POWER_VALUE, int Count = 1);
void Score_Create(const DirectX::XMFLOAT2& Enemy_Center_Pos, const DirectX::XMFLOAT2& Enemy_Size,
	Item_Type Type, int Score_Value = DEFAULT_SCORE_UP, int Count = 1);
void Power_Create_On_Player_Death(const DirectX::XMFLOAT2& Player_Pos);

bool Item_IsEnable(int Index);
Circle Item_GetCollision(int Index);

#endif // ITEM_H
