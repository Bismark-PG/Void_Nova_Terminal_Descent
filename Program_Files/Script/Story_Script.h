/*==============================================================================

	Story Script Manager [Story_Script.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H
#include "Stage_Select.h"
#include <DirectXMath.h>
#include <Story_Manager.h>

enum class SCRIPT_ANIMATION_STATE
{
	IN_ACTIVE,
	BG_INTRO_VERTICAL,   
	BG_INTRO_HORIZONTAL, 
	COMMANDER_BOX_INTRO, 
	COMMANDER_FADE_IN,   
	TEXT_ACTIVE,         
	COMMANDER_BOX_OUTRO, 
	BG_OUTRO,            
	FINISHED
};

enum class PLAYER_MOVE_POSITION
{
	STAGE_START_POS,
	STAGE_3_BOSS_START_POS,
	STAGE_4_BOSS_START_POS,
	STAGE_5_BOSS_START_POS
};

static constexpr float STORY_WIDTH_SACLE = 0.8f;
static constexpr float STORY_HEIGHT_SACLE = 0.3f;
static constexpr float COMMANDER_SIZE_SACLE = 0.2f;
static constexpr float BUTTON_SIZE_SACLE = 0.2f;

static float Story_X;
static float Story_Y;
static float Story_Width;
static float Story_Height;
static float Story_Direction_X;
static float Story_Direction_Y;
static float Story_Direction_Width;
static float Story_Direction_Height;
static float Story_Alpha;

static float Story_Enemy_Y;
static float Story_Enemy_Button_Y;

static float Commander_X;
static float Commander_Y;
static float Commander_Size;
static float Commander_Direction_X;
static float Commander_Direction_Y;
static float Commander_Direction_Size;
static float Commander_Alpha;

static float Story_Button_X;
static float Story_Button_Y;
static float Story_Button_Size;

static constexpr float Story_Direction_Limit = 0.5f;
static constexpr float Commander_Direction_Limit = 0.5f;
static constexpr float Intro_Direction_Speed = 2.5f;
static constexpr float Outro_Direction_Speed = 3.0f;
static constexpr float Commander_Alpha_Direction_Speed = 2.0f;

static double Page_Input_Cooldown_Timer = 0.0;
static constexpr double PAGE_INPUT_DELAY = 0.5;

static double Boss_Page_Auto_Timer = 0.0;
static constexpr double BOSS_PAGE_AUTO_DELAY = 2.0;

static DirectX::XMFLOAT2 Stage_Start_Pos;
static DirectX::XMFLOAT2 Stage_3_Boss_Start_Pos;
static DirectX::XMFLOAT2 Stage_4_Boss_Start_Pos;
static DirectX::XMFLOAT2 Stage_5_Boss_Start_Pos;

static constexpr float Stage_1_Start_Speed = 100.0f;
static constexpr float Stage_2_Start_Speed = 50.0f;

void Script_Initialize();
void Script_Finalize();

bool Script_Update(double elapsed_time, bool Stoty_Type, bool Is_Boss_Story);

void Ending_Commander_Update(double elapsed_time);
void Ending_Commander_Draw();
void Ending_Commander_Start();

void Script_Animation_Draw(bool Is_Boss_Story);
void Script_Text_Draw(NOW_PLAYING_STAGE Now_Playing_Stage, int Now_Playing_Page, bool Is_Boss_Story, STORY_TYPE Type);

void Script_Begin(bool isAutoForward, bool Is_Boss_Story);
void Script_Start_Outro(bool Is_Boss_Story);

void Script_Reset();

void Script_Next_Button_Draw(bool Is_Boss_Story);

void Set_Script_State(SCRIPT_ANIMATION_STATE State);
SCRIPT_ANIMATION_STATE Get_Script_State();

bool Is_Script_Outro_Finished();

DirectX::XMFLOAT2 Script_Get_Enum(PLAYER_MOVE_POSITION POS);
void Script_Set_Player_Target_Pos();

void Script_Texture();

#endif // SCRIPT_MANAGER_H