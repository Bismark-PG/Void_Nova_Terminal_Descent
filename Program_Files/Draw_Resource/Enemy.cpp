/*==============================================================================

	Make Enemy [Enemy.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Enemy.h"
#include <DirectXMath.h>
#include "direct3d.h"
#include "texture.h"
#include "sprite.h"
#include "Effect.h"
#include "Player.h"
#include "Palette.h"
#include "Game_Window.h"
#include "Score.h"
#include "Enemy_Pattern.h"
#include "Audio_Manager.h"
#include "Item.h"

using namespace DirectX;
using namespace PALETTE;

static Enemy Enemy_s[ENEMY_MAX]{};

static int Type_Origin = -1;
static int Type_Saber  = -1;
static int Type_Spear  = -1;
static int Type_Armor  = -1;
static int Type_Stage_3_Boss = -1;
static int Type_Stage_4_Boss = -1;
static int Type_Final_Phase_1_Boss = -1;
static int Type_Final_Phase_2_Boss = -1;

static Enemy_Type EnemyType[Enemy_Type_MAX]
{
	// HP, TexID, X, Y, W, H, { Velocity.x, y },
	// { { Collision.x, Collision.y }, Collision.Radius },
	// Speed,  FireInterval, FireCount, BulletType,
	// Patrol_Time, TargetPOS, GetScore }
};

void Enemy_Initialize()
{
	Enemy_Reset();

	Type_Origin = Texture_Load(L"Resource/Texture/Enemy/Enemy_Original_Black_Fixed.png");
	Type_Saber = Texture_Load(L"Resource/Texture/Enemy/Enemy_Saber_Black.png");
	Type_Spear = Texture_Load(L"Resource/Texture/Enemy/Enemy_Spear_Origin.png");
	Type_Armor = Texture_Load(L"Resource/Texture/Enemy/Enemy_Armor.png");
	Type_Stage_3_Boss = Texture_Load(L"Resource/Texture/Enemy/Enemy_Middle_Original.png");
	Type_Stage_4_Boss = Texture_Load(L"Resource/Texture/Enemy/Enemy_Middle_2_Black.png");
	Type_Final_Phase_1_Boss = Texture_Load(L"Resource/Texture/Enemy/Enemy_Boss_W_Fixed.png");
	Type_Final_Phase_2_Boss = Texture_Load(L"Resource/Texture/Enemy/Enemy_Boss_B_Fixed.png");

	ENEMY_Original_Width  = ENEMY_ORIGIN_WIDTH * Game_Scale;
	ENEMY_Original_Height = ENEMY_ORIGIN_HEIGHT * Game_Scale;
	ENEMY_Original_Radius = (ENEMY_Original_Width + ENEMY_Original_Height) * A_Quarter;
	ENEMY_Original_Speed  = ENEMY_ORIGIN_SPEED * Game_Scale;

	ENEMY_Saber_Width	= ENEMY_SABER_WIDTH_SCALE * Game_Scale;
	ENEMY_Saber_Height	= ENEMY_SABER_HEIGHT_SCALE * Game_Scale;
	ENEMY_Saber_Radius	= (ENEMY_Saber_Width + ENEMY_Saber_Height) * A_Quarter;
	ENEMY_Saber_Speed	= ENEMY_SABER_SPEED * Game_Scale;

	ENEMY_Spear_Width	= ENEMY_SPEAR_WIDTH_SCALE * Game_Scale;
	ENEMY_Spear_Height	= ENEMY_SPEAR_HEIGHT_SCALE * Game_Scale;
	ENEMY_Spear_Radius	= (ENEMY_Spear_Width + ENEMY_Spear_Height) * A_Quarter;
	ENEMY_Spear_Speed	= ENEMY_SPEAR_SPEED * Game_Scale;

	ENEMY_Armor_Width  = ENEMY_ARMOR_WIDTH_SCALE * Game_Scale;
	ENEMY_Armor_Height = ENEMY_ARMOR_HEIGHT_SCALE * Game_Scale;
	ENEMY_Armor_Radius = (ENEMY_Armor_Width + ENEMY_Armor_Height) * A_Quarter;
	ENEMY_Armor_Speed  = ENEMY_ARMOR_SPEED * Game_Scale;

	ENEMY_Special_BOSS_Width  = ENEMY_SPECAIL_WIDTH  * Game_Scale;
	ENEMY_Special_BOSS_Height = ENEMY_SPECAIL_HEIGHT * Game_Scale;
	ENEMY_Special_BOSS_Radius = (ENEMY_Special_BOSS_Width + ENEMY_Special_BOSS_Width) * A_Quarter;
	ENEMY_Special_BOSS_Speed  = ENEMY_ARMOR_SPEED * Game_Scale;

	ENEMY_Middle_BOSS_Width  = ENEMY_MIDDLE_BOSS_WIDTH  * Game_Scale;
	ENEMY_Middle_BOSS_Height = ENEMY_MIDDLE_BOSS_HEIGHT * Game_Scale;
	ENEMY_Middle_BOSS_Radius = (ENEMY_Middle_BOSS_Width + ENEMY_Middle_BOSS_Width) * A_Quarter;
	ENEMY_Middle_BOSS_Speed  = ENEMY_ARMOR_SPEED * Game_Scale;

	ENEMY_Final_BOSS_Width  = ENEMY_FINAL_BOSS_BOSS_WIDTH * Game_Scale;
	ENEMY_Final_BOSS_Height = ENEMY_FINAL_BOSS_BOSS_HEIGHT * Game_Scale;
	ENEMY_Final_BOSS_Radius = (ENEMY_Final_BOSS_Height + ENEMY_Final_BOSS_Height) * A_Quarter;
	ENEMY_Final_BOSS_Speed  = ENEMY_FINAL_BOSS_SPEED * Game_Scale;

	// EnemyType [Type_ID] - HP, TexID, UV_X, UV_Y, UV_W, UV_H, { Velocity.x, y },
	// { { Collision.x, Collision.y }, Collision.Radius },
	// Speed,  FireInterval, FireCount, BulletType,
	// Patrol_Time, TargetPOS, GetScore }

	// Original
	EnemyType[Enemy_Type_Origin] = { ENEMY_ORIGIN_HP, Type_Origin, 0, 0, 0, 0, { A_Zero, ENEMY_Original_Speed },
	{ { ENEMY_Original_Width * A_Half, ENEMY_Original_Height * A_Half }, ENEMY_Original_Radius },
		ENEMY_Original_Speed,  ENEMY_FIRE_RATE_NORMAL, ENEMY_ORIGIN_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		ENEMY_PATROL_TIME_NORMAL, Enemy_Give_Score::Enemy_Type_Origin_Socre };
	
	EnemyType[Enemy_Type_Origin_ZAKO] = { ENEMY_ORIGIN_ZAKO_HP, Type_Origin, 0, 0, 0, 0, { A_Zero, ENEMY_Original_Speed },
	{ { ENEMY_Original_Width * A_Half, ENEMY_Original_Height * A_Half }, ENEMY_Original_Radius },
		ENEMY_Original_Speed,  ENEMY_FIRE_RATE_NORMAL, ENEMY_ORIGIN_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		ENEMY_PATROL_TIME_NORMAL, Enemy_Give_Score::Enemy_Type_Origin_Zako_Socre };

	EnemyType[Enemy_Type_Mini_Boss] = { ENEMY_MINI_BOSS_HP, Type_Origin, 0, 0, 0, 0, { A_Zero, ENEMY_Original_Speed },
	{ { ENEMY_Armor_Width * A_Half, ENEMY_Armor_Height * A_Half }, ENEMY_Armor_Radius },
		ENEMY_Armor_Speed,  ENEMY_FIRE_RATE_MINI_BOSS, ENEMY_ORIGIN_BOSS_FIRE_COUNT, Enemy_Bullet_Type::LONG,
		ENEMY_PATROL_TIME_NORMAL, Enemy_Give_Score::Enemy_Mini_BOSS_Socre };

	// Saber
	EnemyType[Enemy_Type_Saber] = { ENEMY_SABER_HP, Type_Saber, 0, 0, 0, 0, { A_Zero, ENEMY_Saber_Speed },
	{ { ENEMY_Saber_Width * A_Half, ENEMY_Saber_Height * A_Half }, ENEMY_Saber_Radius },
		ENEMY_Saber_Speed,  ENEMY_FIRE_RATE_SABER, ENEMY_SABER_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		ENEMY_PATROL_TIME_SABER, Enemy_Give_Score::Enemy_Type_Saber_Socre };

	EnemyType[Enemy_Type_Saber_ZAKO] = { ENEMY_SABER_ZAKO_HP, Type_Saber, 0, 0, 0, 0, { A_Zero, ENEMY_Saber_Speed },
	{ { ENEMY_Saber_Width * A_Half, ENEMY_Saber_Height * A_Half }, ENEMY_Saber_Radius },
		ENEMY_Saber_Speed,  ENEMY_FIRE_RATE_SABER, ENEMY_SABER_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		ENEMY_PATROL_TIME_SABER, Enemy_Give_Score::Enemy_Type_Saber_Socre };

	// Spear
	EnemyType[Enemy_Type_Spear] = { ENEMY_SPEAR_HP, Type_Spear, 0, 0, 0, 0, { A_Zero, ENEMY_SPEAR_SPEED },
	{ { ENEMY_Spear_Width * A_Half, ENEMY_Spear_Height * A_Half }, ENEMY_Spear_Radius },
		ENEMY_SPEAR_SPEED,  ENEMY_FIRE_RATE_SPEAR, ENEMY_SPEAR_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		ENEMY_PATROL_TIME_SPEAR, Enemy_Give_Score::Enemy_Type_Spear_Socre };

	EnemyType[Enemy_Type_Spear_ZAKO] = { ENEMY_SPEAR_ZAKO_HP, Type_Spear, 0, 0, 0, 0, { A_Zero, ENEMY_SPEAR_SPEED },
	{ { ENEMY_Spear_Width * A_Half, ENEMY_Spear_Height * A_Half }, ENEMY_Spear_Radius },
		ENEMY_SPEAR_SPEED,  ENEMY_FIRE_RATE_SPEAR, ENEMY_SPEAR_FIRE_COUNT, Enemy_Bullet_Type::SPIRE,
		ENEMY_PATROL_TIME_SPEAR, Enemy_Give_Score::Enemy_Type_Spear_Socre };

	EnemyType[Enemy_Type_Spear_ZAKO_ITEM] = { ENEMY_SPEAR_ZAKO_HP, Type_Spear, 0, 0, 0, 0, { A_Zero, ENEMY_Saber_Speed },
	{ { ENEMY_Saber_Width * A_Half, ENEMY_Saber_Height * A_Half }, ENEMY_Saber_Radius },
		ENEMY_Saber_Speed,  ENEMY_FIRE_RATE_SABER, ENEMY_SABER_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		ENEMY_PATROL_TIME_SABER, Enemy_Give_Score::Enemy_Type_Saber_Socre };

	// Armor
	EnemyType[Enemy_Type_Armor] = { ENEMY_ARMOR_HP, Type_Armor, 0, 0, 0, 0, { A_Zero, ENEMY_Armor_Speed },
	{ { ENEMY_Armor_Width * A_Half, ENEMY_Armor_Height * A_Half }, ENEMY_Armor_Radius },
		ENEMY_Armor_Speed,  ENEMY_FIRE_RATE_ARMOR, ENEMY_ARMOR_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		ENEMY_PATROL_TIME_ARMOR, Enemy_Give_Score::Enemy_Type_Armor_Socre };

	EnemyType[Enemy_Type_Armor_Boss] = { ENEMY_ARMOR_BOSS_HP, Type_Armor, 0, 0, 0, 0, { A_Zero, ENEMY_Armor_Speed },
	{ { ENEMY_Armor_Width * A_Half, ENEMY_Armor_Height * A_Half }, ENEMY_Armor_Radius },
		ENEMY_Armor_Speed,  ENEMY_FIRE_RATE_ARMOR_BOSS, ENEMY_ARMOR_BOSS_FIRE_COUNT, Enemy_Bullet_Type::LONG,
		ENEMY_PATROL_TIME_ARMOR, Enemy_Give_Score::Enemy_Type_Armor_Socre };

	EnemyType[Enemy_Type_Armor_Give_HP] = { ENEMY_ARMOR_HP, Type_Armor, 0, 0, 0, 0, { A_Zero, ENEMY_Armor_Speed },
	{ { ENEMY_Armor_Width * A_Half, ENEMY_Armor_Height * A_Half }, ENEMY_Armor_Radius },
		ENEMY_Armor_Speed,  ENEMY_FIRE_RATE_ARMOR, ENEMY_ARMOR_FIRE_COUNT, Enemy_Bullet_Type::LONG,
		ENEMY_PATROL_TIME_ARMOR, Enemy_Give_Score::Enemy_Type_Armor_Socre };

	EnemyType[Enemy_Type_Special_Boss_Stage_3] = { ENEMY_SPECAIL_HP, Type_Stage_3_Boss, 0, 0, 0, 0, { A_Zero, ENEMY_Special_BOSS_Speed },
	{ { ENEMY_Special_BOSS_Width * A_Half, ENEMY_Special_BOSS_Height * A_Half }, ENEMY_Special_BOSS_Radius },
		ENEMY_Special_BOSS_Speed,  ENEMY_FIRE_RATE_SPECAIL_BOSS, ENEMY_SPECAIL_BOSS_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		0, Enemy_Give_Score::Enemy_Type_Special_Boss_Score };

	EnemyType[Enemy_Type_Middle_Boss_Stage_4] = { ENEMY_MIDDLE_BOSS_HP, Type_Stage_4_Boss, 0, 0, 0, 0, { A_Zero, ENEMY_Middle_BOSS_Speed },
	{ { ENEMY_Middle_BOSS_Width * A_Half, ENEMY_Middle_BOSS_Height * A_Half }, ENEMY_Middle_BOSS_Radius },
		ENEMY_Middle_BOSS_Speed,  ENEMY_FIRE_RATE_MIDDLE_BOSS, ENEMY_MIDDLE_BOSS_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		0, Enemy_Give_Score::Enemy_Type_Middle_Boss_Score };

	EnemyType[Enemy_Type_Final_Phase_1_Boss] = { ENEMY_FINAL_PHASE_1_HP, Type_Final_Phase_2_Boss, 0, 0, 0, 0, { A_Zero, ENEMY_Final_BOSS_Speed },
	{ { ENEMY_Final_BOSS_Width * A_Half, ENEMY_Final_BOSS_Height * A_Half }, ENEMY_Final_BOSS_Radius },
		ENEMY_Final_BOSS_Speed,  ENEMY_FIRE_RATE_FINAL_BOSS, ENEMY_FINAL_BOSS_BOSS_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		0, Enemy_Give_Score::Enemy_Type_Final_P_1_Boss_Score };

	EnemyType[Enemy_Type_Final_Phase_2_Boss] = { ENEMY_FINAL_PHASE_1_HP, Type_Final_Phase_2_Boss, 0, 0, 0, 0, { A_Zero, ENEMY_Final_BOSS_Speed },
	{ { ENEMY_Final_BOSS_Width * A_Half, ENEMY_Final_BOSS_Height * A_Half }, ENEMY_Final_BOSS_Radius },
		ENEMY_Final_BOSS_Speed,  ENEMY_FIRE_RATE_FINAL_BOSS, ENEMY_FINAL_BOSS_BOSS_FIRE_COUNT, Enemy_Bullet_Type::NORMAL,
		0, Enemy_Give_Score::Enemy_Type_Final_P_2_Boss_Score };
}

void Enemy_Finalize()
{
	Enemy_Reset();

	ENEMY_Original_Width  = 0;
	ENEMY_Original_Height = 0;
	ENEMY_Original_Radius = 0;
	ENEMY_Original_Speed  = 0;

	ENEMY_Saber_Width  = 0;
	ENEMY_Saber_Height = 0;
	ENEMY_Saber_Radius = 0;
	ENEMY_Saber_Speed  = 0;

	ENEMY_Spear_Width  = 0;
	ENEMY_Spear_Height = 0;
	ENEMY_Spear_Radius = 0;
	ENEMY_Spear_Speed  = 0;

	ENEMY_Armor_Width  = 0;
	ENEMY_Armor_Height = 0;
	ENEMY_Armor_Radius = 0;
	ENEMY_Armor_Speed  = 0;

	ENEMY_Special_BOSS_Width  = 0;
	ENEMY_Special_BOSS_Height = 0;
	ENEMY_Special_BOSS_Radius = 0;
	ENEMY_Special_BOSS_Speed  = 0;

	ENEMY_Middle_BOSS_Width	 = 0;
	ENEMY_Middle_BOSS_Height = 0;
	ENEMY_Middle_BOSS_Radius = 0;
	ENEMY_Middle_BOSS_Speed	 = 0;

	ENEMY_Final_BOSS_Width  = 0;
	ENEMY_Final_BOSS_Height = 0;
	ENEMY_Final_BOSS_Radius = 0;
	ENEMY_Final_BOSS_Speed  = 0;
}

void Enemy_Reset()
{
	for (Enemy& enemy : Enemy_s)
		enemy.isEnable = false;
}

void Enemy_Update(double elapsed_time)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (!Enemy_s[i].isEnable)
			continue;

		if (Enemy_s[i].State == Enemy_State::DESTRUCTION)
			continue;

		switch (Enemy_s[i].Move_Pattern)
		{
		case Enemy_Move_Pattern::NONE:
			break;

		case Enemy_Move_Pattern::Down_And_Up:
			Pattern_Down_And_Up(Enemy_s[i], elapsed_time);
			break;

		case Enemy_Move_Pattern::Down_Patrol:
			Pattern_Patrol(Enemy_s[i], elapsed_time);
			break;

		case Enemy_Move_Pattern::Down_And_Down:
			Pattern_Down_And_Down(Enemy_s[i], elapsed_time);
			break;

		case Enemy_Move_Pattern::Down_And_Chase_Player:
			Pattern_Chaser(Enemy_s[i], elapsed_time);
			break;

		case Enemy_Move_Pattern::Down_Fixed:
			Pattern_Fixed(Enemy_s[i], elapsed_time);
			break;

		case Enemy_Move_Pattern::Down_Shoot_Down:
			Pattern_Down_Shoot_Down(Enemy_s[i], elapsed_time);
			break;

		case Enemy_Move_Pattern::Down_Shoot_Up:
			Pattern_Down_Shoot_Up(Enemy_s[i], elapsed_time);
			break;

		case Enemy_Move_Pattern::Down_Shoot_Patrol:
			Pattern_Down_Shoot_Patrol(Enemy_s[i], elapsed_time);
			break;
		}

		if (Enemy_s[i].Position.y < -Enemy_s[i].Size.y ||
			Enemy_s[i].Position.y > Game_Screen_Height ||
			Enemy_s[i].Position.x + Enemy_s[i].Size.x < Game_Offset.x ||
			Enemy_s[i].Position.x > Game_Offset.x + Game_Screen_Width)
		{
			Enemy_s[i].isEnable = false;
		}
	}
}

void Enemy_Draw()
{
	for (Enemy& enemy : Enemy_s)
	{
		if (!enemy.isEnable)	continue;

		float Angle = 0.0f;

		if (enemy.Move_Pattern == Enemy_Move_Pattern::Down_And_Chase_Player && enemy.State == Enemy_State::Leave)
			Angle = enemy.Angle;

		Sprite_Draw(EnemyType[enemy.Type_ID].TexID,
							  enemy.Position.x, enemy.Position.y,
							  enemy.Size.x, enemy.Size.y, Angle,
			enemy.isDamage ? Half_Magenta : White);

		enemy.isDamage = false;
	}
}

int Enemy_Create(Enemy_Type_ID ID, const DirectX::XMFLOAT2& Position, Enemy_Move_Pattern Pattern, const PATTERN_DATA& data, const XMFLOAT2& targetPos)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (Enemy_s[i].isEnable) continue;

		Enemy& enemy = Enemy_s[i]; 

		enemy.isEnable = true;
		enemy.HP = EnemyType[ID].HP;

		if (ID == Enemy_Type_Special_Boss_Stage_3)
		{
			enemy.Current_Phase = 1;
			enemy.Max_Phase = 3;
		}
		else if (ID == Enemy_Type_Middle_Boss_Stage_4)
		{
			enemy.Current_Phase = 1;
			enemy.Max_Phase = 5;
		}
		else if (ID == Enemy_Type_Final_Phase_1_Boss)
		{
			enemy.Current_Phase = 1;
			enemy.Max_Phase = 4;
		}
		else if (ID == Enemy_Type_Final_Phase_2_Boss)
		{
			enemy.Current_Phase = 1;
			enemy.Max_Phase = 2;
		}
		else
		{
			enemy.Current_Phase = 0;
			enemy.Max_Phase = 0;
		}

		enemy.Type_ID = ID;
		enemy.Position = Position;
		enemy.Angle = 0.0f;

		enemy.Pattern_Data = data;
		enemy.Velocity = EnemyType[ID].Velocity;
		enemy.Speed = EnemyType[ID].Speed;

		enemy.Size = { Enemy_Get_Width(ID, true), Enemy_Get_Width(ID, false) };

		enemy.Move_Pattern = Pattern;

		enemy.State = Enemy_State::Enter;
		enemy.State_Timer = 0.0;
		enemy.Fire_Timer = 0.0;
		enemy.Now_Fire_Count = 0;
		enemy.Now_Patrol_Time = 0.0f;
		enemy.Fix_Pattern_Loop_Count = 0;

		enemy.Get_Score = EnemyType[ID].Give_Score;

		if (targetPos.x != 0.0f || targetPos.y != 0.0f)
			enemy.Target_POS = targetPos;
		else
			enemy.Target_POS = { Position.x, data.Target_Y };

		enemy.Leave_POS = { Position.x, Game_Screen_Height + enemy.Size.x };

		enemy.isDamage = false;
		return i;
	}

	return -1;
}

void Enemy_Create_Destruction_Shell(int Index)
{
	const Enemy& source = Enemy_s[Index];
	if (!source.isEnable) return;

	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (Enemy_s[i].isEnable) continue;

		Enemy& shell = Enemy_s[i];
		shell = source;

		shell.HP = 99999;
		shell.Max_Phase = -1;
		shell.Get_Score = 0;

		break;
	}
}

bool Enemy_Go_To_Next_Phase(int Boss_Index)
{
	if (Boss_Index < 0 || Boss_Index >= ENEMY_MAX)
		return false;

	Enemy& Boss = Enemy_s[Boss_Index];

	if (!Boss.isEnable || Boss.Max_Phase == 0)
		return false;

	if (Boss.Current_Phase < Boss.Max_Phase)
	{
		Boss.Current_Phase++;
		Boss.HP = Get_Enemy_Info(Boss.Type_ID).HP;
		return false;
	}
	else
		return true;
}

int Enemy_Get_Current_Phase(int Boss_Index)
{
	if (Boss_Index >= 0 && Boss_Index < ENEMY_MAX)
		return Enemy_s[Boss_Index].Current_Phase;

	return 0;
}

float Enemy_Get_Width(Enemy_Type_ID ID, bool Is_Width) 
{
	float Width = 0.0f, Height = 0.0;

	switch (ID)
	{
	case Enemy_Type_Origin:
	case Enemy_Type_Origin_ZAKO:
		Width  = ENEMY_Original_Width;
		Height = ENEMY_Original_Height;
		break;

	case Enemy_Type_Saber:
	case Enemy_Type_Saber_ZAKO:
		Width  = ENEMY_Saber_Width;
		Height = ENEMY_Saber_Height;
		break;

	case Enemy_Type_Spear:
	case Enemy_Type_Spear_ZAKO:
	case Enemy_Type_Spear_ZAKO_ITEM:
		Width  = ENEMY_Spear_Width;
		Height = ENEMY_Spear_Height;
		break;

	case Enemy_Type_Armor:
	case Enemy_Type_Armor_Boss:
	case Enemy_Type_Armor_Give_HP:
	case Enemy_Type_Mini_Boss:
		Width  = ENEMY_Armor_Width;
		Height = ENEMY_Armor_Height;
		break;

	case Enemy_Type_Special_Boss_Stage_3:
		Width  = ENEMY_Special_BOSS_Width;
		Height = ENEMY_Special_BOSS_Height;
		break;

	case Enemy_Type_Middle_Boss_Stage_4:
		Width  = ENEMY_Middle_BOSS_Width;
		Height = ENEMY_Middle_BOSS_Height;
		break;

	case Enemy_Type_Final_Phase_1_Boss:
	case Enemy_Type_Final_Phase_2_Boss:
		Width  = ENEMY_Final_BOSS_Width;
		Height = ENEMY_Final_BOSS_Height;
		break;
	}

	return Is_Width ? Width : Height;
}

void Enemy_Give_Score(int Index)
{
	Score_Increase(Enemy_s[Index].Get_Score);
}

bool Enemy_IsEnable(int Index)
{
	return Enemy_s[Index].isEnable;
}

Circle Enemy_GetCollision(int Index)
{
	if (Enemy_s[Index].State == Enemy_State::DESTRUCTION)
		return { { A_Zero, A_Zero }, A_Zero };


	int ID = Enemy_s[Index].Type_ID;

	float Cx = Enemy_s[Index].Position.x + EnemyType[ID].Collision.Center.x;
	float Cy = Enemy_s[Index].Position.y + EnemyType[ID].Collision.Center.y;

	return { { Cx, Cy }, EnemyType[ID].Collision.Radius };
}

const Enemy* Enemy_Get(int Index)
{
	if (Index >= 0 && Index < ENEMY_MAX && Enemy_s[Index].isEnable)
		return &Enemy_s[Index];

	return nullptr;
}

void Enemy_Damage(int Index)
{
	Enemy_s[Index].HP -= Bullet_Get_Now_Damage();
	Enemy_s[Index].isDamage = true;

	if (Enemy_s[Index].HP <= 0.0f)
	{
		if (Enemy_s[Index].Max_Phase > 0)
		{
			bool Is_Final_Phase_Done = Enemy_Go_To_Next_Phase(Index);
			if (Is_Final_Phase_Done)
			{
				Enemy_s[Index].State = Enemy_State::DESTRUCTION;
				Enemy_s[Index].HP = 0;
			}
		}
		else
		{
			switch (Enemy_s[Index].Type_ID)
			{
			case Enemy_Type_Armor_Boss:
				SM->Play_SFX("Enemy_Mini_Boss_Dead");
				break;

			default:
				SM->Play_SFX("Enemy_Dead");
				break;
			}

			Effect_Create(Effect_Type::ENEMY_EXPLOSION, Enemy_s[Index].Position, { Enemy_s[Index].Size.x, Enemy_s[Index].Size.x });

			Enemy_Destroy(Index);
		}
		
	}
}

void Enemy_Bomb_Damage(int Index, float Damage)
{
	Enemy_s[Index].HP -= Damage;
	Enemy_s[Index].isDamage = true;

	if (Enemy_s[Index].HP <= 0.0f)
	{
		if (Enemy_s[Index].Max_Phase > 0)
		{
			bool Is_Final_Phase_Done = Enemy_Go_To_Next_Phase(Index);
			if (Is_Final_Phase_Done)
			{
				Enemy_s[Index].State = Enemy_State::DESTRUCTION;
				Enemy_s[Index].HP = 0;
			}
		}
		else
		{
			Effect_Create(Effect_Type::ENEMY_EXPLOSION, Enemy_s[Index].Position, { Enemy_s[Index].Size.x, Enemy_s[Index].Size.x });
			Enemy_Destroy(Index);
		}
	}
}

void Enemy_Destroy(int Index)
{
	XMFLOAT2 Enemy_Center =
	{
		Enemy_s[Index].Position.x + Enemy_s[Index].Size.x * A_Half,
		Enemy_s[Index].Position.y + Enemy_s[Index].Size.y * A_Half
	};

	XMFLOAT2 Enemy_Size = Enemy_s[Index].Size;

	switch (Enemy_s[Index].Type_ID)
	{
	case Enemy_Type_Origin_ZAKO:
	case Enemy_Type_Saber_ZAKO:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::POWER_UP, A_Quarter);
		break;
	case Enemy_Type_Spear_ZAKO:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::POWER_UP, A_Fifth);
		break;
	case Enemy_Type_Spear_ZAKO_ITEM:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::BOMB);
		break;

	case Enemy_Type_Origin:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::POWER_UP);
		Score_Create(Enemy_Center, Enemy_Size, Item_Type::SCORE);
		break;

	case Enemy_Type_Saber:	
		Score_Create(Enemy_Center, Enemy_Size, Item_Type::SCORE, Enemy_Saber_Bouns_Score, 2);
		break;

	case Enemy_Type_Spear:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::POWER_UP, A_Third);
		break;

	case Enemy_Type_Armor:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::BOMB);
		Score_Create(Enemy_Center, Enemy_Size, Item_Type::SCORE, Enemy_Armor_Bouns_Score);
		break;

	case Enemy_Type_Armor_Boss:
		Score_Create(Enemy_Center, Enemy_Size, Item_Type::SCORE, Enemy_Armor_BOSS_Bonus_Score, 8);

	case Enemy_Type_Armor_Give_HP:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::LIVE);
		Score_Create(Enemy_Center, Enemy_Size, Item_Type::SCORE, Enemy_Armor_Bouns_Score);
		break;

	case Enemy_Type_Mini_Boss:
		Item_Create(Enemy_Center, Enemy_Size, Item_Type::BOMB);
		Score_Create(Enemy_Center, Enemy_Size, Item_Type::SCORE, Enemy_Mini_BOSS_Bonus_Score, 5);
		break;

	case Enemy_Type_Special_Boss_Stage_3:
	case Enemy_Type_Middle_Boss_Stage_4:
	case Enemy_Type_Final_Phase_1_Boss:
	case Enemy_Type_Final_Phase_2_Boss:
	default:
		break;
	}

	Enemy_s[Index].isEnable = false;
}

void Enemy_Destroy_All_Normal_Enemies()
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		if (Enemy_s[i].isEnable)
		{
			XMFLOAT2 Enemy_Center =
			{
				Enemy_s[i].Position.x + Enemy_s[i].Size.x * A_Half,
				Enemy_s[i].Position.y + Enemy_s[i].Size.y * A_Half
			};

			XMFLOAT2 Enemy_Size = Enemy_s[i].Size;

			switch (Enemy_s[i].Type_ID)
			{
			case Enemy_Type_Armor:
			case Enemy_Type_Armor_Boss:
			case Enemy_Type_Armor_Give_HP:
				Enemy_Bomb_Damage(i, BOMB_DAMAGE);
				break;

			case Enemy_Type_Mini_Boss:
				break;

			case Enemy_Type_Special_Boss_Stage_3:
			case Enemy_Type_Middle_Boss_Stage_4:
			case Enemy_Type_Final_Phase_1_Boss:
			case Enemy_Type_Final_Phase_2_Boss:
				Enemy_Bomb_Damage(i, BOMB_DAMAGE);
				break;

			default:
				Item_Create(Enemy_Center, Enemy_Size, Item_Type::POWER_UP, A_Z_T_Quarters);
				Score_Create(Enemy_Center, Enemy_Size, Item_Type::SCORE, Enemy_BOMB_Score);
				Effect_Create(Effect_Type::ENEMY_EXPLOSION, Enemy_s[i].Position, { Enemy_s[i].Size.x, Enemy_s[i].Size.x });
				Enemy_s[i].isEnable = false;
				Enemy_Give_Score(i);
				break;
			}
		}
	}
}

Enemy_Type Get_Enemy_Info(int Type_ID)
{
	return EnemyType[Type_ID];
}
