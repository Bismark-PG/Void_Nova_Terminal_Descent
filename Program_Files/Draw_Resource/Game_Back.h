/*==============================================================================

	Draw Game Background [Game_Back.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_BACK_H
#define GAME_BACK_H

#include "Game_Window.h"
#include "Texture.h"
#include "Sprite_Animation.h"
#include "Palette.h"

enum class BOSS_UI_STATE
{
	HIDDEN,
	ENTERING,
	VISIBLE,
	EXITING
};

enum class BOMB_EFFECT_STATE
{
	IDLE,
	ENTERING,
	FADING_OUT
};

static constexpr float SCORE_DISPLAY_WIDTH = 296.0f;
static constexpr float SCORE_DISPLAY_HEIGHT = 498.0f;

static float UI_TITLE_Y = 0.8f;
static float UI_TITLE_HEIGHT = 0.2f;
static float UI_IN_GAME_X = 0.1f;
static float UI_IN_GAME_HEIGHT = 0.05f;

static float UI_LIFE_Y = 0.1f;
static float UI_BOMB_Y = 3.0f;
static float UI_LIFE_BOMB_WIDTH  = 0.5f;
static float UI_SCORE_WIDTH  = 0.75f;

static float UI_Full_L_W;
static float UI_Full_L_X;
static float UI_Full_R_W;
static float UI_Full_R_X;

static float UI_Title_Y;
static float UI_Title_Height;

static float UI_Life_Bomb_X;
static float UI_Score_X;
static float UI_Life_Score_Y;
static float UI_Bomb_Y;

static float UI_Life_Bomb_Width;
static float UI_Score_Width;
static float UI_In_Game_Height;

static float UI_Life_Bomb_Num_X;
static float UI_Life_Num_Y;
static float UI_Bomb_Num_Y;
static float UI_Life_Bomb_Texture_X;
static float UI_Life_Bomb_Texture_Size;
static float UI_Life_Bomb_Num_Width;

static float UI_Power_X;
static float UI_Power_Y;
static float UI_Power_Width;
static float UI_Power_Height;

static float UI_Power_Bar_1_x;
static float UI_Power_Bar_2_x;
static float UI_Power_Bar_3_x;
static float UI_Power_Bar_4_x;
static float UI_Power_Bar_Y;
static float UI_Power_Bar_Width;
static float UI_Power_Bar_Height;
static constexpr float UI_Power_Bar_Width_Scale = 0.9f;

static float Stage_BG_X;
static float Stage_BG_Y;
static float Stage_Space_BG_Y;
static float Stage_Second_BG_Y;
static float Stage_BG_Width;
static float Stage_BG_Height;
static constexpr float Stage_BG_Height_Scale = 1.0025f;

static float Deployment_BG_X;
static float Deployment_BG_Y;
static float Deployment_BG_Width;
static float Deployment_BG_Height;

static float Initiated_BG_X;
static float Initiated_BG_Y;
static float Initiated_BG_Width;
static float Initiated_BG_Height;

static float Mission_BG_X;
static float Mission_BG_Y;
static float Mission_BG_Width;
static float Mission_BG_Height;

static float Score_Num_X;
static float Score_Num_Y;
static float Score_Num_Width;
static float Score_Num_Height;
static constexpr int SCORE_DIGITS = 10;

static float Bomb_Man;
static float Bomb_Man_Alpha;
static float Bomb_Scale;
static constexpr float Bomb_Man_Move_Scale = 10.0f;
static constexpr float Bomb_Scale_Increase = 25.0f;

void Game_Back_Initialize();
void Game_Back_Finalize();

void Game_Boss_UI_Initialize();
void Game_Boss_UI_Finalize();
void Game_Boss_UI_Update(double elapsed_time);
void Game_Boss_UI_Draw();

void Game_Boss_UI_Start_Appearance(int bossIndex);
void Game_Boss_UI_Start_Disappearance();

void Game_UI_Initialize();
void Game_UI_Finalize();
void Game_UI_And_Logo_Draw();
void Game_UI_Draw_Status_Numbers();

void Game_UI_Update_Bomb_Effect(double elapsed_time);
void Game_UI_Draw_Bomb_Effect();
void Game_Draw_Bomb_Effect(float P_X, float P_Y);
void Game_UI_Start_Bomb_Effect();

// Default Score Numver Draw
void Draw_Number(int Num, int Count, float Alpha);

// Draw Numder With POS And Alpha // True : Loop / False : Don`t Loop
void Draw_Number(float D_X, float D_Y, float D_W, float D_H, int Num, bool Is_Loop, int Count = 0, float Alpha = PALETTE::A_T_Quarters);

void Stage_Ready_Initialize();
void Stage_Ready_Finalize();
void Stage_Ready_Draw();
void Stage_Ready_Reset();
bool Is_Ready_To_Logic_Draw();

void Stage_1_Background_Draw();
void Stage_2_Background_Draw();
void Stage_3_Background_Draw();
void Stage_4_Background_Draw();
void Stage_5_Background_Draw();
void Stage_BOSS_Background_Draw();

#if defined(DEBUG) || defined(_DEBUG)
void Debug_BG_Draw();
#endif

#endif // GAME_BACK_H