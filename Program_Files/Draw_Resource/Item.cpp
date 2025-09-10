/*==============================================================================

	Manage Item [Item.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Item.h"
#include "Game_Window.h"
#include "Texture.h"
#include "Sprite.h"
#include "Player.h"      
#include "Game_Status.h" 
#include "Palette.h"      
#include "Debug_Collision.h"
#include "Score.h"
#include "Audio_Manager.h"
#include <random>
#include "system_timer.h" 

using namespace DirectX;
using namespace PALETTE;

static Item Items[ITEM_MAX]{};

static int Power_Up_TexID = -1;
static int Score_Up_TexID = -1;
static int Bomb_Up_TexID  = -1;
static int Life_Up_TexID  = -1;

static int Anime_Power = -1;
static int Anime_Score = -1;
static int Anime_Bomb  = -1;
static int Anime_Life  = -1;

static int Play_Power = -1;
static int Play_Score = -1;
static int Play_Bomb = -1;
static int Play_Life = -1;

static std::random_device RD;
static std::mt19937 Get(RD());

void Item_Initialize()
{
	Power_Up_TexID = Texture_Load(L"Resource/Texture/UI/UI_Item P_Animeition.png");
	Score_Up_TexID = Texture_Load(L"Resource/Texture/UI/UI_Item S_Animeition.png");
	Bomb_Up_TexID  = Texture_Load(L"Resource/Texture/UI/UI_Item B_Animeition.png");
	Life_Up_TexID  = Texture_Load(L"Resource/Texture/UI/UI_Item L_Animeition.png");

	Anime_Power = SpriteAni_Get_Pattern_Info(Power_Up_TexID, 2, 2, 1.0, { 454, 454 }, { 454 * 0, 454 * 0 }, true);
	Anime_Score = SpriteAni_Get_Pattern_Info(Score_Up_TexID, 2, 2, 0.75, { 454, 454 }, { 454 * 0, 454 * 0 }, true);
	Anime_Bomb  = SpriteAni_Get_Pattern_Info(Bomb_Up_TexID,  2, 2, 1.25, { 454, 454 }, { 454 * 0, 454 * 0 }, true);
	Anime_Life  = SpriteAni_Get_Pattern_Info(Life_Up_TexID,  2, 2, 1.25, { 454, 454 }, { 454 * 0, 454 * 0 }, true);

	Play_Power = SpriteAni_CreatePlayer(Anime_Power);
	Play_Score = SpriteAni_CreatePlayer(Anime_Score);
	Play_Bomb  = SpriteAni_CreatePlayer(Anime_Bomb);
	Play_Life  = SpriteAni_CreatePlayer(Anime_Life);

	for (int i = 0; i < ITEM_MAX; i++)
		Items[i].isEnable = false;

	Item_Size   = ITEM_SIZE * Game_Scale;
	Item_Speed  = ITEM_SPEED * Game_Scale;
	Item_Radius = (Item_Size + Item_Size) * A_Quarter;

	Score_HP_Size   = Item_Size * A_T_Quarters;
	Score_HP_Speed  = SCORE_SPEED * Game_Scale;
	Score_HP_Radius = (Score_HP_Size + Score_HP_Size) * A_Quarter;
}

void Item_Finalize()
{
	SpriteAni_DestroyPlayer(Play_Power);
	SpriteAni_DestroyPlayer(Play_Score);
	SpriteAni_DestroyPlayer(Play_Bomb);
	SpriteAni_DestroyPlayer(Play_Life);
}

void Item_Update(double elapsed_time)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!Items[i].isEnable) continue;

		if (Items[i].State == Item_State::NONE)
		{
			Items[i].Life_Time -= static_cast<float>(elapsed_time);
			if (Items[i].Life_Time <= A_Zero)
			{
				Items[i].isEnable = false;
				continue;
			}
		}
		else
		{
			Items[i].Position.y += Items[i].Velocity.y * static_cast<float>(elapsed_time);

			if (Items[i].Position.y > Game_Screen_Height)
				Items[i].isEnable = false;
		}

		if (Player_Is_Alive() && Collision_IsOverlapCircle(Item_GetCollision(i), Player_Get_Item_Collision()))
		{
			switch (Items[i].Type)
			{
			case Item_Type::POWER_UP:
				Status_Add_Power(Items[i].Get_Power);
				SM->Play_SFX("Player_Get_Power");
				break;
			case Item_Type::SCORE:
				Score_Increase(Items[i].Get_Score);
				SM->Play_SFX("Player_Get_Score");
				break;
			case Item_Type::BOMB:
				Status_Add_Bomb(STATUS_UP);
				SM->Play_SFX("Player_Ger_Bomb");
				break;
			case Item_Type::LIVE:
				Status_Add_Lives(STATUS_UP);
				SM->Play_SFX("Player_Get_Lives");
				break;
			}
			Items[i].isEnable = false;
			continue;
		}
	}
}

void Item_Draw()
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (!Items[i].isEnable) continue;

		int ID = -1;
		switch (Items[i].Type)
		{
		case Item_Type::POWER_UP:
			ID = Play_Power;
			break;

		case Item_Type::SCORE:
			ID = Play_Score;
			break;

		case Item_Type::BOMB:
			ID = Play_Bomb;
			break;

		case Item_Type::LIVE:
			ID = Play_Life;
			break;
		}

		if (ID != -1)
		{
			float Alpha = A_Origin;
			if (Items[i].State == Item_State::NONE && Items[i].Life_Time < A_Triple)
			{
				float Frequency = A_Quintuple + (15.0f * (A_Triple - Items[i].Life_Time) * A_Third);
				Alpha = A_Half + A_Half * sinf(static_cast<float>(SystemTimer_GetTime()) * Frequency);
			}

			SpriteAni_Draw(ID, Items[i].Position.x, Items[i].Position.y, Items[i].Size.x, Items[i].Size.y, A_Zero, { 1.f, 1.f, 1.f, Alpha });
		}
	}
}

void Item_Create(const DirectX::XMFLOAT2& Enemy_Center_Pos, const DirectX::XMFLOAT2& Enemy_Size, Item_Type Type, float Power_Value, int Count)
{
	for (int Spawn_Count = 0; Spawn_Count < Count; Spawn_Count++)
	{
		for (int i = 0; i < ITEM_MAX; i++)
		{
			if (Items[i].isEnable) continue;

			Items[i].isEnable = true;
			Items[i].Type = Type;
			Items[i].State = Item_State::FALLING;
			Items[i].Get_Power = Power_Value;

			Items[i].Size = { Item_Size, Item_Size };

			std::uniform_real_distribution<float> Ran_X(-Enemy_Size.x * A_Half, Enemy_Size.x * A_Half);
			std::uniform_real_distribution<float> Ran_Y(-Enemy_Size.y * A_Half, Enemy_Size.y * A_Half);

			float Pos_X = Enemy_Center_Pos.x + Ran_X(Get) - Item_Size * A_Half;
			float Pos_Y = Enemy_Center_Pos.y + Ran_Y(Get) - Item_Size * A_Half;

			Items[i].Position = { Pos_X, Pos_Y };
			Items[i].Velocity = { A_Zero, Item_Speed };
			Items[i].Collision = { { Item_Size * A_Half, Item_Size * A_Half }, Item_Radius };
			break;
		}
	}
}

void Score_Create(const DirectX::XMFLOAT2& Enemy_Center_Pos, const DirectX::XMFLOAT2& Enemy_Size, Item_Type Type, int Score_Value, int Count)
{
	for (int Spawn_Count = 0; Spawn_Count < Count; Spawn_Count++)
	{
		for (int i = 0; i < ITEM_MAX; i++)
		{
			if (Items[i].isEnable) continue;

			Items[i].isEnable = true;
			Items[i].Type = Type;
			Items[i].State = Item_State::FALLING;
			Items[i].Get_Score = Score_Value;

			Items[i].Size = { Score_HP_Size, Score_HP_Size };

			std::uniform_real_distribution<float> Ran_X(-Enemy_Size.x * A_Half, Enemy_Size.x * A_Half);
			std::uniform_real_distribution<float> Ran_Y(-Enemy_Size.y * A_Half, Enemy_Size.y * A_Half);

			float Pos_X = Enemy_Center_Pos.x + Ran_X(Get) - Score_HP_Size * A_Half;
			float Pos_Y = Enemy_Center_Pos.y + Ran_Y(Get) - Score_HP_Size * A_Half;

			Items[i].Position = { Pos_X, Pos_Y };
			Items[i].Velocity = { A_Zero, Score_HP_Speed };
			Items[i].Collision = { { Score_HP_Size * A_Half, Score_HP_Size * A_Half }, Score_HP_Radius };
			break;
		}
	}
}

void Power_Create_On_Player_Death(const DirectX::XMFLOAT2& Player_Pos)
{
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (Items[i].isEnable) continue;

		Items[i].isEnable = true;
		Items[i].Type = Item_Type::POWER_UP;
		Items[i].State = Item_State::NONE;
		Items[i].Get_Power = PLAYER_DROP_POWER_VALUE;
		Items[i].Life_Time = static_cast<float>(PLAYER_DROP_LIFE_TIME);

		Items[i].Size = { Score_HP_Size, Score_HP_Size };

		std::uniform_real_distribution<float> Ran_POS(-Player_Height * A_Half, Player_Height * A_Half);

		float Player_Center_X = Player_Pos.x + Player_Width * A_Half;
		float Player_Center_Y = Player_Pos.y + Player_Height * A_Half;

		float Pos_X = Player_Center_X + Ran_POS(Get) - Items[i].Size.x * A_Half;
		float Pos_Y = Player_Center_Y + Ran_POS(Get) - Items[i].Size.y * A_Half;

		Items[i].Position = { Pos_X, Pos_Y };
		Items[i].Velocity = { A_Zero, A_Zero };
		Items[i].Collision = { { Score_HP_Size * A_Half, Score_HP_Size * A_Half }, Score_HP_Radius };

		break;
	}
}
bool Item_IsEnable(int Index)
{
	return Items[Index].isEnable;
}

Circle Item_GetCollision(int Index)
{
	float Cx = Items[Index].Collision.Center.x
		+ Items[Index].Position.x;
	float Cy = Items[Index].Collision.Center.y
		+ Items[Index].Position.y;

	return { { Cx, Cy }, Items[Index].Collision.Radius };
}