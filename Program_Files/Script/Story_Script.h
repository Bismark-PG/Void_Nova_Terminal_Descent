/*==============================================================================

	Story Script Manager [Story_Script.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef SCRIPT_MANAGER_H
#define SCRIPT_MANAGER_H
#include "Stage_Select.h"
#include <DirectXMath.h>

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

extern DirectX::XMFLOAT2 Stage_1_2_3_Start_Pos;
extern DirectX::XMFLOAT2 Stage_3_Boss_Start_Pos;
extern DirectX::XMFLOAT2 Stage_4_Start_Pos;
extern DirectX::XMFLOAT2 Stage_4_Boss_Start_Pos;
extern DirectX::XMFLOAT2 Stage_5_Start_Pos;
extern DirectX::XMFLOAT2 Stage_5_Boss_Start_Pos;

static constexpr float Stage_1_Start_Speed = 100.0f;
static constexpr float Stage_2_Start_Speed = 50.0f;

void Script_Initialize();
void Script_Finalize();

bool Script_Update(double elapsed_time);
bool Script_Boss_Update(double elapsed_time);

void Ending_Commander_Update(double elapsed_time);
void Ending_Commander_Draw();
void Ending_Commander_Start();

void Script_Animation_Draw();
void Script_Start();
void Script_Start_Outro();
void Script_Reset();

void Script_Boss_Animation_Draw();
void Script_Boss_Start();
void Script_Boss_Start_Outro();
void Script_Boss_Reset();

void Script_Text_Draw(NOW_PLAYING_STAGE Now_Playing_Stage, int Now_Playing_Page, bool Is_Boss_Story);
void Script_Next_Button_Draw();

void Set_Script_State(SCRIPT_ANIMATION_STATE State, bool Is_Boss = false);

SCRIPT_ANIMATION_STATE Get_Script_State();
SCRIPT_ANIMATION_STATE Get_Boss_Script_State();
bool Is_Script_Outro_Finished(bool Is_Boss);

void Script_Set_Player_POS_Enum();

void Script_Stage_1_Draw(int Now_Page);
void Script_Stage_2_Draw(int Now_Page);
void Script_Stage_3_Draw(int Now_Page, bool is_boss_story);
void Script_Stage_4_Draw(int Now_Page, bool is_boss_story);
void Script_Stage_5_Draw(int Now_Page, bool is_boss_story);

#endif // SCRIPT_MANAGER_H