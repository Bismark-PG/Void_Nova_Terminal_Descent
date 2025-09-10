/*==============================================================================

	Game Main Menu [Game_Menu.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef GAME_MENU
#define GAME_MENU

#include "Title.h"
#include "Setting.h"
#include "Game_Select.h"

enum class UI_STATE
{
	NONE,
	GLOW,
	MENU_UI
};

enum MENU_BUFFER : int
{
	NONE,
	START,
	SETTINGS,
	EXIT,
	MAIN_BACK,
	DONE
};

static int INTRO_WIDTH = 10;
static int INTRO_HEIGHT = 1;
static constexpr int INTRO_SCALE = 50;

static int UI_WIDTH = 5;
static int UI_HEIGHT = 1;
static constexpr int UI_SCALE = 30;

static float MENU_X;
static float MENU_Y;
static float MENU_WIDTH;
static float MENU_HEIGHT;

static float BACK_WIDTH;
static float BACK_HEIGHT;

static float Intro_X;
static float Intro_Y;
static float Intro_Width;
static float Intro_Height;

static float UI_Y;
static float UI_Width;
static float UI_Height;

static float Start_X;
static float Settings_X;
static float Exit_X;

static constexpr float Menu_Alpha_Scale = 0.01f;
static constexpr double FADE_OUT_TIME   = 2.0;

void Menu_Initialize();
void Menu_Finalize();

void Menu_Update();

void Menu_UI_Draw();

void Update_Main_Buffer(UI_STATE UI_Buffer);
UI_STATE Get_Main_Draw_State();

void Update_Main_Select_Buffer(MENU_BUFFER Main_Buffer);
MENU_BUFFER Get_Main_Select_Buffer();

void Set_Wait_For_Release(bool Release_State);
bool Get_Wait_For_Release();

bool IF_IS_Game_Done();

#endif // GAME_MENU