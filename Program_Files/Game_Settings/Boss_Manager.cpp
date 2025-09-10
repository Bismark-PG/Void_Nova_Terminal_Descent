/*==============================================================================

	Manage Boss Pattern [Boss_Manager.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Boss_Manager.h"
#include "Game_Mode_Manager.h"
#include "Enemy.h"
#include "Stage_Select.h" 
#include "Boss_Pattern.h"
#include "Item.h"

static BOSS_STATE Current_Boss_State;
static NOW_PLAYING_STAGE Current_Stage;

static int Active_Boss_Index = -1;
static int Last_Known_Phase = 1;

void Boss_Initialize()
{
	Current_Boss_State = BOSS_STATE::INACTIVE;
	Current_Stage = NOW_PLAYING_STAGE::STAGE_NONE;
	Active_Boss_Index = -1;
	Last_Known_Phase = 1;
}

void Boss_Finalize()
{
}

void Boss_Activate(NOW_PLAYING_STAGE stage)
{
	if (Current_Boss_State != BOSS_STATE::INACTIVE) return;

	Current_Stage = stage;
	GameMode now_mode = Get_Game_Mode();

	if (now_mode == GameMode::NEW_GAME || now_mode == GameMode::CONTINUE)
		Current_Boss_State = BOSS_STATE::STORY;
	else
		Current_Boss_State = BOSS_STATE::BATTLE;

	Last_Known_Phase = 1;
}

void Boss_Update(double elapsed_time)
{
	if (Current_Boss_State == BOSS_STATE::INACTIVE || Current_Boss_State == BOSS_STATE::DEFEATED)
		return;

	if (Active_Boss_Index != -1 && Enemy_IsEnable(Active_Boss_Index))
	{
		Enemy& BOSS = *const_cast<Enemy*>(Enemy_Get(Active_Boss_Index));

		if (Current_Boss_State == BOSS_STATE::STORY)
			Boss_Pattern_Enter(BOSS, elapsed_time);
		else if (Current_Boss_State == BOSS_STATE::BATTLE)
		{
			switch (Current_Stage)
			{
			case NOW_PLAYING_STAGE::STAGE_THREE:
				Boss_Pattern_Stage_3_Update(BOSS, elapsed_time);
				break;

			case NOW_PLAYING_STAGE::STAGE_FOUR:
				Boss_Pattern_Stage_4_Update(BOSS, elapsed_time);
				break;

			case NOW_PLAYING_STAGE::STAGE_FIVE:
				Boss_Pattern_Stage_5_Update(BOSS, elapsed_time);
				break;
			}

			if (BOSS.Current_Phase > Last_Known_Phase)
			{
				XMFLOAT2 Boss_Center =
				{
					BOSS.Position.x + BOSS.Size.x * A_Half,
					BOSS.Position.y + BOSS.Size.y * A_Half
				};

				SM->Play_SFX("Enemy_Mini_Boss_Dead");

				switch (Current_Stage)
				{
				case NOW_PLAYING_STAGE::STAGE_THREE:
					Item_Create(Boss_Center, BOSS.Size, Item_Type::POWER_UP, A_Quarter, 5);
					Item_Create(Boss_Center, BOSS.Size, Item_Type::LIVE);
					Item_Create(Boss_Center, BOSS.Size, Item_Type::BOMB);
					Score_Create(Boss_Center, BOSS.Size, Item_Type::SCORE, Enemy_Type_Special_Boss_Phase_Change_Score, 15);
					break;

				case NOW_PLAYING_STAGE::STAGE_FOUR:
					Item_Create(Boss_Center, BOSS.Size, Item_Type::POWER_UP, A_Quarter, 6);
					Item_Create(Boss_Center, BOSS.Size, Item_Type::LIVE);
					Item_Create(Boss_Center, BOSS.Size, Item_Type::BOMB, 0, 2);
					Score_Create(Boss_Center, BOSS.Size, Item_Type::SCORE, Enemy_Type_Middle_Boss_Phase_Change_Score, 15);
					break;
				}

				Last_Known_Phase = BOSS.Current_Phase;

				Boss_Pattern_Initialize(BOSS);
			}

			if (BOSS.State == Enemy_State::DESTRUCTION)
			{
				Current_Boss_State = BOSS_STATE::DEFEATED;
				SM->Play_SFX("Enemy_Boss_Dead");
				Effect_Create(Effect_Type::REAL_EXPLOSION, BOSS.Position, { BOSS.Size.x, BOSS.Size.x });
				Enemy_Destroy(Active_Boss_Index);
				Active_Boss_Index = -1;
			}
		}
	}
}

void Boss_Draw()
{
	// effect ??
}

BOSS_STATE Get_Boss_State()
{
	return Current_Boss_State;
}

void Set_Boss_State(BOSS_STATE state)
{
	Current_Boss_State = state;
}

bool Is_Boss_Defeated()
{
	return Current_Boss_State == BOSS_STATE::DEFEATED;
}

int Enemy_Get_Active_Boss_Index()
{
	if (Current_Boss_State == BOSS_STATE::BATTLE && Active_Boss_Index != -1)
		return Active_Boss_Index;

	if (Active_Boss_Index != -1)
		return Active_Boss_Index;

	return -1;
}
void Set_Active_Boss_Index(int index)
{
	Active_Boss_Index = index;
}
