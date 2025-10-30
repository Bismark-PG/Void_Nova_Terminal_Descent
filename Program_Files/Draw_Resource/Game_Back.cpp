/*==============================================================================

	Draw Game Background [Game_Back.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Game_Back.h"
#include "Stage_Select.h"
#include "Game_Status.h"
#include "Score.h"
#include "Stage_Game_Over.h" 
#include "Enemy.h"
#include "Game_Window.h"
#include <algorithm> 
#include "Boss_Manager.h" 

using namespace PALETTE;

//----------------Stage Background----------------//
static int Stage_BG_1_TexID = -1;
static int Stage_BG_2_TexID = -1;
static int Stage_BG_3_TexID = -1;
static int Stage_BG_4_TexID = -1;
static int Stage_BG_5_TexID = -1;
static int Stage_5_BlackOut_TexID = -1;
static int Anime_Final_BG = -1;
static int Play_Final_BG = -1;

//-----------------------UI------------------------//
static int UI_BG_L = -1;
static int UI_BG_R = -1;
static int UI_Title = -1;

static int UI_FS_L = -1;
static int UI_FS_R = -1;

static int UI_Life = -1;
static int UI_Bomb = -1;
static int UI_Score = -1;
static int UI_Power = -1;

static int UI_P_Bar_Min = -1;
static int UI_P_Bar_Max = -1;
static int UI_P_Bar_Fill = -1;

static int UI_Bomb_Effect_Man = -1;
static int UI_Bomb_Effect_Ring = -1;

static int UI_Life_Texture = -1;
static int UI_Bomb_Texture = -1;

static int UI_BOSS_HP = -1;
static int UI_BOSS_HP_Fill = -1;
static int UI_BOSS_HP_Frame = -1;

static int UI_Number_Texture = -1;

//------------------Stage Start UI------------------//
static int Deployment_TexID = -1;
static int Anime_Deployment_TexID = -1;
static int Play_Deployment_TexID  = -1;

static int Initiated_TexID = -1;
static int Anime_Initiated_TexID = -1;
static int Play_Initiated_TexID  = -1;

static int Mission_TexID = -1;
static int Anime_Mission_TexID = -1;
static int Play_Mission_TexID = -1;

static int Final_Ready_1 = -1;
static int Anime_Final_Ready_1 = -1;
static int Play_Final_Ready_1 = -1;

static int Final_Ready_2 = -1;
static int Anime_Final_Ready_2 = -1;
static int Play_Final_Ready_2 = -1;

static float Start_BG_Alpha = 1.0f;
static float Final_Start_BG_Alpha = 1.0f;

static bool Is_Anime_Draw_Done = false;
static bool Is_Ready_Anime_Finished = false;
static bool Is_Final_Ready_Anime_Finished = false;
static double Ready_State_Timer = 0.0;

static bool Is_Bomb_Effect_Playing = false;
static bool Increase_Done = true;
static BOMB_EFFECT_STATE Bomb_Effect_State = BOMB_EFFECT_STATE::IDLE;
static FINAL_BOSS_ANIME	Anime_Flow = FINAL_BOSS_ANIME::NONE;

static BOSS_UI_STATE Boss_UI_State = BOSS_UI_STATE::HIDDEN;
static int Active_Boss_Index = -1;
static float Boss_UI_Animation_Timer = 0.0f;
static float Boss_UI_Current_Alpha = 0.0f;

static float Boss_UI_X = 0.0f;
static float Boss_UI_Y = 0.0f;
static float Boss_UI_Width = 0.0f;
static float Boss_UI_Height = 0.0f;

static constexpr float BOSS_UI_FADE_IN_DURATION = 1.0f;
static constexpr float BOSS_UI_FADE_OUT_DURATION = 1.5f;

static ScreenMode Now_Mode;

void Game_Back_Initialize()
{
	Game_BG_UI_Texture();

	Stage_BG_X = Game_Offset.x;
	Stage_BG_Y = Game_Offset.y;
	Stage_Space_BG_Y  = Stage_BG_Y - static_cast<float>(Game_Screen_Height);
	Stage_BG_Width  = static_cast<float>(Game_Screen_Width);
	Stage_BG_Height = static_cast<float>(Game_Screen_Height) * Stage_BG_Height_Scale;

	Game_UI_Initialize();
	Game_Boss_UI_Initialize();

	Stage_Ready_Initialize();
	Game_Over_Initialize();
}

void Game_Back_Finalize()
{
	Game_Over_Finalize();
	Stage_Ready_Finalize();

	Game_Boss_UI_Finalize();
	Game_UI_Finalize();

	Stage_BG_X = 0;
	Stage_BG_Y = 0;
	Stage_Space_BG_Y  = 0;
	Stage_Second_BG_Y = 0;
	Stage_BG_Width  = 0;
	Stage_BG_Height = 0;
}


void Game_Boss_UI_Initialize()
{
	Boss_UI_Width = static_cast<float>(Game_Screen_Width);
	Boss_UI_Height = static_cast<float>(Game_Screen_Height) * A_Zero_Two;
	Boss_UI_X = Game_Offset.x;
	Boss_UI_Y = Game_Offset.y;

	Active_Boss_Index = -1;
	Boss_UI_State = BOSS_UI_STATE::HIDDEN;
	Boss_UI_Animation_Timer = 0.0f;
	Boss_UI_Current_Alpha = 0.0f;
}

void Game_Boss_UI_Finalize()
{
}

void Game_Boss_UI_Update(double elapsed_time)
{
	int BOSS_Index = Enemy_Get_Active_Boss_Index();
	int MINI_Boss_Index = -1;

	for (int i = 0; i < ENEMY_MAX; ++i)
	{
		if (Enemy_IsEnable(i))
		{
			const Enemy* enemy = Enemy_Get(i);
			if (enemy->Type_ID == Enemy_Type_Armor_Boss || enemy->Type_ID == Enemy_Type_Mini_Boss)
			{
				MINI_Boss_Index = i;
				break;
			}
		}
	}

	int target_boss_index = -1;

	if (MINI_Boss_Index != -1)
		target_boss_index = MINI_Boss_Index;
	else if (BOSS_Index != -1)
		target_boss_index = BOSS_Index;

	if (target_boss_index != -1)
	{
		if (Active_Boss_Index != target_boss_index)
			Game_Boss_UI_Start_Appearance(target_boss_index);
	}
	else if (Active_Boss_Index != -1)
	{
		if (!Enemy_IsEnable(Active_Boss_Index))
			Game_Boss_UI_Start_Disappearance();
	}

	float Time = static_cast<float>(elapsed_time);

	if (Boss_UI_State == BOSS_UI_STATE::ENTERING)
	{
		Boss_UI_Animation_Timer += Time;
		Boss_UI_Current_Alpha = std::min(Boss_UI_Animation_Timer / BOSS_UI_FADE_IN_DURATION, 1.0f);

		if (Boss_UI_Animation_Timer >= BOSS_UI_FADE_IN_DURATION)
		{
			Boss_UI_State = BOSS_UI_STATE::VISIBLE;
			Boss_UI_Current_Alpha = 1.0f;
		}
	}
	else if (Boss_UI_State == BOSS_UI_STATE::EXITING)
	{
		Boss_UI_Animation_Timer += Time;
		Boss_UI_Current_Alpha = 1.0f - std::min(Boss_UI_Animation_Timer / BOSS_UI_FADE_OUT_DURATION, 1.0f);

		if (Boss_UI_Animation_Timer >= BOSS_UI_FADE_OUT_DURATION)
		{
			Boss_UI_State = BOSS_UI_STATE::HIDDEN;
			Boss_UI_Current_Alpha = 0.0f;
			Active_Boss_Index = -1;
		}
	}
}

void Game_Boss_UI_Draw()
{
	if (Boss_UI_State == BOSS_UI_STATE::HIDDEN || Active_Boss_Index == -1)
		return;

	const Enemy* BOSS = Enemy_Get(Active_Boss_Index);

	if (!BOSS)
		return;

	const Enemy_Type& Boss_Type = Get_Enemy_Info(BOSS->Type_ID);
	float MAX_HP = Boss_Type.HP;
	float HP = BOSS->HP;
	float Ratio = (HP > 0.0f) ? (HP / MAX_HP) : 0.0f;

	Sprite_Draw(UI_BOSS_HP, Boss_UI_X, Boss_UI_Y, Boss_UI_Width, Boss_UI_Height, A_Zero, {A_Origin, A_Origin, A_Origin, Boss_UI_Current_Alpha * A_T_Quarters});

	float Fill_Width = Boss_UI_Width * Ratio;

	float T_W = static_cast<float>(Texture_M->Get_Width(UI_BOSS_HP_Frame));
	float UV_W = T_W * Ratio;

	if (Fill_Width > 0)
	{
		Sprite_UV_Draw(UI_BOSS_HP_Fill, Boss_UI_X, Boss_UI_Y, Fill_Width, Boss_UI_Height,
					A_Zero, A_Zero, UV_W, static_cast<float>(Texture_M->Get_Height(UI_BOSS_HP_Fill)),
					A_Zero, { A_Origin, A_Origin, A_Origin, Boss_UI_Current_Alpha });
	}

	Sprite_Draw(UI_BOSS_HP_Frame, Boss_UI_X, Boss_UI_Y, Boss_UI_Width, Boss_UI_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Boss_UI_Current_Alpha });
}

void Game_Boss_UI_Start_Appearance(int bossIndex)
{
	if (Boss_UI_State == BOSS_UI_STATE::HIDDEN || Boss_UI_State == BOSS_UI_STATE::EXITING || Active_Boss_Index != bossIndex)
	{
		Active_Boss_Index = bossIndex;
		if (Active_Boss_Index != -1)
		{
			Boss_UI_State = BOSS_UI_STATE::ENTERING;
			Boss_UI_Animation_Timer = 0.0f;
		}
	}
}

void Game_Boss_UI_Start_Disappearance()
{
	if (Boss_UI_State == BOSS_UI_STATE::VISIBLE || Boss_UI_State == BOSS_UI_STATE::ENTERING)
	{
		Boss_UI_State = BOSS_UI_STATE::EXITING;
		Boss_UI_Animation_Timer = 0.0f;
	}
}

void Game_UI_Initialize()
{
	UI_Title_Height = UI_Area.Height * UI_TITLE_HEIGHT;
	UI_Title_Y = UI_Area.Height - UI_Title_Height;

	UI_Life_Bomb_Width = UI_Area.Width * UI_LIFE_BOMB_WIDTH;
	UI_Score_Width = UI_Area.Width * UI_SCORE_WIDTH;
	UI_In_Game_Height = UI_Area.Height * UI_IN_GAME_HEIGHT;

	UI_Life_Bomb_X  = UI_Area.Lx + (UI_Area.Width * UI_IN_GAME_X);
	UI_Life_Score_Y = UI_Area.Width * UI_LIFE_Y;
	UI_Bomb_Y = UI_Life_Score_Y + (UI_In_Game_Height * UI_BOMB_Y);

	UI_Life_Bomb_Texture_X = UI_Life_Bomb_X + (UI_Life_Bomb_X * A_Fifth);
	UI_Life_Bomb_Num_Width = UI_Life_Bomb_Width * A_Fifth;

	UI_Life_Bomb_Num_X = UI_Area.Lx + (UI_Area.Width * A_Half) - (UI_Life_Bomb_Num_Width * A_Half);
	UI_Life_Num_Y = UI_Life_Score_Y + ((UI_Bomb_Y - (UI_Life_Score_Y + UI_In_Game_Height)) * A_Half) + (UI_In_Game_Height * A_Half);
	UI_Bomb_Num_Y = UI_Bomb_Y + ((UI_Bomb_Y - (UI_Life_Score_Y + UI_In_Game_Height)) * A_Half) + (UI_In_Game_Height * A_Half);

	UI_Score_X = UI_Area.Rx + (UI_Area.Width * A_Half) - (UI_Score_Width * A_Half);

	UI_Power_Width  = UI_Score_Width * A_Half;
	UI_Power_Height = UI_In_Game_Height * A_Half;
	UI_Power_X = static_cast<float>(UI_Area.Lx + UI_Area.Width);
	UI_Power_Y = static_cast<float>(Game_Screen_Height) - UI_Power_Height;

	UI_Power_Bar_Width  = UI_Power_Width * A_F_Fifths;
	UI_Power_Bar_Height = UI_Power_Height * A_Half;
	UI_Power_Bar_1_x = UI_Power_X + UI_Power_Width + (UI_Power_Bar_Width * A_One_Tenth);
	UI_Power_Bar_2_x = UI_Power_Bar_1_x + (UI_Power_Bar_Width * UI_Power_Bar_Width_Scale);
	UI_Power_Bar_3_x = UI_Power_Bar_2_x + (UI_Power_Bar_Width * UI_Power_Bar_Width_Scale);
	UI_Power_Bar_4_x = UI_Power_Bar_3_x + (UI_Power_Bar_Width * UI_Power_Bar_Width_Scale);
	UI_Power_Bar_Y   = static_cast<float>(Game_Screen_Height) - UI_Power_Bar_Height;

	Score_Num_Height = UI_In_Game_Height * A_Half;
	Score_Num_Width = (UI_In_Game_Height * A_T_Quarters) * A_Half;
	float Total_Score_Width = Score_Num_Width * SCORE_DIGITS;

	Score_Num_X = UI_Area.Rx + (UI_Area.Width * A_Half) - (Total_Score_Width * A_Half);
	Score_Num_Y = UI_Life_Score_Y + UI_In_Game_Height + Score_Num_Height;

	Bomb_Man = static_cast<float>(UI_Area.Rx);
	Bomb_Man_Alpha = A_Origin;
	Bomb_Scale = A_Zero;

	Now_Mode = Window_M.Get_Now_Screen_Mode();
	UI_Full_L_W = static_cast<float>(UI_Area.Lx);
	UI_Full_L_X = A_Zero;
	UI_Full_R_W = static_cast<float>(SCREEN_WIDTH - UI_Area.Rx);
	UI_Full_R_X = Game_Offset.x + Game_Screen_Width + UI_Area.Width;
}

void Game_UI_Finalize()
{
	UI_TITLE_Y = 0.8f;
	UI_TITLE_HEIGHT = 0.2f;
	UI_IN_GAME_X = 0.1f;
	UI_IN_GAME_HEIGHT = 0.05f;

	UI_LIFE_Y = 0.1f;
	UI_BOMB_Y = 3.0f;
	UI_LIFE_BOMB_WIDTH = 0.5f;
	UI_SCORE_WIDTH = 0.75f;

	UI_Title_Y = 0;
	UI_Title_Height = 0;

	UI_Life_Bomb_X = 0;
	UI_Score_X = 0;
	UI_Life_Score_Y = 0;
	UI_Bomb_Y = 0;

	UI_Life_Bomb_Width = 0;
	UI_Score_Width = 0;
	UI_In_Game_Height = 0;

	UI_Life_Bomb_Num_X = 0;
	UI_Life_Num_Y = 0;
	UI_Bomb_Num_Y = 0;
	UI_Life_Bomb_Texture_Size = 0;
	UI_Life_Bomb_Num_Width = 0;

	UI_Power_X = 0;
	UI_Power_Y = 0;
	UI_Power_Width = 0;
	UI_Power_Height = 0;

	UI_Power_Bar_1_x = 0;
	UI_Power_Bar_2_x = 0;
	UI_Power_Bar_3_x = 0;
	UI_Power_Bar_4_x = 0;
	UI_Power_Bar_Y = 0;
	UI_Power_Bar_Width = 0;
	UI_Power_Bar_Height = 0;

	Score_Num_X = 0;
	Score_Num_Y = 0;
	Score_Num_Width = 0;
	Score_Num_Height = 0;

	UI_Full_L_W = 0;
	UI_Full_L_X = 0;
	UI_Full_R_W = 0;
	UI_Full_R_X = 0;
}

void Game_UI_And_Logo_Draw()
{
	if (!Is_Final_Boss_Active())
	{
		if (Is_Bomb_Effect_Playing)
			Game_UI_Draw_Bomb_Effect();

		Sprite_Draw(UI_BG_L, static_cast<float>(UI_Area.Lx), static_cast<float>(UI_Area.y), static_cast<float>(UI_Area.Width), static_cast<float>(UI_Area.Height));
		Sprite_Draw(UI_BG_R, static_cast<float>(UI_Area.Rx), static_cast<float>(UI_Area.y), static_cast<float>(UI_Area.Width), static_cast<float>(UI_Area.Height));
		Sprite_Draw(UI_Title, static_cast<float>(UI_Area.Rx), UI_Title_Y, static_cast<float>(UI_Area.Width), UI_Title_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_Half });
	}

	switch (Now_Mode)
	{
	case FullScreen_Mode:
	case Borderless_Mode:
		Sprite_Draw(UI_FS_L, UI_Full_L_X, static_cast<float>(UI_Area.y), UI_Full_L_W, static_cast<float>(UI_Area.Height));
		Sprite_Draw(UI_FS_R, UI_Full_R_X, static_cast<float>(UI_Area.y), UI_Full_R_W, static_cast<float>(UI_Area.Height));
		break;
	}

	Sprite_Draw(UI_Life, UI_Life_Bomb_X, UI_Life_Score_Y, UI_Life_Bomb_Width, UI_In_Game_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_T_Quarters });
	Sprite_Draw(UI_Bomb, UI_Life_Bomb_X, UI_Bomb_Y, UI_Life_Bomb_Width, UI_In_Game_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_T_Quarters });
	Sprite_Draw(UI_Score, UI_Score_X, UI_Life_Score_Y, UI_Score_Width, UI_In_Game_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_T_Quarters });
	Sprite_Draw(UI_Power, UI_Power_X, UI_Power_Y, UI_Power_Width, UI_Power_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_T_Quarters });

	Game_UI_Draw_Status_Numbers();

	Score_Draw();

	float Current_Power = Status_Get_Power();
	int Full_Bars = static_cast<int>(Current_Power);
	float Partial_fill = Current_Power - Full_Bars;

	float Bar_POS_X[] = { UI_Power_Bar_1_x, UI_Power_Bar_2_x, UI_Power_Bar_3_x, UI_Power_Bar_4_x };

	for (int i = 0; i < 4; ++i)
	{
		Sprite_Draw(UI_P_Bar_Min, Bar_POS_X[i], UI_Power_Bar_Y, UI_Power_Bar_Width, UI_Power_Bar_Height, A_Zero, {A_Origin, A_Origin, A_Origin, A_Half});

		if (i < Full_Bars)
			Sprite_Draw(UI_P_Bar_Max, Bar_POS_X[i], UI_Power_Bar_Y, UI_Power_Bar_Width, UI_Power_Bar_Height);

		else if (i == Full_Bars && Partial_fill > 0.0f && Full_Bars < 4)
		{
			float Filled_Width = UI_Power_Bar_Width * Partial_fill;

			float Texture_width  = static_cast<float>(Texture_M->Get_Width(UI_P_Bar_Max));
			float Texture_height = static_cast<float>(Texture_M->Get_Height(UI_P_Bar_Max));

			float Partial_Texture_Width = Texture_width * Partial_fill;

			Sprite_UV_Draw(UI_P_Bar_Fill, Bar_POS_X[i], UI_Power_Bar_Y, Filled_Width, UI_Power_Bar_Height,
							A_Zero, A_Zero, Partial_Texture_Width, Texture_height);
		}
	}
}

void Game_UI_Draw_Status_Numbers()
{
	int Lives = Status_Get_Lives();
	int Bombs = Status_Get_Bombs();

	Sprite_Draw(UI_Life_Texture, UI_Life_Bomb_X, UI_Life_Num_Y, UI_In_Game_Height, UI_In_Game_Height, A_Zero, {A_Origin, A_Origin, A_Origin, A_T_Quarters});
	Draw_Number(UI_Life_Bomb_Num_X, UI_Life_Num_Y, UI_Life_Bomb_Num_Width, UI_In_Game_Height, Lives, false);

	Sprite_Draw(UI_Bomb_Texture, UI_Life_Bomb_X, UI_Bomb_Num_Y, UI_In_Game_Height, UI_In_Game_Height, A_Zero, { A_Origin, A_Origin, A_Origin, A_T_Quarters });
	Draw_Number(UI_Life_Bomb_Num_X, UI_Bomb_Num_Y, UI_Life_Bomb_Num_Width, UI_In_Game_Height, Bombs, false);
}

void Game_UI_Update_Bomb_Effect(double elapsed_time)
{
	if (!Is_Bomb_Effect_Playing) return;

	float Time = static_cast<float>(elapsed_time);

	switch (Bomb_Effect_State)
	{
	case BOMB_EFFECT_STATE::IDLE:
		break;

	case BOMB_EFFECT_STATE::ENTERING:
		Bomb_Man -= (Game_Screen_Width * A_Double) * Time;

		if (Bomb_Man <= Game_Offset.x)
		{
			Bomb_Man = Game_Offset.x;
			Bomb_Effect_State = BOMB_EFFECT_STATE::FADING_OUT;
		}
		break;

	case BOMB_EFFECT_STATE::FADING_OUT:
		Bomb_Man_Alpha -= (A_Origin / A_One_And_Half) * Time;

		if (Bomb_Man_Alpha <= A_Zero)
		{
			Bomb_Man_Alpha = A_Zero;
			Bomb_Effect_State = BOMB_EFFECT_STATE::IDLE;
			Is_Bomb_Effect_Playing = false;
		}
		break;
	}
}

void Game_UI_Draw_Bomb_Effect()
{
	if (Bomb_Effect_State == BOMB_EFFECT_STATE::IDLE)
		return;

	Sprite_Draw(UI_Bomb_Effect_Man, Bomb_Man, Game_Offset.y, static_cast<float>(Game_Screen_Width), static_cast<float>(Game_Screen_Height),
				A_Zero, { A_Origin, A_Origin, A_Origin, Bomb_Man_Alpha });
}

void Game_Draw_Bomb_Effect(float P_X, float P_Y)
{
	if (Increase_Done)
		return;

	float Draw_X = P_X - (Bomb_Scale * A_Half);
	float Draw_Y = P_Y - (Bomb_Scale * A_Half);

	Sprite_Draw(UI_Bomb_Effect_Ring, Draw_X, Draw_Y, Bomb_Scale, Bomb_Scale, A_Zero,{ A_Origin, A_Origin, A_Origin, A_T_Quarters });
	Bomb_Scale += Bomb_Scale_Increase;

	if (Bomb_Scale >= Game_Screen_Height * 3.0f)
		Increase_Done = true;
}

void Game_UI_Start_Bomb_Effect()
{
	if (Is_Bomb_Effect_Playing) return;

	Bomb_Man = static_cast<float>(UI_Area.Rx); 
	Bomb_Man_Alpha = A_Origin;                 
	Bomb_Scale = A_Zero;

	Increase_Done = false;
	Is_Bomb_Effect_Playing = true;
	Bomb_Effect_State = BOMB_EFFECT_STATE::ENTERING;
}


void Draw_Number(int Num, int Count, float Alpha)
{
	if (Num < 0 || Num > 9) return;

	float UV_start = SCORE_DISPLAY_WIDTH * Num;
	float Draw_X = Score_Num_X + (Score_Num_Width * Count);

	Sprite_UV_Draw(UI_Number_Texture, Draw_X, Score_Num_Y, Score_Num_Width, Score_Num_Height, UV_start, A_Zero, 
					SCORE_DISPLAY_WIDTH, SCORE_DISPLAY_HEIGHT, A_Zero, { A_Origin, A_Origin, A_Origin, Alpha });
}


void Draw_Number(float D_X, float D_Y, float D_W, float D_H, int Num, bool Is_Loop, int Count, float Alpha)
{
	if (Num < 0 || Num > 9) return;

	float UV_start = SCORE_DISPLAY_WIDTH * Num;
	float Draw_X = D_X;

	if (Is_Loop)
		Draw_X += D_W * Count;

	Sprite_UV_Draw(UI_Number_Texture, Draw_X, D_Y, D_W, D_H, UV_start, A_Zero, 
					SCORE_DISPLAY_WIDTH, SCORE_DISPLAY_HEIGHT, A_Zero, { A_Origin, A_Origin, A_Origin, Alpha });
}

void Stage_Ready_Initialize()
{
	Deployment_BG_Width	 = Stage_BG_Width * A_Half;
	Deployment_BG_Height = static_cast<float>(Game_Screen_Height) * A_One_Tenth;
	Deployment_BG_X = Stage_BG_X + (Stage_BG_Width * A_Half) - (Deployment_BG_Width * A_Half);
	Deployment_BG_Y = (static_cast<float>(Game_Screen_Height) * A_Quarter) - (Deployment_BG_Height * A_Half);

	Initiated_BG_Width  = Deployment_BG_Width * A_F_Fifths;
	Initiated_BG_Height = Deployment_BG_Height * A_Half;
	Initiated_BG_X = Stage_BG_X + (Stage_BG_Width * A_Half) - (Initiated_BG_Width * A_Half);
	Initiated_BG_Y = Deployment_BG_Y + Deployment_BG_Height;

	Mission_BG_Width  = Initiated_BG_Width * A_Half;
	Mission_BG_Height = Initiated_BG_Height * A_F_Fifths;
	Mission_BG_X = Stage_BG_X + (Stage_BG_Width * A_Half) - (Mission_BG_Width * A_Half);
	Mission_BG_Y = Deployment_BG_Y - Mission_BG_Height;

	Final_Ready_Width = Deployment_BG_Width;
	Final_Ready_Height = Deployment_BG_Height + Initiated_BG_Height + Mission_BG_Height;

	Is_Anime_Draw_Done = false;
	Is_Ready_Anime_Finished = false;
	Is_Final_Ready_Anime_Finished = false;
}

void Stage_Ready_Finalize()
{
	Is_Final_Ready_Anime_Finished = false;
	Is_Ready_Anime_Finished = false;
	Is_Anime_Draw_Done = false;

	Deployment_BG_X = 0;
	Deployment_BG_Y = 0;
	Deployment_BG_Width = 0;
	Deployment_BG_Height = 0;

	Initiated_BG_X = 0;
	Initiated_BG_Y = 0;
	Initiated_BG_Width = 0;
	Initiated_BG_Height = 0;

	Mission_BG_X = 0;
	Mission_BG_Y = 0;
	Mission_BG_Width = 0;
	Mission_BG_Height = 0;

	Final_Ready_Width = 0;
	Final_Ready_Height = 0;
}

void Stage_Ready_Draw()
{
	if (Is_Anime_Draw_Done && Is_Ready_Anime_Finished) return;

	if (SpriteAni_IsStopped(Play_Deployment_TexID) && SpriteAni_IsStopped(Play_Initiated_TexID) && SpriteAni_IsStopped(Play_Mission_TexID))
		Is_Anime_Draw_Done = true;

	if (Is_Anime_Draw_Done)
	{
		if (Start_BG_Alpha > A_Zero)
		{
			Sprite_Draw(Deployment_TexID, Deployment_BG_X, Deployment_BG_Y, Deployment_BG_Width, Deployment_BG_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Start_BG_Alpha });
			Sprite_Draw(Initiated_TexID, Initiated_BG_X, Initiated_BG_Y, Initiated_BG_Width, Initiated_BG_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Start_BG_Alpha });
			Sprite_Draw(Mission_TexID, Mission_BG_X, Mission_BG_Y, Mission_BG_Width, Mission_BG_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Start_BG_Alpha });
			Start_BG_Alpha -= A_Zero_Five;

			if (Start_BG_Alpha < A_Zero)
				Start_BG_Alpha = A_Zero;
		}
		else
			Is_Ready_Anime_Finished = true;
	}
	else
	{
		SpriteAni_Draw(Play_Deployment_TexID, Deployment_BG_X, Deployment_BG_Y, Deployment_BG_Width, Deployment_BG_Height);
		SpriteAni_Draw(Play_Initiated_TexID, Initiated_BG_X, Initiated_BG_Y, Initiated_BG_Width, Initiated_BG_Height);
		SpriteAni_Draw(Play_Mission_TexID, Mission_BG_X, Mission_BG_Y, Mission_BG_Width, Mission_BG_Height);
	}
}

void Stage_Ready_Reset()
{
	Is_Anime_Draw_Done = false;
	Is_Ready_Anime_Finished = false;
	Start_BG_Alpha = A_Origin;

	// Make Reset Animation ID
	SpriteAni_DestroyPlayer(Play_Deployment_TexID);
	SpriteAni_DestroyPlayer(Play_Initiated_TexID);
	SpriteAni_DestroyPlayer(Play_Mission_TexID);
	Play_Deployment_TexID = SpriteAni_CreatePlayer(Anime_Deployment_TexID);
	Play_Initiated_TexID = SpriteAni_CreatePlayer(Anime_Initiated_TexID);
	Play_Mission_TexID = SpriteAni_CreatePlayer(Anime_Mission_TexID);
}

bool Is_Ready_To_Logic_Draw()
{
	return Is_Ready_Anime_Finished;
}

void Stage_1_Background_Draw()
{
	Stage_BG_Y += (1.5f * Game_Scale);
	Stage_Space_BG_Y += (1.5f * Game_Scale);

	if (Stage_BG_Y >= static_cast<float>(static_cast<float>(Game_Screen_Height)))
		Stage_BG_Y = -static_cast<float>(static_cast<float>(Game_Screen_Height));

	if (Stage_Space_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_Space_BG_Y = -static_cast<float>(Game_Screen_Height);

	Sprite_Draw(Stage_BG_1_TexID, Stage_BG_X, Stage_BG_Y, Stage_BG_Width, Stage_BG_Height);
	Sprite_Draw(Stage_BG_1_TexID, Stage_BG_X, Stage_Space_BG_Y, Stage_BG_Width, Stage_BG_Height);
}

void Stage_2_Background_Draw()
{
	Stage_BG_Y += (1.5f * Game_Scale);
	Stage_Space_BG_Y += (1.5f * Game_Scale);

	if (Stage_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_BG_Y = -static_cast<float>(Game_Screen_Height);

	if (Stage_Space_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_Space_BG_Y = -static_cast<float>(Game_Screen_Height);

	Sprite_Draw(Stage_BG_2_TexID, Stage_BG_X, Stage_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
	Sprite_Draw(Stage_BG_2_TexID, Stage_BG_X, Stage_Space_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
}

void Stage_3_Background_Draw()
{
	Stage_BG_Y += (1.5f * Game_Scale);
	Stage_Space_BG_Y += (1.5f * Game_Scale);

	if (Stage_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_BG_Y = -static_cast<float>(Game_Screen_Height);

	if (Stage_Space_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_Space_BG_Y = -static_cast<float>(Game_Screen_Height);

	Sprite_Draw(Stage_BG_3_TexID, Stage_BG_X, Stage_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
	Sprite_Draw(Stage_BG_3_TexID, Stage_BG_X, Stage_Space_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
}

void Stage_4_Background_Draw()
{
	Stage_BG_Y += (1.5f * Game_Scale);
	Stage_Space_BG_Y += (1.5f * Game_Scale);

	if (Stage_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_BG_Y = -static_cast<float>(Game_Screen_Height);

	if (Stage_Space_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_Space_BG_Y = -static_cast<float>(Game_Screen_Height);

	Sprite_Draw(Stage_BG_4_TexID, Stage_BG_X, Stage_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
	Sprite_Draw(Stage_BG_4_TexID, Stage_BG_X, Stage_Space_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
}

void Stage_5_Background_Draw()
{
	Stage_BG_Y += (1.5f * Game_Scale);
	Stage_Space_BG_Y += (1.5f * Game_Scale);

	if (Stage_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_BG_Y = -static_cast<float>(Game_Screen_Height);

	if (Stage_Space_BG_Y >= static_cast<float>(Game_Screen_Height))
		Stage_Space_BG_Y = -static_cast<float>(Game_Screen_Height);

	Sprite_Draw(Stage_BG_5_TexID, Stage_BG_X, Stage_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
	Sprite_Draw(Stage_BG_5_TexID, Stage_BG_X, Stage_Space_BG_Y, Stage_BG_Width, Stage_BG_Height, A_Zero);
}

void Stage_5_BlackOut_Draw()
{
	Sprite_Draw(Stage_5_BlackOut_TexID, 0.0f, 0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
}

void Stage_BOSS_Background_Draw()
{
	SpriteAni_Draw(Play_Final_BG, 0.0f, 0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
}

void Stage_BOSS_Ready_Update(double elapsed_time)
{
	switch (Anime_Flow)
	{
	case FINAL_BOSS_ANIME::NONE:
	case FINAL_BOSS_ANIME::IDLE:
	case FINAL_BOSS_ANIME::ANIME_DONE:
		break;

	case FINAL_BOSS_ANIME::ANIME_1_START:
		if (SpriteAni_IsStopped(Play_Final_Ready_1))
		{
			Ready_State_Timer += elapsed_time;
			if (Ready_State_Timer > 1.0)
				Set_Now_Anime_Flow(FINAL_BOSS_ANIME::ANIME_1_DONE);
		}
		break;

	case FINAL_BOSS_ANIME::ANIME_1_DONE:
		Set_Blackout_Overlay(false);
		Fade_Start(0.5, false, White);
		Set_Now_Anime_Flow(FINAL_BOSS_ANIME::ANIME_2_START);
		break;

	case FINAL_BOSS_ANIME::ANIME_2_START:
		if (Fade_GetState() == FADE_STATE::FINISHED_IN)
			Set_Now_Anime_Flow(FINAL_BOSS_ANIME::ANIME_2_DONE);
		break;

	case FINAL_BOSS_ANIME::ANIME_2_DONE:
		if (!Is_Final_Ready_Anime_Finished)
		{
			if (Final_Start_BG_Alpha > A_Zero)
			{
				Final_Start_BG_Alpha -= A_Origin * static_cast<float>(elapsed_time);

				if (Final_Start_BG_Alpha <= A_Zero)
				{
					Final_Start_BG_Alpha = A_Zero;
					Is_Final_Ready_Anime_Finished = true;
				}
			}
			else
			{
				Final_Start_BG_Alpha = A_Zero;
				Is_Final_Ready_Anime_Finished = true;
			}
		}
		break;
	}
}

void Stage_BOSS_Ready_Draw()
{
	switch (Anime_Flow)
	{
	case FINAL_BOSS_ANIME::ANIME_1_START:
		SpriteAni_Draw(Play_Final_Ready_1, Deployment_BG_X, Deployment_BG_Y, Final_Ready_Width, Final_Ready_Height);
		break;

	case FINAL_BOSS_ANIME::ANIME_1_DONE:
		Sprite_Draw(Final_Ready_1, Deployment_BG_X, Deployment_BG_Y, Final_Ready_Width, Final_Ready_Height);
		break;

	case FINAL_BOSS_ANIME::ANIME_2_START:
		Sprite_Draw(Final_Ready_2, Deployment_BG_X, Deployment_BG_Y, Final_Ready_Width, Final_Ready_Height);
		break;

	case FINAL_BOSS_ANIME::ANIME_2_DONE:
		if (Final_Start_BG_Alpha > A_Zero)
			Sprite_Draw(Final_Ready_2, Deployment_BG_X, Deployment_BG_Y, Final_Ready_Width, Final_Ready_Height, A_Zero, { A_Origin, A_Origin, A_Origin, Final_Start_BG_Alpha });
		break;
	}
}

void Stage_BOSS_Ready_Draw_Start()
{
	Stage_Final_Ready_Reset();
	Set_Now_Anime_Flow(FINAL_BOSS_ANIME::ANIME_1_START);
}

void Stage_Final_Ready_Reset()
{
	Final_Start_BG_Alpha = A_Origin;
	Is_Final_Ready_Anime_Finished = false;
	Ready_State_Timer = 0.0;

	// Make Reset Animation ID
	SpriteAni_DestroyPlayer(Play_Final_Ready_1);
	SpriteAni_DestroyPlayer(Play_Final_Ready_2);
	Play_Final_Ready_1 = SpriteAni_CreatePlayer(Anime_Final_Ready_1);
	Play_Final_Ready_2 = SpriteAni_CreatePlayer(Anime_Final_Ready_2);
}

bool Is_Ready_To_Final_Battle()
{
	return Is_Final_Ready_Anime_Finished;
}

void Set_Now_Anime_Flow(FINAL_BOSS_ANIME State)
{
	Anime_Flow = State;
}

FINAL_BOSS_ANIME Get_Now_Anime_Flow()
{
	return Anime_Flow;
}

void Game_BG_UI_Texture()
{
	//----------------Stage Background----------------//
	Stage_BG_1_TexID = Texture_M->GetID("Stage_1");
	Stage_BG_2_TexID = Texture_M->GetID("Stage_2");
	Stage_BG_3_TexID = Texture_M->GetID("Stage_3");
	Stage_BG_4_TexID = Texture_M->GetID("Stage_4");
	Stage_BG_5_TexID = Texture_M->GetID("Stage_5");
	Stage_5_BlackOut_TexID = Texture_M->GetID("K");

	int Final_BG = Texture_M->GetID("Stage_Final");
	Anime_Final_BG = SpriteAni_Get_Pattern_Info(Final_BG, 10, 5, 0.1, { 1200, 1200 }, { 1200 * 0, 1200 * 0 }, true);
	Play_Final_BG = SpriteAni_CreatePlayer(Anime_Final_BG);

	//-----------------------UI------------------------//
	UI_BG_L = Texture_M->GetID("UI_BG_Left");
	UI_BG_R = Texture_M->GetID("UI_BG_Right");
	UI_Title = Texture_M->GetID("UI_Title");

	UI_FS_L = Texture_M->GetID("UI_BG_FullScreen_L");
	UI_FS_R = Texture_M->GetID("UI_BG_FullScreen_R");

	UI_Life = Texture_M->GetID("UI_Text_Life");
	UI_Bomb = Texture_M->GetID("UI_Text_Bomb");
	UI_Score = Texture_M->GetID("UI_Text_Score");
	UI_Power = Texture_M->GetID("UI_Text_Power");

	UI_P_Bar_Min = Texture_M->GetID("UI_Power_Bar_Default");
	UI_P_Bar_Max = Texture_M->GetID("UI_Power_Bar_Max");
	UI_P_Bar_Fill = Texture_M->GetID("UI_Power_Bar_Charge");

	UI_Bomb_Effect_Man = Texture_M->GetID("Player_Bomb_Effect_Man");
	UI_Bomb_Effect_Ring = Texture_M->GetID("Player_Bomb_Effect_Ring");

	UI_Life_Texture = Texture_M->GetID("UI_Life");
	UI_Bomb_Texture = Texture_M->GetID("UI_Bomb");

	UI_BOSS_HP = Texture_M->GetID("UI_Boss_HP");
	UI_BOSS_HP_Fill = Texture_M->GetID("UI_Boss_HP_Fill");
	UI_BOSS_HP_Frame = Texture_M->GetID("UI_Boss_HP_Frame");

	UI_Number_Texture = Texture_M->GetID("UI_Score_Number");

	//------------------Stage Start UI------------------//
	int Deployment = Texture_M->GetID("UI_Alert_Anime_Deployment");
	int Initiated = Texture_M->GetID("UI_Alert_Anime_Initiated");
	int Mission = Texture_M->GetID("UI_Alert_Anime_Mission_Start");

	Anime_Deployment_TexID = SpriteAni_Get_Pattern_Info(Deployment, 12, 4, 0.1, { 512, 128 }, { 512 * 0, 128 * 0 }, false);
	Anime_Initiated_TexID = SpriteAni_Get_Pattern_Info(Initiated, 12, 4, 0.1, { 768, 128 }, { 768 * 0, 128 * 0 }, false);
	Anime_Mission_TexID = SpriteAni_Get_Pattern_Info(Mission, 8, 4, 0.2, { 512, 128 }, { 512 * 0, 128 * 0 }, false);

	Play_Deployment_TexID = SpriteAni_CreatePlayer(Anime_Deployment_TexID);
	Play_Initiated_TexID = SpriteAni_CreatePlayer(Anime_Initiated_TexID);
	Play_Mission_TexID = SpriteAni_CreatePlayer(Anime_Mission_TexID);

	Deployment_TexID = Texture_M->GetID("UI_Alert_Deployment");
	Initiated_TexID = Texture_M->GetID("UI_Alert_Initiated");
	Mission_TexID = Texture_M->GetID("UI_Alert_Mission_Start");

	int FInal_1 = Texture_M->GetID("UI_Alert_Anime_Final_Mission_1");
	int FInal_2 = Texture_M->GetID("UI_Alert_Anime_Final_Mission_2");

	Anime_Final_Ready_1 = SpriteAni_Get_Pattern_Info(FInal_1, 10, 5, 0.1, { 1131, 729 }, { 1131 * 0, 729 * 0 }, false);
	Anime_Final_Ready_2 = SpriteAni_Get_Pattern_Info(FInal_2, 10, 5, 0.1, { 1131, 729 }, { 1131 * 0, 729 * 0 }, false);

	Play_Final_Ready_1 = SpriteAni_CreatePlayer(Anime_Final_Ready_1);
	Play_Final_Ready_2 = SpriteAni_CreatePlayer(Anime_Final_Ready_2);

	Final_Ready_1 = Texture_M->GetID("UI_Alert_Final_Mission_1");
	Final_Ready_2 = Texture_M->GetID("UI_Alert_Final_Mission_2");
}

#if defined(DEBUG) || defined(_DEBUG)
// Use For Debug
void Debug_BG_Draw(const std::string& RGBCMYK)
{
	Sprite_Draw(Texture_M->GetID(RGBCMYK), 0.0f, 0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
}
void Debug_BG_Draw(const std::string& RGBCMYK, const std::wstring& Overloading)
{
	int ID = Texture_M->Load(RGBCMYK, Overloading);

	if (ID != -1)
		Sprite_Draw(ID, 0.0f, 0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT));
}
#endif

