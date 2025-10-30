/*==============================================================================

	Script Draw For Stage One [Script_Draw.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Script_Draw.h"
#include "Sprite.h"
#include "Game_Window.h"
#include "Palette.h"
#include <Texture_Manager.h>
using namespace PALETTE;

static bool Is_Page_Draw_Complete;

void Set_Position(float X, float Y, float E_Y, float W, float H)
{
	Draw_X = X;
	Draw_Y = Y;
	Draw_Enemy_Y = E_Y;
	Draw_Width  = W;
	Draw_Height = H;
}

void Script_Draw_Initialize()
{
	Is_Page_Draw_Complete = false;
}

void Script_Stage_1_Draw(int Now_Page, STORY_TYPE Type)
{
	switch (Type)
	{
	case STORY_TYPE::INTRO_STAGE:
		Script_Stage_1(Now_Page);
		break;
	case STORY_TYPE::OUTRO_STAGE:
		// Outro?
		break;
	}
}

void Script_Stage_2_Draw(int Now_Page, STORY_TYPE Type)
{
	switch (Type)
	{
	case STORY_TYPE::INTRO_STAGE:
		Script_Stage_2(Now_Page);
		break;
	case STORY_TYPE::OUTRO_STAGE:
		// Outro?
		break;
	}
}

void Script_Stage_3_Draw(int Now_Page, STORY_TYPE Type)
{
	switch (Type)
	{
	case STORY_TYPE::INTRO_STAGE:
		Script_Stage_3(Now_Page);
		break;
	case STORY_TYPE::INTRO_BOSS:
		Script_Stage_3_BOSS(Now_Page);
		break;
	}
}

void Script_Stage_4_Draw(int Now_Page, STORY_TYPE Type)
{
	switch (Type)
	{
	case STORY_TYPE::INTRO_STAGE:
		Script_Stage_4(Now_Page);
		break;
	case STORY_TYPE::INTRO_BOSS:
		Script_Stage_4_BOSS(Now_Page);
		break;
	}
}

void Script_Stage_5_Draw(int Now_Page, STORY_TYPE Type)
{
	switch (Type)
	{
	case STORY_TYPE::INTRO_STAGE:
		Script_Stage_5(Now_Page);
		break;
	case STORY_TYPE::INTRO_BOSS:
		Script_Stage_5_BOSS_Phase_1(Now_Page);
		break;
	case STORY_TYPE::OUTRO_FINAL_BOSS_LIE: // Or any other specific type
		Script_Stage_5_BOSS_Phase_2(Now_Page);
		break;
	}
}

bool Is_Current_Script_Done()
{
	return Is_Page_Draw_Complete;
}

void Is_Page_Complete()
{
	Is_Page_Draw_Complete = false;
}

void Script_Stage_1(int Now_Page)
{
	int Stage_1 = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_1 = Texture_M->GetID("Stage_1_1");
		break;
	case 1:
		Stage_1 = Texture_M->GetID("Stage_1_2");
		break;
	case 2:
		Stage_1 = Texture_M->GetID("Stage_1_3");
		break;
	case 3:
		Stage_1 = Texture_M->GetID("Stage_1_4");
		break;
	case 4:
		Stage_1 = Texture_M->GetID("Stage_1_5");
		break;
	}

	Sprite_Draw(Stage_1, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_2(int Now_Page)
{
	int Stage_2 = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_2 = Texture_M->GetID("Stage_2_1");
		break;
	case 1:
		Stage_2 = Texture_M->GetID("Stage_2_2");
		break;
	case 2:
		Stage_2 = Texture_M->GetID("Stage_2_3");
		break;
	case 3:
		Stage_2 = Texture_M->GetID("Stage_2_4");
		break;
	case 4:
		Stage_2 = Texture_M->GetID("Stage_2_5");
		break;
	case 5:
		Stage_2 = Texture_M->GetID("Stage_2_6");
		break;
	case 6:
		Stage_2 = Texture_M->GetID("Stage_2_7");
		break;
	}

	Sprite_Draw(Stage_2, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_3(int Now_Page)
{
	int Stage_3 = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_3 = Texture_M->GetID("Stage_3_1");
		break;
	case 1:
		Stage_3 = Texture_M->GetID("Stage_3_2");
		break;
	case 2:
		Stage_3 = Texture_M->GetID("Stage_3_3");
		break;
	case 3:
		Stage_3 = Texture_M->GetID("Stage_3_4");
		break;
	case 4:
		Stage_3 = Texture_M->GetID("Stage_3_5");
		break;
	case 5:
		Stage_3 = Texture_M->GetID("Stage_3_6");
		break;
	}

	Sprite_Draw(Stage_3, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_3_BOSS(int Now_Page)
{
	int Stage_3_Boss = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_3_Boss = Texture_M->GetID("Stage_3_Boss_1");
		break;
	case 1:
		Stage_3_Boss = Texture_M->GetID("Stage_3_Boss_2");
		break;
	case 2:
		Stage_3_Boss = Texture_M->GetID("Stage_3_Boss_3");
		break;
	case 3:
		Stage_3_Boss = Texture_M->GetID("Stage_3_Boss_4");
		break;
	}

	Sprite_Draw(Stage_3_Boss, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_4(int Now_Page)
{
	int Stage_4 = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_4 = Texture_M->GetID("Stage_4_1");
		break;
	case 1:
		Stage_4 = Texture_M->GetID("Stage_4_2");
		break;
	case 2:
		Stage_4 = Texture_M->GetID("Stage_4_3");
		break;
	}

	Sprite_Draw(Stage_4, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_4_BOSS(int Now_Page)
{
	int Stage_4_Boss = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_4_Boss = Texture_M->GetID("Stage_4_Boss_1");
		break;
	case 1:
		Stage_4_Boss = Texture_M->GetID("Stage_4_Boss_2");
		break;
	case 2:
		Stage_4_Boss = Texture_M->GetID("Stage_4_Boss_3");
		break;
	case 3:
		Stage_4_Boss = Texture_M->GetID("Stage_4_Boss_4");
		break;
	case 4:
		Stage_4_Boss = Texture_M->GetID("Stage_4_Boss_5");
		break;
	}

	Sprite_Draw(Stage_4_Boss, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_5(int Now_Page)
{
	int Stage_5 = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_5 = Texture_M->GetID("Stage_5_1");
		break;
	case 1:
		Stage_5 = Texture_M->GetID("Stage_5_2");
		break;
	}

	Sprite_Draw(Stage_5, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_5_BOSS_Phase_1(int Now_Page)
{
	int Stage_5_Boss = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_5_Boss = Texture_M->GetID("Stage_5_Boss_1");
		break;
	case 1:
		Stage_5_Boss = Texture_M->GetID("Stage_5_Boss_2");
		break;
	case 2:
		Stage_5_Boss = Texture_M->GetID("Story_Monologue");
		break;
	case 3:
		Stage_5_Boss = Texture_M->GetID("Stage_5_Boss_3");
		break;
	case 4:
		Stage_5_Boss = Texture_M->GetID("Stage_5_Boss_4");
		break;
	}

	Sprite_Draw(Stage_5_Boss, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}

void Script_Stage_5_BOSS_Phase_2(int Now_Page)
{
	int Stage_Final_Boss = -1;

	switch (Now_Page)
	{
	case 0:
		Stage_Final_Boss = Texture_M->GetID("Stage_Final_1");
		break;
	case 1:
		Stage_Final_Boss = Texture_M->GetID("Story_Monologue");
		break;
	case 2:
		Stage_Final_Boss = Texture_M->GetID("Stage_Final_2");
		break;
	case 3:
		Stage_Final_Boss = Texture_M->GetID("Story_Monologue");
		break;
	case 4:
		Stage_Final_Boss = Texture_M->GetID("Stage_Final_3");
		break;
	case 5:
		Stage_Final_Boss = Texture_M->GetID("Stage_Final_4");
		break;
	}

	Sprite_Draw(Stage_Final_Boss, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);

	Is_Page_Draw_Complete = true;
}