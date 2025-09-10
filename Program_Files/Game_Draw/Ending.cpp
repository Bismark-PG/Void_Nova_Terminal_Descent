/*==============================================================================

	Game Ending Screen [Ending.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Ending.h"
#include "Game_Header_Manager.h"
#include "Palette.h"
#include "Story_Script.h"

using namespace DirectX;
using namespace PALETTE;

static ENDING_SEQUENCE Current_Sequence;
static double State_Timer = 0.0;
static float Current_Alpha = 0.0f;

static int Ending_BG_TexID = -1;
static float Ending_BG_Y1 = 0.0f;
static float Ending_BG_Y2 = 0.0f;

static bool Is_Player_Moving = false;
static XMFLOAT2 Player_Target_Pos;
static float Player_Current_Speed = 0.0f;
static float Player_Max_Speed = 0.0f;

static int Sample_Text_TexID_1 = -1;
static int Sample_Text_TexID_2 = -1;
static int Sample_Text_TexID_3 = -1;
static int Sample_Text_TexID_4 = -1;
static int Sample_Text_TexID_5 = -1;
static int Sample_Text_TexID_6 = -1;
static int Sample_Text_TexID_7 = -1;

static int Base_TexID = -1;
static XMFLOAT2 Base_Position;

static int Thanks_TexID = -1;

static float w;
static float h;
static float x;
static float y;

static float Ending_Commander_X;
static float Ending_Commander_Y;
static float Ending_Commander_W;
static float Ending_Commander_H;

void Ending_Initialize()
{
	Ending_BG_TexID = Texture_Load(L"Resource/Texture/BG/Game_BG_Ending.png");
	Base_TexID = Texture_Load(L"Resource/Texture/Enemy/Main.png");

	Sample_Text_TexID_1 = Texture_Load(L"Resource/Texture/Story/Script/Ending_1.png");
	Sample_Text_TexID_2 = Texture_Load(L"Resource/Texture/Story/Script/Ending_2.png");
	Sample_Text_TexID_3 = Texture_Load(L"Resource/Texture/Story/Script/Ending_3.png");
	Sample_Text_TexID_4 = Texture_Load(L"Resource/Texture/Story/Script/Ending_4.png");
	Sample_Text_TexID_5 = Texture_Load(L"Resource/Texture/Story/Script/Ending_5.png");
	Sample_Text_TexID_6 = Texture_Load(L"Resource/Texture/Story/Script/Ending_6.png");
	Sample_Text_TexID_7 = Texture_Load(L"Resource/Texture/Story/Script/Ending_Commander.png");

	Thanks_TexID = Texture_Load(L"Resource/Texture/Story/Script/Ending_7.png");

	Current_Sequence = ENDING_SEQUENCE::FADE_IN;
	State_Timer = 0.0;
	Current_Alpha = 0.0f;

	Ending_BG_Y1 = 0.0f;
	Ending_BG_Y2 = -static_cast<float>(SCREEN_HEIGHT);

	Player_Reset_For_Story();
	Player_Set_Position({
		Game_Offset.x + (Game_Screen_Width * 0.75f) - (Player_Width * 0.5f),
		static_cast<float>(SCREEN_HEIGHT)
		});
	Player_Max_Speed = 150.0f * Game_Scale;
	Is_Player_Moving = false;

	Base_Position = { Game_Offset.x, -static_cast<float>(SCREEN_HEIGHT) * 0.5f };

	SM->Stop_BGM();
	SM->Play_BGM("Ending");
	Fade_Start(3.0, false);

	w = WM.Get_Now_Screen_Mode() == Window_Mode ? 400.0f : 600.0f;
	h = WM.Get_Now_Screen_Mode() == Window_Mode ? 400.0f : 600.0f;
	x = WM.Get_Now_Screen_Mode() == Window_Mode ? 250.0f : 600.0f;
	y = WM.Get_Now_Screen_Mode() == Window_Mode ? 200.0f : 600.0f;
}

void Ending_Set_Position(float X, float Y, float W, float H)
{
	Ending_Commander_X = X;
	Ending_Commander_Y = Y;
	Ending_Commander_W = W;
	Ending_Commander_H = H;
}

void Ending_Finalize()
{
	Update_Main_Screen(Main_Screen::SELECT_GAME);
	Update_Sub_Screen(Sub_Screen::S_DONE);
	Update_Game_Select_Screen(Game_Select_Screen::GAME_MENU_SELECT);
	Update_Game_Select_Buffer(SELECT_GAME::SELECT_WAIT);

	Title_Initialize();
	Menu_Initialize();
	Game_Select_Initialize();
	Setting_Initialize();

	SM->Stop_BGM();
	SM->Play_BGM("Title");
}


void Ending_Update(double elapsed_time)
{
	Ending_BG_Y1 += 1.0f * Game_Scale;
	Ending_BG_Y2 += 1.0f * Game_Scale;
	if (Ending_BG_Y1 >= static_cast<float>(SCREEN_HEIGHT)) Ending_BG_Y1 = -static_cast<float>(SCREEN_HEIGHT);
	if (Ending_BG_Y2 >= static_cast<float>(SCREEN_HEIGHT)) Ending_BG_Y2 = -static_cast<float>(SCREEN_HEIGHT);

	if (Is_Player_Moving)
	{
		XMFLOAT2 currentPos = Player_Get_Position();
		XMVECTOR dir = XMVector2Normalize(XMVectorSet(Player_Target_Pos.x - currentPos.x, Player_Target_Pos.y - currentPos.y, 0, 0));

		float distance = XMVectorGetX(XMVector2Length(XMVectorSet(Player_Target_Pos.x - currentPos.x, Player_Target_Pos.y - currentPos.y, 0, 0)));
		if (Current_Sequence == ENDING_SEQUENCE::PLAYER_RISE)
		{
			Player_Current_Speed = Player_Max_Speed * (distance / (Game_Screen_Height * 0.4f));
			Player_Current_Speed = max(Player_Current_Speed, Player_Max_Speed * 0.1f);
		}
		else
		{
			Player_Current_Speed += 100.0f * Game_Scale * static_cast<float>(elapsed_time);
			if (Player_Current_Speed > Player_Max_Speed) Player_Current_Speed = Player_Max_Speed;
		}

		currentPos.x += XMVectorGetX(dir) * Player_Current_Speed * static_cast<float>(elapsed_time);
		currentPos.y += XMVectorGetY(dir) * Player_Current_Speed * static_cast<float>(elapsed_time);
		Player_Set_Position(currentPos);

		if (distance < 5.0f)
		{
			Player_Set_Position(Player_Target_Pos);
			Is_Player_Moving = false;
			Player_Current_Speed = 0.0f;
		}
	}

	State_Timer += elapsed_time;
	switch (Current_Sequence)
	{
	case ENDING_SEQUENCE::FADE_IN:
		if (Fade_GetState() == FADE_STATE::FINISHED_IN)
		{
			Is_Player_Moving = true;
			Player_Target_Pos = { Game_Offset.x + (Game_Screen_Width * 0.75f) - (Player_Width * 0.5f), Game_Screen_Height * 0.6f };
			Current_Sequence = ENDING_SEQUENCE::PLAYER_RISE;
		}
		break;

	case ENDING_SEQUENCE::PLAYER_RISE:
		if (!Is_Player_Moving)
		{
			State_Timer = 0.0;
			Current_Alpha = 0.0f;
			Current_Sequence = ENDING_SEQUENCE::SHOW_TEXT_1;
		}
		break;

	case ENDING_SEQUENCE::SHOW_TEXT_1:
	case ENDING_SEQUENCE::SHOW_TEXT_2:
	case ENDING_SEQUENCE::SHOW_TEXT_3:
	case ENDING_SEQUENCE::SHOW_TEXT_4:
	case ENDING_SEQUENCE::SHOW_TEXT_5:
	case ENDING_SEQUENCE::SHOW_TEXT_6:
		if (State_Timer < 2.0) Current_Alpha += (float)elapsed_time / 2.0f; // Fade in
		else if (State_Timer >= 8.0 && State_Timer < 10.0) Current_Alpha -= (float)elapsed_time / 2.0f; // Fade out

		Current_Alpha = max(0.0f, min(1.0f, Current_Alpha));

		if (State_Timer >= 10.0)
		{
			State_Timer = 0.0;
			Current_Alpha = 0.0f;
			if (Current_Sequence == ENDING_SEQUENCE::SHOW_TEXT_6)
			{
				Current_Sequence = ENDING_SEQUENCE::BASE_DESCEND;
			}
			else
			{
				Current_Sequence = static_cast<ENDING_SEQUENCE>(static_cast<int>(Current_Sequence) + 1);
			}
		}
		break;

	case ENDING_SEQUENCE::BASE_DESCEND:
		Base_Position.y += (Game_Screen_Height * 0.1f) * static_cast<float>(elapsed_time);
		if (State_Timer >= 5.0)
		{
			Is_Player_Moving = true;
			Player_Target_Pos = { Game_Offset.x + (Game_Screen_Width / 2.0f) - (Player_Width * 0.5f), Base_Position.y };
			Player_Max_Speed *= 2.0f;
			Current_Sequence = ENDING_SEQUENCE::PLAYER_APPROACH_BASE;
		}
		break;

	case ENDING_SEQUENCE::PLAYER_APPROACH_BASE:
		if (!Is_Player_Moving)
		{
			Player_Set_Enable(false);
			State_Timer = 0.0;
			Current_Sequence = ENDING_SEQUENCE::COMMANDER_APPEAR;
		}
		break;

	case ENDING_SEQUENCE::COMMANDER_APPEAR:
		if (State_Timer == 0.0)
			Ending_Commander_Start();

		Ending_Commander_Update(elapsed_time);

		if (State_Timer >= 3.0)
		{
			State_Timer = 0.0;
			Current_Sequence = ENDING_SEQUENCE::COMMANDER_MESSAGE;
		}
		break;

	case ENDING_SEQUENCE::COMMANDER_MESSAGE:
		if (State_Timer >= 3.0)
		{
			State_Timer = 0.0;
			Current_Sequence = ENDING_SEQUENCE::FINAL_FADE_OUT;
		}
		break;

	case ENDING_SEQUENCE::FINAL_FADE_OUT:
		if (State_Timer == 0.0) Fade_Start(5.0, true);
		if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
		{
			State_Timer = 0.0;
			Current_Alpha = 0.0f;
			Current_Sequence = ENDING_SEQUENCE::THANKS_MESSAGE;
		}
		break;

	case ENDING_SEQUENCE::THANKS_MESSAGE:
		if (SM->Get_BGM_Volume() > 0)
		{
			int newVolume = SM->Get_BGM_Volume() - 1;
			SM->Set_BGM_Volume(max(0, newVolume));
		}

		if (State_Timer < 2.0)
			Current_Alpha += (float)elapsed_time / 2.0f;
		else if (State_Timer >= 5.0 && State_Timer < 7.0)
			Current_Alpha -= (float)elapsed_time / 2.0f;

		Current_Alpha = max(0.0f, min(1.0f, Current_Alpha));

		if (State_Timer >= 10.0)
			Current_Sequence = ENDING_SEQUENCE::RESET_TO_MAIN;
		break;

	case ENDING_SEQUENCE::RESET_TO_MAIN:
		Ending_Finalize();
		Current_Sequence = ENDING_SEQUENCE::DONE;
		break;

	case ENDING_SEQUENCE::DONE:
		break;
	}
}


void Ending_Draw()
{
	Sprite_Draw(Ending_BG_TexID, Game_Offset.x, Ending_BG_Y1, static_cast<float>(Game_Screen_Width), static_cast<float>(SCREEN_HEIGHT), 0.0f);
	Sprite_Draw(Ending_BG_TexID, Game_Offset.x, Ending_BG_Y2, static_cast<float>(Game_Screen_Width), static_cast<float>(SCREEN_HEIGHT), 0.0f);

	Player_Draw();


	if (Current_Sequence >= ENDING_SEQUENCE::BASE_DESCEND)
	{
		float base_h = static_cast<float>(SCREEN_HEIGHT) * 0.5f;
		Sprite_Draw(Base_TexID, Base_Position.x, Base_Position.y, static_cast<float>(Game_Screen_Width), base_h, 0.0f);
	}



	switch (Current_Sequence)
	{
	case ENDING_SEQUENCE::SHOW_TEXT_1:
		Sprite_Draw(Sample_Text_TexID_1, x, y, w, h, 0.0f, { 1,1,1, Current_Alpha });
		break;
	case ENDING_SEQUENCE::SHOW_TEXT_2:
		Sprite_Draw(Sample_Text_TexID_2, x, y, w, h, 0.0f, { 1,1,1, Current_Alpha });
		break;
	case ENDING_SEQUENCE::SHOW_TEXT_3:
		Sprite_Draw(Sample_Text_TexID_3, x, y, w, h, 0.0f, { 1,1,1, Current_Alpha });
		break;
	case ENDING_SEQUENCE::SHOW_TEXT_4:
		Sprite_Draw(Sample_Text_TexID_4, x, y, w, h, 0.0f, { 1,1,1, Current_Alpha });
		break;
	case ENDING_SEQUENCE::SHOW_TEXT_5:
		Sprite_Draw(Sample_Text_TexID_5, x, y, w, h, 0.0f, { 1,1,1, Current_Alpha });
		break;
	case ENDING_SEQUENCE::SHOW_TEXT_6:
		Sprite_Draw(Sample_Text_TexID_6, x, y, w, h, 0.0f, { 1,1,1, Current_Alpha });
		break;
	}

	if (Current_Sequence >= ENDING_SEQUENCE::COMMANDER_MESSAGE && Current_Sequence < ENDING_SEQUENCE::FINAL_FADE_OUT)
	{
		Ending_Commander_Draw();
		if (State_Timer > 3.0)
			Sprite_Draw(Sample_Text_TexID_7, Ending_Commander_X, Ending_Commander_Y, Ending_Commander_W, Ending_Commander_H, 0.0f);
	}

	if (Current_Sequence == ENDING_SEQUENCE::THANKS_MESSAGE)
	{
		float w = SCREEN_WIDTH * 0.7f;
		float h = w * 0.2f;
		float x = (SCREEN_WIDTH - w) / 2.0f;
		float y = (SCREEN_HEIGHT - h) / 2.0f;
		Sprite_Draw(Thanks_TexID, x, y, w, h, 0.0f, { 1,1,1, Current_Alpha });
	}
}
