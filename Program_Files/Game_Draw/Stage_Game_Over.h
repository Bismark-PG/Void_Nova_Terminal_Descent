/*==============================================================================

	Game Over Draw [Stage_Game_Over.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_OVER_H
#define GAME_OVER_H

enum class STAGE_OUTRO_STATE
{
	IN_ACTIVE,		   
	BG_FADE_IN,		   
	UI_FADE_IN,		   
	SCORE_COUNT,	   
	WAIT_BEFORE_FADE,  
	ALL_FADE_OUT,	   
	FINISHED		   
};

enum class GAME_OVER_MENU_BUFFER
{
	NONE,
	PLAYER_DEAD,
	RETRY,
	GO_MAIN,
	DONE
};

enum class GAME_OVER_MENU_SELCETED
{
	NONE,
	RE_START,
	GO_TO_MAIN
};

static float BG_A_Width;
static float BG_A_Height;

static float Game_Over_Title_X;
static float Game_Over_Title_Y;
static float Game_Over_Title_Width;
static float Game_Over_Title_Height;

static float Game_Over_Status_X;
static float Game_Over_Status_Y;
static float Game_Over_Status_Width;
static float Game_Over_Status_Height;

static float Game_Over_JP_X;
static float Game_Over_JP_Y;
static float Game_Over_JP_Width;
static float Game_Over_JP_Height;

static float Game_Over_UI_X;
static float Game_Over_UI_JP_X;
static float Game_Over_UI_Width;
static float Game_Over_UI_Height;

static float Retry_Y;
static float Retry_JP_Y;
static float Go_To_Main_Y;
static float Go_To_Main_JP_Y;

static float Clear_Score_X;
static float Clear_Score_Y;
static float Clear_Score_Width;
static float Clear_Score_Height;

static constexpr float BG_Alpha_Increase = 0.0125f;
static constexpr float UI_Alpha_Increase = 0.025f;
static constexpr float Menu_Alpha_Increase = 0.025f;
static constexpr float Clear_Alpha_Increase = 0.025f;
static constexpr float Score_Alpha_Increase = 0.025f;

void Game_Over_Initialize();
void Game_Over_Finalize();

void Game_Over_Update();
void Game_Over_Draw();
void Stage_Over_Draw_Reset();

void Game_Paused_Draw();
void Set_Is_Paused_Menu(bool is_paused);
bool Is_Paused_Menu();

void Stage_Outro_Start();
void Stage_Outro_Update(double elapsed_time);
void Stage_Outro_Draw();
bool Is_Stage_Outro_Finished();

void Stage_Done_BG_Draw();
void Stage_Failed_UI_Draw();
void Stage_Clear_UI_Draw();

void Set_Game_Over_Input(GAME_OVER_MENU_BUFFER Input);
GAME_OVER_MENU_BUFFER Get_Game_Over_Input();

void Set_Game_Over_Menu_Selected(GAME_OVER_MENU_SELCETED Selected);
GAME_OVER_MENU_SELCETED Get_Game_Over_Menu_Selected();

#endif // GAME_OVER_H
