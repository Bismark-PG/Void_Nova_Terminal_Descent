/*==============================================================================

	Game Ending Screen [Ending.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Ending.h"
#include "Game_Header_Manager.h"
#include "Palette.h"
#include "Story_Script.h"
#include "System_Logic_Manager.h"
#include "Update_Screen.h"
#include "Game_Select.h"
#include "Title.h"
#include "Game_Menu.h"
#include "Setting.h"

using namespace DirectX;
using namespace PALETTE;

static ENDING_SEQUENCE Current_Sequence;
static double State_Timer = 0.0;

static int Ending_BG_TexID = -1;
static float Ending_BG_Y1 = 0.0f;
static float Ending_BG_Y2 = 0.0f;

static float Credit_Base_Y = 0.0f;
static float Text_Space = 0.0f;
static const float SCROLL_SPEED = 75.0f;
static const float TEXT_SPACING = 25.0f;

static int Anime_Player = -1;
static int Play_Player = -1;
static XMFLOAT2 Ending_Player_Pos;
static XMFLOAT2 Ending_Player_Target_Pos;
static float Ending_Player_W = PLAYER_WIDTH  * A_Double;
static float Ending_Player_H = PLAYER_HEIGHT * A_Double;
static float Ending_Player_Speed = 0.0f;
static float Ending_Player_Alpha = 1.0f;
static bool Is_Ending_Player_Moving = false;

static int Ending_1_TexID = -1;
static int Ending_2_TexID = -1;
static int Ending_3_TexID = -1;
static int Ending_4_TexID = -1;
static int Ending_5_TexID = -1;
static int Ending_6_TexID = -1;
static int Ending_7_TexID = -1;
static int Ending_8_TexID = -1;
static int Ending_9_TexID = -1;
static int Ending_10_TexID = -1;
static int Ending_Commender_TexID = -1;
static int Thanks_TexID = -1;

static int Base_TexID = -1;
static XMFLOAT2 Base_Position;
static float Base_Target_Y = 0.0f;
static float Base_Width = 0.0f;
static float Base_Height = 0.0f;
static float Base_Speed = 50.0f;

static float W;
static float H;
static float X;

static float Ending_X;
static float Ending_Y;
static float Ending_W;
static float Ending_H;
static bool Is_Ending_Text_Active = false;

static float BGM_Volume = 0.0f;
static double BGM_Timer = 0.0f;
static constexpr float FADE_OUT_TIMER = 5.0f;
static bool Is_Fade_Active = false;

void Ending_Initialize()
{
	Ending_BG_TexID = Texture_M->GetID("BG_Ending");
	Base_TexID = Texture_M->GetID("Ending_Base");
	Ending_1_TexID = Texture_M->GetID("Ending_Text_1");
	Ending_2_TexID = Texture_M->GetID("Ending_Text_2");
	Ending_3_TexID = Texture_M->GetID("Ending_Text_3");
	Ending_4_TexID = Texture_M->GetID("Ending_Text_4");
	Ending_5_TexID = Texture_M->GetID("Ending_Text_5");
	Ending_6_TexID = Texture_M->GetID("Ending_Text_6");
	Ending_7_TexID = Texture_M->GetID("Ending_Text_7");
	Ending_8_TexID = Texture_M->GetID("Ending_Text_8");
	Ending_9_TexID = Texture_M->GetID("Ending_Text_9");
	Ending_10_TexID = Texture_M->GetID("Ending_Text_10");
	Ending_Commender_TexID = Texture_M->GetID("Ending_Text_Commander");
	Thanks_TexID = Texture_M->GetID("Ending_Text_Thanks");

	int Straight = Texture_M->GetID("Player_Ending_BG");
	Anime_Player = SpriteAni_Get_Pattern_Info(Straight, 16, 4, 0.25, { 1000, 800 }, { 1000 * 0, 800 * 0 }, true);
	Play_Player = SpriteAni_CreatePlayer(Anime_Player);

	Current_Sequence = ENDING_SEQUENCE::IDLE;
	State_Timer = 0.0;

	Ending_BG_Y1 = 0.0f;
	Ending_BG_Y2 = -static_cast<float>(SCREEN_HEIGHT);

	Ending_Player_Pos = { static_cast<float>(SCREEN_WIDTH) * 0.75f, static_cast<float>(SCREEN_HEIGHT) };
	Is_Ending_Player_Moving = false;
	Is_Ending_Text_Active = false;
	Is_Fade_Active = false;
	Ending_Player_Alpha = 1.0f;

	Base_Width  = static_cast<float>(SCREEN_WIDTH) * 1.2f;
	Base_Height = static_cast<float>(SCREEN_HEIGHT) * 0.5f;
	Base_Position = { (static_cast<float>(SCREEN_WIDTH) * 0.5f) - (Base_Width * 0.5f), -Base_Height};
	Base_Target_Y = -Game_Screen_Height * 0.1f;
	Base_Speed *= Game_Scale;

	W = Game_Screen_Width * 0.6f;
	H = W;
	X = Game_Offset.x + (Game_Screen_Width * 0.5f) - (W * 0.5f);
	Credit_Base_Y = static_cast<float>(SCREEN_HEIGHT);
	Text_Space = TEXT_SPACING * Game_Scale;

	BGM_Timer = 0.0f;
	BGM_Volume = 0.0f;

	Script_Reset();
}

void Ending_Finalize()
{
	System_Reset_For_Ending();
}

void Update_Ending_Player_Movement(double elapsed_time)
{
	if (!Is_Ending_Player_Moving) return;

	XMVECTOR currentVec = XMLoadFloat2(&Ending_Player_Pos);
	XMVECTOR targetVec = XMLoadFloat2(&Ending_Player_Target_Pos);
	XMVECTOR toTarget = targetVec - currentVec;
	float distance = XMVectorGetX(XMVector2Length(toTarget));
	float moveAmount = Ending_Player_Speed * static_cast<float>(elapsed_time);

	if (distance > moveAmount)
	{
		XMVECTOR direction = XMVector2Normalize(toTarget);
		currentVec += direction * moveAmount;
		XMStoreFloat2(&Ending_Player_Pos, currentVec);
	}
	else
	{
		Ending_Player_Pos = Ending_Player_Target_Pos;
		Is_Ending_Player_Moving = false;
	}
}


void Ending_Update(double elapsed_time)
{
	Ending_BG_Y1 += 1.0f * Game_Scale;
	Ending_BG_Y2 += 1.0f * Game_Scale;

	if (Ending_BG_Y1 >= static_cast<float>(SCREEN_HEIGHT)) Ending_BG_Y1 = -static_cast<float>(SCREEN_HEIGHT);
	if (Ending_BG_Y2 >= static_cast<float>(SCREEN_HEIGHT)) Ending_BG_Y2 = -static_cast<float>(SCREEN_HEIGHT);

	Update_Ending_Player_Movement(elapsed_time);

	if (Current_Sequence == ENDING_SEQUENCE::COMMANDER_APPEAR)
	{
		Ending_Commander_Update(elapsed_time);
	}

	State_Timer += elapsed_time;

	switch (Current_Sequence)
	{
	case ENDING_SEQUENCE::IDLE:
		break;

	case ENDING_SEQUENCE::ENDING_START:
		Sound_M->Stop_BGM();
		Sound_M->Play_BGM("Ending");
		Fade_Start(3.0, false);
		Set_Ending_Status(ENDING_SEQUENCE::FADE_IN);
		break;

	case ENDING_SEQUENCE::FADE_IN:
		if (Fade_GetState() == FADE_STATE::FINISHED_IN)
		{

			Ending_Player_Target_Pos = { Ending_Player_Pos.x, Game_Screen_Height * A_T_Quarters };
			Ending_Player_Speed = 75.0f * Game_Scale;
			Is_Ending_Player_Moving = true;
			State_Timer = 0.0;

			Set_Ending_Status(ENDING_SEQUENCE::PLAYER_RISE);
		}
		break;

	case ENDING_SEQUENCE::PLAYER_RISE:
		if (!Is_Ending_Player_Moving)
		{
			State_Timer = 0.0;

			Set_Ending_Status(ENDING_SEQUENCE::CREDIT_ROLL);
		}
		break;

	case ENDING_SEQUENCE::CREDIT_ROLL:
	{
		Credit_Base_Y -= SCROLL_SPEED * Game_Scale * static_cast<float>(elapsed_time);

		float last_text_y = Credit_Base_Y + 10 * (H + Text_Space);
		if (last_text_y < -H)
		{
			State_Timer = 0.0;

			Set_Ending_Status(ENDING_SEQUENCE::BASE_APPEAR);
		}
		break;
	}

	case ENDING_SEQUENCE::BASE_APPEAR:
	{
		float Amount = Base_Speed * static_cast<float>(elapsed_time);
		if (Base_Position.y < Base_Target_Y)
		{
			Base_Position.y += Amount;
			if (Base_Position.y > Base_Target_Y)
			{
				Base_Position.y = Base_Target_Y;
			}
		}

		if (Base_Position.y == Base_Target_Y)
		{
			Ending_Player_Target_Pos = { Ending_Player_Pos.x, -Ending_Player_H * 1.5f };
			Ending_Player_Speed = 80.0f * Game_Scale;
			Is_Ending_Player_Moving = true;
			State_Timer = 0.0;

			Set_Ending_Status(ENDING_SEQUENCE::PLAYER_MOVE_TO_BASE);
		}
		break;
	}

	case ENDING_SEQUENCE::PLAYER_MOVE_TO_BASE:
		if (!Is_Ending_Player_Moving)
		{
			State_Timer = 0.0;

			Set_Ending_Status(ENDING_SEQUENCE::COMMANDER_ANIM_START);
		}
		break;

	case ENDING_SEQUENCE::COMMANDER_ANIM_START:
		Ending_Commander_Start();
		State_Timer = 0.0;

		Set_Ending_Status(ENDING_SEQUENCE::COMMANDER_APPEAR);
		break;


	case ENDING_SEQUENCE::COMMANDER_APPEAR:
		if (State_Timer > 1.0)
		{
			State_Timer = 0.0;

			Set_Ending_Status(ENDING_SEQUENCE::COMMANDER_BG_WAIT);
		}
		break;

	case ENDING_SEQUENCE::COMMANDER_BG_WAIT:
		if (State_Timer > 1.0)
		{
			State_Timer = 0.0;
			Is_Ending_Text_Active = true;

			Set_Ending_Status(ENDING_SEQUENCE::COMMANDER_MESSAGE);
		}
		break;

	case ENDING_SEQUENCE::COMMANDER_MESSAGE:
		if (State_Timer >= 3.0)
		{
			State_Timer = 0.0;
			BGM_Timer = 0.0f;

			Set_Ending_Status(ENDING_SEQUENCE::FINAL_FADE_OUT);
		}
		break;

	case ENDING_SEQUENCE::FINAL_FADE_OUT:
	{
		BGM_Timer += elapsed_time;

		if (!Is_Fade_Active)
		{
			Fade_Start(5.0, true);
			Is_Fade_Active = true;
		}

		if (BGM_Timer > FADE_OUT_TIMER)
		{
			Sound_M->Stop_BGM();
			Sound_M->Update_Current_BGM_Volume(Sound_M->Get_Target_BGM_Volume());

			State_Timer = 0.0;

			Set_Ending_Status(ENDING_SEQUENCE::RESET_TO_MAIN);
		}

		float Target_Volume = Sound_M->Get_Target_BGM_Volume();
		BGM_Volume = Target_Volume * (1.0f - static_cast<float>(BGM_Timer / FADE_OUT_TIMER));

		if (BGM_Volume < 0.0f)
			BGM_Volume = 0.0f;

		Sound_M->Update_Current_BGM_Volume(BGM_Volume);
	}
		break;

	case ENDING_SEQUENCE::RESET_TO_MAIN:
	{
		float User_Set_Volume = Sound_M->Get_Target_BGM_Volume();
		Sound_M->Update_Current_BGM_Volume(User_Set_Volume);
		Ending_Finalize();

		if (Fade_GetState() == FADE_STATE::FINISHED_OUT)
			Set_Ending_Status(ENDING_SEQUENCE::DONE);
	}
		break;

	case ENDING_SEQUENCE::DONE:
		break;
	}
}


void Ending_Draw()
{
	Sprite_Draw(Ending_BG_TexID, 0.0f, Ending_BG_Y1, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f);
	Sprite_Draw(Ending_BG_TexID, 0.0f, Ending_BG_Y2, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f);

	if (Current_Sequence >= ENDING_SEQUENCE::PLAYER_RISE && Current_Sequence < ENDING_SEQUENCE::COMMANDER_ANIM_START) 
	{
		SpriteAni_Draw(Play_Player, Ending_Player_Pos.x, Ending_Player_Pos.y, Ending_Player_W, Ending_Player_H, 30.0f,
			{ A_Origin, A_Origin, A_Origin, Ending_Player_Alpha });
	}

	if (Current_Sequence >= ENDING_SEQUENCE::BASE_APPEAR)
	{
		Sprite_Draw(Base_TexID, Base_Position.x, Base_Position.y, Base_Width, Base_Height, 0.0f);
	}			

	switch (Current_Sequence)
	{
	case ENDING_SEQUENCE::CREDIT_ROLL:
		Sprite_Draw(Ending_1_TexID, X, Credit_Base_Y + 0 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_2_TexID, X, Credit_Base_Y + 1 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_3_TexID, X, Credit_Base_Y + 2 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_4_TexID, X, Credit_Base_Y + 3 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_5_TexID, X, Credit_Base_Y + 4 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_6_TexID, X, Credit_Base_Y + 5 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_7_TexID, X, Credit_Base_Y + 6 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_8_TexID, X, Credit_Base_Y + 7 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_9_TexID, X, Credit_Base_Y + 8 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Ending_10_TexID, X, Credit_Base_Y + 9 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		Sprite_Draw(Thanks_TexID, X, Credit_Base_Y + 10 * (H + Text_Space), W, H, 0.0f, { 1,1,1, 1.0f });
		break;

	case ENDING_SEQUENCE::COMMANDER_APPEAR:
	case ENDING_SEQUENCE::COMMANDER_BG_WAIT:
	case ENDING_SEQUENCE::COMMANDER_MESSAGE:
	case ENDING_SEQUENCE::FINAL_FADE_OUT:
		Ending_Commander_Draw();

		if (Is_Ending_Text_Active)
			Sprite_Draw(Ending_Commender_TexID, Ending_X, Ending_Y, Ending_W, Ending_H);

		if (Current_Sequence == ENDING_SEQUENCE::COMMANDER_MESSAGE)
			Is_Ending_Text_Active = true;
		break;
	}
}

void Ending_Get_Text_POS(float x, float y, float w, float h)
{
	Ending_X = x;
	Ending_Y = y;
	Ending_W = w;
	Ending_H = h;
}

ENDING_SEQUENCE Get_Ending_Status()
{
	return Current_Sequence;
}

void Set_Ending_Status(ENDING_SEQUENCE Status)
{
	Current_Sequence = Status;
}