/*==============================================================================

	Script Draw For Stage One [Script_Draw.h]

	Author : Choi HyungJoon

==============================================================================*/
#include <Story_Manager.h>
#ifndef SCRIPT_STAGE_1
#define SCRIPT_STAGE_1

static float Draw_X;
static float Draw_Y;
static float Draw_Enemy_Y;
static float Draw_Width;
static float Draw_Height;

void Set_Position(float X, float Y, float E_Y, float W, float H);
void Script_Draw_Initialize();

void Script_Stage_1_Draw(int Now_Page, STORY_TYPE Type);
void Script_Stage_2_Draw(int Now_Page, STORY_TYPE Type);
void Script_Stage_3_Draw(int Now_Page, STORY_TYPE Type);
void Script_Stage_4_Draw(int Now_Page, STORY_TYPE Type);
void Script_Stage_5_Draw(int Now_Page, STORY_TYPE Type);

bool Is_Current_Script_Done();
void Is_Page_Complete();

void Script_Stage_1(int Now_Page);
void Script_Stage_2(int Now_Page);

void Script_Stage_3(int Now_Page);
void Script_Stage_3_BOSS(int Now_Page);

void Script_Stage_4(int Now_Page);
void Script_Stage_4_BOSS(int Now_Page);

void Script_Stage_5(int Now_Page);
void Script_Stage_5_BOSS_Phase_1(int Now_Page);
void Script_Stage_5_BOSS_Phase_2(int Now_Page);


#endif // SCRIPT_STAGE_1