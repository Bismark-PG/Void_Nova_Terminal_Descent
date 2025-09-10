/*==============================================================================

	Script Draw For Stage One [Script_Draw.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Script_Draw.h"
#include "Sprite_Animation.h"
#include "Game_Window.h"
#include "Palette.h"
using namespace PALETTE;

static int Story_Stage_1_1 = -1;
static int Story_Stage_1_2 = -1;
static int Story_Stage_1_3 = -1;
static int Story_Stage_1_4 = -1;
static int Story_Stage_1_5 = -1;

static int Story_Stage_2_1 = -1;
static int Story_Stage_2_2 = -1;
static int Story_Stage_2_3 = -1;
static int Story_Stage_2_4 = -1;
static int Story_Stage_2_5 = -1;
static int Story_Stage_2_6 = -1;
static int Story_Stage_2_7 = -1;

static int Story_Stage_3_1 = -1;
static int Story_Stage_3_2 = -1;
static int Story_Stage_3_3 = -1;
static int Story_Stage_3_4 = -1;
static int Story_Stage_3_5 = -1;
static int Story_Stage_3_6 = -1;

static int Story_Stage_3_Boss_1 = -1;
static int Story_Stage_3_Boss_2 = -1;
static int Story_Stage_3_Boss_3 = -1;
static int Story_Stage_3_Boss_4 = -1;

static int Story_Stage_4_1 = -1;
static int Story_Stage_4_2 = -1;
static int Story_Stage_4_3 = -1;

static int Story_Stage_4_Boss_1 = -1;
static int Story_Stage_4_Boss_2 = -1;
static int Story_Stage_4_Boss_3 = -1;
static int Story_Stage_4_Boss_4 = -1;
static int Story_Stage_4_Boss_5 = -1;

static int Story_Stage_5_1 = -1;
static int Story_Stage_5_2 = -1;

static int Story_Stage_5_Boss_1 = -1;
static int Story_Stage_5_Boss_2 = -1;
static int Story_Stage_5_Boss_3 = -1;
static int Story_Stage_5_Boss_4 = -1;
static int Story_Stage_5_Boss_5 = -1;

static float X;
static float Y;

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

	Story_Stage_1_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_1_1.png");
	Story_Stage_1_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_1_2.png");
	Story_Stage_1_3 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_1_3.png");
	Story_Stage_1_4 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_1_4.png");
	Story_Stage_1_5 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_1_5.png");

	Story_Stage_2_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_2_1.png");
	Story_Stage_2_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_2_2.png");
	Story_Stage_2_3 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_2_3.png");
	Story_Stage_2_4 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_2_4.png");
	Story_Stage_2_5 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_2_5.png");
	Story_Stage_2_6 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_2_6.png");
	Story_Stage_2_7 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_2_7.png");

	Story_Stage_3_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_1.png");
	Story_Stage_3_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_2.png");
	Story_Stage_3_3 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_3.png");
	Story_Stage_3_4 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_4.png");
	Story_Stage_3_5 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_5.png");
	Story_Stage_3_6 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_6.png");

	Story_Stage_3_Boss_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_Boss_1.png");
	Story_Stage_3_Boss_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_Boss_2.png");
	Story_Stage_3_Boss_3 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_Boss_3.png");
	Story_Stage_3_Boss_4 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_3_Boss_4.png");

	Story_Stage_4_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_1.png");
	Story_Stage_4_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_2.png");
	Story_Stage_4_3 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_3.png");

	Story_Stage_4_Boss_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_Boss_1.png");
	Story_Stage_4_Boss_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_Boss_2.png");
	Story_Stage_4_Boss_3 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_Boss_3.png");
	Story_Stage_4_Boss_4 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_Boss_4.png");
	Story_Stage_4_Boss_5 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_4_Boss_5.png");

	Story_Stage_5_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_5_1.png");
	Story_Stage_5_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_5_2.png");

	Story_Stage_5_Boss_1 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_5_Boss_1.png");
	Story_Stage_5_Boss_2 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_5_Boss_2.png");
	Story_Stage_5_Boss_3 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_5_Boss_3.png");
	Story_Stage_5_Boss_4 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_5_Boss_4.png");
	Story_Stage_5_Boss_5 = Texture_Load(L"Resource/Texture/Story/Script/Story_Stage_5_Boss_5.png");

	X = SCREEN_WIDTH  * A_Half;
	Y = SCREEN_HEIGHT * A_Half;
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
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_1_1, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_1_2, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 2:
		Sprite_Draw(Story_Stage_1_3, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 3:
		Sprite_Draw(Story_Stage_1_4, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 4:
		Sprite_Draw(Story_Stage_1_5, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_2(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_2_1, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_2_2, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 2:
		Sprite_Draw(Story_Stage_2_3, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 3:
		Sprite_Draw(Story_Stage_2_4, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 4:
		Sprite_Draw(Story_Stage_2_5, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 5:
		Sprite_Draw(Story_Stage_2_6, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 6:
		Sprite_Draw(Story_Stage_2_7, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_3(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_3_1, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_3_2, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 2:
		Sprite_Draw(Story_Stage_3_3, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 3:
		Sprite_Draw(Story_Stage_3_4, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 4:
		Sprite_Draw(Story_Stage_3_5, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 5:
		Sprite_Draw(Story_Stage_3_6, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_3_BOSS(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_3_Boss_1, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_3_Boss_2, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 2:
		Sprite_Draw(Story_Stage_3_Boss_3, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 3:
		Sprite_Draw(Story_Stage_3_Boss_4, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_4(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_4_1, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_4_2, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 2:
		Sprite_Draw(Story_Stage_4_3, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_4_BOSS(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_4_Boss_1, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_4_Boss_2, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 2:
		Sprite_Draw(Story_Stage_4_Boss_3, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 3:
		Sprite_Draw(Story_Stage_4_Boss_4, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 4:
		Sprite_Draw(Story_Stage_4_Boss_5, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_5(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_5_1, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_5_2, Draw_X, Draw_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_5_BOSS_Phase_1(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		Sprite_Draw(Story_Stage_5_Boss_1, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 1:
		Sprite_Draw(Story_Stage_5_Boss_2, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 2:
		Sprite_Draw(Story_Stage_5_Boss_3, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 3:
		Sprite_Draw(Story_Stage_5_Boss_4, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	case 4:
		Sprite_Draw(Story_Stage_5_Boss_5, Draw_X, Draw_Enemy_Y, Draw_Width, Draw_Height, 0.f);
		break;
	}

	Is_Page_Draw_Complete = true;
}

void Script_Stage_5_BOSS_Phase_2(int Now_Page)
{
	switch (Now_Page)
	{
	case 0:
		break;
	}

	Is_Page_Draw_Complete = true;
}