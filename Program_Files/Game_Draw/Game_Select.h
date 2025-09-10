
/*==============================================================================

	Game Stage Select Menu [Game_Select.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_STAGE_SELECT
#define GAME_STAGE_SELECT

#include "Game_Menu.h"
#include "Update_Screen.h"
#include "KeyLogger.h"
#include "Sprite_Animation.h"
#include "Fade.h" 
#include <DirectXMath.h>
#include "Palette.h"

enum class SELECT_GAME
{
	NONE,
	INTRO_WAIT,
	BACK_WAIT,
	SELECT_WAIT,
	SELECT_NEW_GAME,
	SELECT_CONTINUE,
	SELECT_STAGE,
	STAGE_WAIT,
	SELECT_BACK,
	FADING_TO_STAGE,	
	DONE
};

enum class SELECT_STAGE_STATE
{
	NONE,
	STAGE_WAIT,
	STAGE_SELECT,
	DONE
};

enum STAGE_NUM : int
{
	STAGE_NONE,
	STAGE_1,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
};


static int S_TITLE_WIDTH  = 16;
static int S_TITLE_HEIGHT = 9;
constexpr int S_TITLE_SCALE = 40;

static int S_UI_WIDTH  = 5;
static int S_UI_HEIGHT = 1;
constexpr int S_UI_SCALE = 30;

static int STAGE_NUM_WIDTH  = 1;
static int STAGE_NUM_HEIGHT = 2;
constexpr int STAGE_NUM_SCALE = 20;

static int PLAYER_INTRO_WIDTH  = 5;
static int PLAYER_INTRO_HEIGHT = 4;

static float Player_Intro_Width;
static float Player_Intro_Height;
static float Player_Intro_X;
static float Player_Intro_Y;
static constexpr float Player_Intro_Scale = 90;

static float S_Title_X;
static float S_Title_Y;
static float S_Title_Width;
static float S_Title_Height;

static float Expend_Scale;
static float BG_Angle;

static float S_BG_X;
static float S_BG_Y;
static float S_BG_Width;
static float S_BG_Height;

static float S_UI_X;
static float S_UI_Width;
static float S_UI_Height;

static float UI_Select_X;

static float UI_New_Game_Y;
static float UI_Continue_Y;
static float UI_Stage_Y;
static float UI_Back_Y;

static float UI_New_Game_Width;
static float UI_Continue_Width;
static float UI_Stage_Width;
static float UI_Back_Width;

static constexpr float UI_New_Game_Scale = 1.5f;
static constexpr float UI_Continue_Scale = 1.25f;
static constexpr float UI_Stage_Scale	 = 0.75f;
static constexpr float UI_Back_Scale	 = 0.75f;

static float Stage_Num_X;
static float Stage_Num_Y;
static float Stage_Up_Y;
static float Stage_Down_Y;
static float Stage_Num_Width;
static float Stage_Num_Height;
constexpr float Stage_Num_Up_Angle = PALETTE::R_Half_Pie * (DirectX::XM_PI / PALETTE::R_One_Pie);
constexpr float Stage_Num_Down_Angle = PALETTE::R_One_And_Half_Pie * (DirectX::XM_PI / PALETTE::R_One_Pie);

void Game_Select_Initialize();
void Game_Select_Finalize();

void Game_Select_Update();

void Game_Select_Draw();

void Game_Select_BG_Draw();
void Game_Select_Player_Draw();
void Game_Select_UI_Draw();

void Update_Game_Select_Buffer(SELECT_GAME Buffer);
SELECT_GAME Get_Game_Select_Buffer();

void Update_Select_Stage_State(SELECT_STAGE_STATE State);
SELECT_STAGE_STATE Get_Select_Stage_State();	

void Update_Stage_Select_Buffer_State(STAGE_NUM Stage_State);
STAGE_NUM Get_Stage_Select_Buffer_State();

void Stage_TexID(int Stage_State);

#endif // GAME_STAGE_SELECT