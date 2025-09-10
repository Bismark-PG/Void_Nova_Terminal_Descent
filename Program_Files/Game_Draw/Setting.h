/*==============================================================================

	Game Setting [Setting.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef SETTING_H
#define SETTING_H
#include "Game_Menu.h"
#include "Update_Screen.h"
#include "KeyLogger.h"
#include "Sprite_Animation.h"
#include "Window_Re_Creator.h"

enum class SETTING_BUFFER
{
	SETTING_NONE,
	SETTING_WAIT,
	BGM,
	SFX,
	SOUND_SETTING,
	WINDOW_MODE,
	FULL_SCREEN_MODE,
	SETTING_BACK,
	SETTING_DONE,
	DONE
};

enum class SOUND_SETTING_STATE
{
	NONE,
	BGM_SETTING,
	SFX_SETTING,
	SET_DONE
};

enum SOUND_SCALE_BUFFER : int
{
	SOUND_MIN,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	SOUND_MAX
};
// Setting X
static constexpr float UI = 0.2f;
static constexpr float SOUND = 0.4f;
static constexpr float L_BUTTON = 0.5f;
static constexpr float NUMBER = 0.6f;
static constexpr float R_BUTTON = 0.7f;

// Setting Y
static constexpr float BGM = 0.2f;
static constexpr float SFX = 0.35f;
static constexpr float W_MODE = 0.5f;
static constexpr float FS_MODE = 0.65f;
static constexpr float BACK = 0.85f;

static constexpr float NUM_MAX_W = 0.85f;

static float SOUND_BGM_X;
static float SOUND_BGM_Y;
static float SOUND_SFX_X;
static float SOUND_SFX_Y;

static float S_W_MODE_X;
static float S_W_MODE_Y;
static float S_FS_MODE_X;
static float S_FS_MODE_Y;

static float BACK_X;
static float BACK_Y;

static float SOUND_UI_WIDTH	   = 2;
static float W_MODE_UI_WIDTH   = 7;
static float FS_MODE_UI_WIDTH  = 9;
static float SETTING_UI_HEIGHT = 1;

static constexpr int SETTING_UI_SCALE  = 50;

static float BACK_UI_WIDTH   = 3;
static float BACK_UI_HEIGHT	 = 1;

static int BACK_UI_SCALE	 = 50;

static float L_BUTTON_X;
static float R_BUTTON_X;
static float BGM_BUTTON_Y;
static float SFX_BUTTON_Y;

static float NUM_X;
static float NUM_MAX_X;
static float BGM_NUM_Y;
static float SFX_NUM_Y;

static float NUM_WIDTH		= 3;
static float NUM_HEIGHT		= 5;
static float NUM_MAX_WIDTH  = 0;

static constexpr int NUM_SCALE = 10;

void Setting_Initialize();
void Setting_Finalize();

void Setting_Update();

void Setting_Draw();
void Setting_UI_Draw();

void Update_Setting_Buffer(SETTING_BUFFER Buffer);
SETTING_BUFFER Get_Setting_Buffer();

void Update_Sound_Setting_State(SOUND_SETTING_STATE State);
SOUND_SETTING_STATE Get_Setting_State();

void Update_BGM_Scale_Buffer(SOUND_SCALE_BUFFER BGM_Scale);
SOUND_SCALE_BUFFER Get_BGM_Scale_Buffer();
void Update_SFX_Scale_Buffer(SOUND_SCALE_BUFFER SFX_Scale);
SOUND_SCALE_BUFFER Get_SFX_Scale_Buffer();

void Set_Mode_Buffer(ScreenMode Mode);
ScreenMode Get_Mode_Buffer();

void Set_Mode_Pre_Buffer(ScreenMode Mode);
ScreenMode Get_Mode_Pre_Buffer();

void Request_Screen_Mode_Change(ScreenMode Mode);
ScreenMode Screen_Mode_Change_Request();

extern ScreenMode Now_W_Mode_Buffer;
extern ScreenMode Pre_W_Mode_Buffer;

void Setting_Sound_TexID(int Now_Scale, SOUND_SETTING_STATE Now_State);

#endif // SETTING_H