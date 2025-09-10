/*==============================================================================

	Make Enemy [Enemy.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef ENEMY_H
#define ENEMY_H

#include <DirectXMath.h>
#include "Collision.h"
#include "Enemy_Bullet.h"

constexpr unsigned int ENEMY_MAX = 512;

static float ENEMY_Original_Width;
static float ENEMY_Original_Height;
static float ENEMY_Original_Radius;
static float ENEMY_Original_Speed;

constexpr float ENEMY_ORIGIN_HP	     = 10.0f;
constexpr float ENEMY_ORIGIN_ZAKO_HP = 5.0f;
constexpr float ENEMY_MINI_BOSS_HP = 50.0f;

constexpr float ENEMY_ORIGIN_WIDTH   = 64.0f;
constexpr float ENEMY_ORIGIN_HEIGHT  = 64.0f;
constexpr float ENEMY_ORIGIN_SPEED	 = 150.0f;
constexpr int ENEMY_ORIGIN_FIRE_COUNT = 5;
constexpr int ENEMY_ORIGIN_BOSS_FIRE_COUNT = 3;

static float ENEMY_Saber_Width;
static float ENEMY_Saber_Height;
static float ENEMY_Saber_Radius;
static float ENEMY_Saber_Speed;

constexpr float ENEMY_SABER_HP			 = 5.0f;
constexpr float ENEMY_SABER_ZAKO_HP		 = 2.5f;

constexpr float ENEMY_SABER_WIDTH_SCALE  = 64.0f;
constexpr float ENEMY_SABER_HEIGHT_SCALE = 51.2f;
constexpr float ENEMY_SABER_SPEED		 = 200.0f;
constexpr int ENEMY_SABER_FIRE_COUNT = 3;

static float ENEMY_Spear_Width;
static float ENEMY_Spear_Height;
static float ENEMY_Spear_Radius;
static float ENEMY_Spear_Speed;

constexpr float ENEMY_SPEAR_HP			 = 3.0f;
constexpr float ENEMY_SPEAR_ZAKO_HP		 = 1.5f;

constexpr float ENEMY_SPEAR_WIDTH_SCALE  = 42.6f;
constexpr float ENEMY_SPEAR_HEIGHT_SCALE = 64.0f;
constexpr float ENEMY_SPEAR_SPEED		 = 300.0f;
constexpr int ENEMY_SPEAR_FIRE_COUNT = 2;

static float ENEMY_Armor_Width;
static float ENEMY_Armor_Height;
static float ENEMY_Armor_Radius;
static float ENEMY_Armor_Speed;

constexpr float ENEMY_ARMOR_HP			 = 25.0f;
constexpr float ENEMY_ARMOR_BOSS_HP		 = 100.0f;

constexpr float ENEMY_ARMOR_WIDTH_SCALE  = 92.0f;
constexpr float ENEMY_ARMOR_HEIGHT_SCALE = 92.0f;
constexpr float ENEMY_ARMOR_SPEED		 = 100.0f;
constexpr int ENEMY_ARMOR_FIRE_COUNT = 10;
constexpr int ENEMY_ARMOR_BOSS_FIRE_COUNT = 5;

static float ENEMY_Special_BOSS_Width;
static float ENEMY_Special_BOSS_Height;
static float ENEMY_Special_BOSS_Radius;
static float ENEMY_Special_BOSS_Speed;

constexpr float ENEMY_SPECAIL_HP		   = 150.0f;
constexpr float ENEMY_SPECAIL_WIDTH		   = 200.0f;
constexpr float ENEMY_SPECAIL_HEIGHT	   = 300.0;
constexpr float ENEMY_SPECAIL_SPEED		   = 100.0f;
constexpr int ENEMY_SPECAIL_BOSS_FIRE_COUNT = 10;

static float ENEMY_Middle_BOSS_Width;
static float ENEMY_Middle_BOSS_Height;
static float ENEMY_Middle_BOSS_Radius;
static float ENEMY_Middle_BOSS_Speed;

constexpr float ENEMY_MIDDLE_BOSS_HP		   = 125.0f;
constexpr float ENEMY_MIDDLE_BOSS_WIDTH		   = 250.0f;
constexpr float ENEMY_MIDDLE_BOSS_HEIGHT	   = 400.0f;
constexpr float ENEMY_MIDDLE_BOSS_SPEED		   = 100.0f;
constexpr int ENEMY_MIDDLE_BOSS_FIRE_COUNT = 20;

static float ENEMY_Final_BOSS_Width;
static float ENEMY_Final_BOSS_Height;
static float ENEMY_Final_BOSS_Radius;
static float ENEMY_Final_BOSS_Speed;

constexpr float ENEMY_FINAL_PHASE_1_HP		  = 300.f;
constexpr float ENEMY_FINAL_PHASE_2_HP		  = 500.f;
constexpr float ENEMY_FINAL_BOSS_BOSS_WIDTH   = 600.f;
constexpr float ENEMY_FINAL_BOSS_BOSS_HEIGHT  = 350.f;
constexpr float ENEMY_FINAL_BOSS_SPEED		  = 100.0f;
constexpr int ENEMY_FINAL_BOSS_BOSS_FIRE_COUNT = 30;

constexpr double ENEMY_FIRE_RATE_NORMAL	 = 1.0;
constexpr double ENEMY_FIRE_RATE_MINI_BOSS  = 1.5;
constexpr double ENEMY_FIRE_RATE_SABER	 = 0.5;
constexpr double ENEMY_FIRE_RATE_SPEAR	 = 0.2;
constexpr double ENEMY_FIRE_RATE_ARMOR	 = 1.2;
constexpr double ENEMY_FIRE_RATE_ARMOR_BOSS	 = 2.0;
constexpr double ENEMY_FIRE_RATE_SPECAIL_BOSS	= 4.0;
constexpr double ENEMY_FIRE_RATE_MIDDLE_BOSS	= 3.0;
constexpr double ENEMY_FIRE_RATE_FINAL_BOSS		= 2.5;

constexpr double ENEMY_PATROL_TIME_NORMAL = 5.5;
constexpr double ENEMY_PATROL_TIME_MINI_BOSS  = 20.0;
constexpr double ENEMY_PATROL_TIME_SABER  = 5.0;
constexpr double ENEMY_PATROL_TIME_SPEAR  = 2.0;
constexpr double ENEMY_PATROL_TIME_ARMOR  = 10.0;

enum class Enemy_Move_Pattern
{
	NONE,
	Down_And_Up,
	Down_Patrol,
	Down_And_Down,
	Down_And_Chase_Player,
	Down_Fixed,
	Down_Shoot_Down,
	Down_Shoot_Up,
	Down_Shoot_Patrol
};

enum class Enemy_State
{
	Enter,		
	Wait,		
	Action,		
	Attack,		
	Leave,
	DESTRUCTION
};

enum Enemy_Type_ID : int
{
	Enemy_Type_Origin = 0,
	Enemy_Type_Origin_ZAKO,
	Enemy_Type_Mini_Boss,

	Enemy_Type_Saber,
	Enemy_Type_Saber_ZAKO,

	Enemy_Type_Spear,
	Enemy_Type_Spear_ZAKO,
	Enemy_Type_Spear_ZAKO_ITEM,

	Enemy_Type_Armor,
	Enemy_Type_Armor_Boss,
	Enemy_Type_Armor_Give_HP,

	Enemy_Type_Special_Boss_Stage_3,
	Enemy_Type_Middle_Boss_Stage_4,

	Enemy_Type_Final_Phase_1_Boss,
	Enemy_Type_Final_Phase_2_Boss,

	Enemy_Type_MAX
};

enum class Enemy_Bullet_Type
{
	NORMAL,
	LONG,
	SPIRE
};

enum Enemy_Give_Score : int
{
	Enemy_BOMB_Score = 25,

	Enemy_Type_Origin_Socre = 250,
	Enemy_Type_Origin_Zako_Socre = 50,
	Enemy_Mini_BOSS_Socre = 500,
	Enemy_Mini_BOSS_Bonus_Score  = 100,

	Enemy_Type_Saber_Socre  = 300,
	Enemy_Saber_Bouns_Score	= 150,

	Enemy_Type_Spear_Socre  = 500,

	Enemy_Type_Armor_Socre  = 350,
	Enemy_Armor_Bouns_Score = 200,
	Enemy_Armor_BOSS_Bonus_Score  = 1000,

	Enemy_Type_Special_Boss_Score = 10000,
	Enemy_Type_Special_Boss_Phase_Change_Score = 100,

	Enemy_Type_Middle_Boss_Score = 15000,
	Enemy_Type_Middle_Boss_Phase_Change_Score = 500,

	Enemy_Type_Final_P_1_Boss_Score = 30000,
	Enemy_Type_Final_P_2_Boss_Score = 50000,
	Enemy_Type_Final_Boss_Phase_Change_Score = 1000,
};

enum class Patrol_Direction
{
	Straight = 0,
	Left = -1,   
	Right = 1    
};

struct Enemy_Type
{
	float HP;

	int TexID;
	int Cut_X, Cut_Y, Texture_W, Texture_H;

	XMFLOAT2 Velocity;
	Circle Collision;
	float Speed;

	double Fire_Interval;
	int Fire_Count;
	Enemy_Bullet_Type Bullet_Type;

	float Patrol_Time;

	int Give_Score = 0;
};

struct PATTERN_DATA
{
	float Target_Y;

	Patrol_Direction Patrol_Direction;

	PATTERN_DATA()
		: Target_Y(Game_Screen_Height * PALETTE::A_Quarter),
		Patrol_Direction(Patrol_Direction::Straight)
	{}
};

struct Enemy
{
	int Type_ID;

	XMFLOAT2 Position;
	XMFLOAT2 Size;

	XMFLOAT2 Velocity; // Input In Enemy_Type, Can Make Speed which Enemy
	float Speed;
	float Angle;
	float HP;

	int Current_Phase;
	int Max_Phase;

	bool isEnable;
	bool isDamage;

	PATTERN_DATA Pattern_Data;
	Enemy_Move_Pattern Move_Pattern;
	Enemy_State State;

	double State_Timer;
	double Fire_Timer;
	double Phase_Change_Timer;

	int Now_Fire_Count;
	float Now_Patrol_Time;
	int Fix_Pattern_Loop_Count;

	int Get_Score;

	XMFLOAT2 Target_POS;
	XMFLOAT2 Leave_POS;
};

void Enemy_Initialize();
void Enemy_Finalize();
void Enemy_Reset();
	
void Enemy_Update(double elapsed_time);
void Enemy_Draw();

int Enemy_Create(Enemy_Type_ID ID, const DirectX::XMFLOAT2& Position, Enemy_Move_Pattern Pattern, const PATTERN_DATA& data, const XMFLOAT2& targetPos = { 0.0f, 0.0f });

void Enemy_Create_Destruction_Shell(int Index);
bool Enemy_Go_To_Next_Phase(int Boss_Index);
int Enemy_Get_Current_Phase(int Boss_Index);

// True : Width, False : Height
float Enemy_Get_Width(Enemy_Type_ID ID, bool Is_Width);
void Enemy_Give_Score(int Index);

bool Enemy_IsEnable(int Index);
Circle Enemy_GetCollision(int Index);

const Enemy* Enemy_Get(int Index);

// Make Damage
void Enemy_Damage(int Index);
void Enemy_Bomb_Damage(int Index, float Damage);
void Enemy_Destroy(int Index);
void Enemy_Destroy_All_Normal_Enemies();

Enemy_Type Get_Enemy_Info(int Type_ID);

#	endif  //ENEMY_H