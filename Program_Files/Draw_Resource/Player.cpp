/*==============================================================================

	Player [Player.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Player.h"
#include "Effect.h"
#include "Palette.h"
#include "system_timer.h"
#include "Stage_Select.h"
#include "Audio_Manager.h"
#include "Item.h"

using namespace DirectX;
using namespace PALETTE;

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849

static XMFLOAT2 PlayerPosition{};
static XMFLOAT2 PlayerVelocity{};
static XMFLOAT2 PlayerSize{};

static int Bomb = -1;
static int Avoid = -1;

static int Straight_Player_TexID = -1;
static int Left_Rotate_Player_TexID = -1;
static int Right_Rotate_Player_TexID = -1;

static int Anime_Straight = -1;
static int Anime_Left = -1;
static int Anime_Right = -1;

static int Play_Straight = -1;
static int Play_Left = -1;
static int Play_Right = -1;

static Circle PlayerCollision{};
static bool Player_Enable = false;
static bool Player_Avoid = false;
static bool Player_Used_Bomb = false;

PLAYER_STATE P_State;

static float Player_Alpha = PLAYER_ALPHA_ORIGINAL;
static double Bullet_Cooldown_Timer = 0.0;
static double Laser_Cooldown_Timer = 0.0;
static double Boom_Cooldown_Timer = 0.0;

void Player_Initialize()
{
	Avoid = Texture_Load(L"Resource/Texture/Player/Player_Avoid_Collision.png");

	Straight_Player_TexID	  = Texture_Load(L"Resource/Texture/Player/Player_Mk_3_Fixed_Animation_Fixed.png");
	Left_Rotate_Player_TexID  = Texture_Load(L"Resource/Texture/Player/Player_Mk_3_Rotate_L_Animation_Fixed.png");
	Right_Rotate_Player_TexID = Texture_Load(L"Resource/Texture/Player/Player_Mk_3_Rotate_R_Animation_Fixed.png");
	
	Anime_Straight = SpriteAni_Get_Pattern_Info(Straight_Player_TexID,	   4, 4, 0.25, { 838, 1019 }, { 838 * 0, 1019 * 0 }, true);
	Anime_Left	   = SpriteAni_Get_Pattern_Info(Left_Rotate_Player_TexID,  4, 4, 0.25, { 839, 1019 }, { 838 * 0, 1019 * 0 }, true);
	Anime_Right	   = SpriteAni_Get_Pattern_Info(Right_Rotate_Player_TexID, 4, 4, 0.25, { 839, 1019 }, { 838 * 0, 1019 * 0 }, true);
	
	Play_Straight = SpriteAni_CreatePlayer(Anime_Straight);
	Play_Left	  = SpriteAni_CreatePlayer(Anime_Left);
	Play_Right	  = SpriteAni_CreatePlayer(Anime_Right);

	Player_Width  = PLAYER_WIDTH  * Game_Scale;
	Player_Height = PLAYER_HEIGHT * Game_Scale;
	Player_Radius = ((Player_Width * A_Half) + (PLAYER_HEIGHT * A_Half)) * A_One_Tenth;
	Player_Item_Radius = (Player_Width + PLAYER_HEIGHT) * A_Quarter;
	Player_Avoid_Radius = Player_Radius * A_T_Quarters;

	Player_Speed_Original   = PLAYER_SPEED_ORIGINAL  * Game_Scale;
	Player_Speed_Avoid_Mode = PLAYER_SPEED_AVOIDANCE * Game_Scale;

	Player_Speed = Player_Speed_Original;

	PlayerPosition = { 0.0f, 0.0f};
	PlayerSize = { Player_Width, Player_Height };
	PlayerCollision = { { Player_Width * A_Half, Player_Height * A_Half }, Player_Radius };
	Player_Enable = false;
	PlayerVelocity = { 0.0f, 0.0f };
	Player_Alpha = PLAYER_ALPHA_ORIGINAL;

	Laser_Width_Half  = LASER_WIDTH  * A_Half;
	Bullet_Width_Half = BULLET_WIDTH * A_Half;

	P_State = PLAYER_STATE::NONE;
}

void Player_Finalize()
{
	Player_Width  = 0.0f;
	Player_Height = 0.0f;
	Player_Radius = 0.0f;

	Player_Speed = 0.0f;
	Player_Speed_Original = 0.0f;
	Player_Speed_Avoid_Mode = 0.0f;

	Laser_Width_Half = 0;
	Bullet_Width_Half = 0;
}

void Player_Update(double elapsed_time)
{
	if (!Player_Enable) return;

	if (Stage_Is_Player_Moving_By_Script()) return;

	if (Bullet_Cooldown_Timer > 0.0)
		Bullet_Cooldown_Timer -= elapsed_time;

	if (Laser_Cooldown_Timer > 0.0)
		Laser_Cooldown_Timer -= elapsed_time;

	if (Boom_Cooldown_Timer > 0.0)
		Boom_Cooldown_Timer -= elapsed_time;

#if defined(DEBUG) || defined(_DEBUG)
	if (KeyLogger_IsTrigger(KK_P)) { Status_Add_Power(0.1f); }
	if (KeyLogger_IsTrigger(KK_O)) { Status_Add_Power(-0.1f); }
	if (KeyLogger_IsTrigger(KK_L)) { Status_Add_Bomb(1); }
#endif	

	XMVECTOR Position = XMLoadFloat2(&PlayerPosition);
	XMVECTOR Velocity = XMLoadFloat2(&PlayerVelocity);

	XMVECTOR Direction{};
	bool L_Pressed = KeyLogger_IsPressed(KK_A) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_LEFT);
	bool R_Pressed = KeyLogger_IsPressed(KK_D) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_RIGHT);

	if (KeyLogger_IsPressed(KK_W) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_UP))
		Direction += { 0.f, -1.f };
	if (KeyLogger_IsPressed(KK_S) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_DPAD_DOWN))
		Direction += { 0.f, 1.f };

	if (L_Pressed && !R_Pressed)
	{
		Direction += { -1.f, 0.f };
		P_State = PLAYER_STATE::LEFT;
	}
	else if (R_Pressed && !L_Pressed)
	{
		Direction += { 1.f, 0.f };
		P_State = PLAYER_STATE::RIGHT;
	}
	else
		P_State = PLAYER_STATE::NONE;

	XMFLOAT2 Stick = XKeyLogger_GetLeftStick();
	float Stick_Magnitude = sqrtf(Stick.x * Stick.x + Stick.y * Stick.y); // Use Pythagorean Theorem

	if (Stick_Magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		Direction += XMVector2Normalize(XMVectorSet(Stick.x, -Stick.y, 0.0f, 0.0f));

		if (Stick.x < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			P_State = PLAYER_STATE::LEFT;
		else if (Stick.x > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			P_State = PLAYER_STATE::RIGHT;
	}

	Direction = XMVector2Normalize(Direction);

	Velocity += Direction * Player_Speed * static_cast<float>(elapsed_time);
	Position += Velocity;
	Velocity *= A_Half;

	XMStoreFloat2(&PlayerPosition, Position);
	XMStoreFloat2(&PlayerVelocity, Velocity);

	if (PlayerPosition.x <= Game_Offset.x)
		PlayerPosition.x = Game_Offset.x;
	if (PlayerPosition.x >= (Game_Offset.x + Game_Screen_Width) - PlayerSize.x)
		PlayerPosition.x = (Game_Offset.x + Game_Screen_Width) - PlayerSize.x;
	if (PlayerPosition.y <= 0)
		PlayerPosition.y = 0;
	if (PlayerPosition.y >= Game_Screen_Height - PlayerSize.y)
		PlayerPosition.y = Game_Screen_Height - PlayerSize.y;

	if (KeyLogger_IsPressed(KK_LEFTSHIFT) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		Player_Avoid = true;
		Player_Speed = Player_Speed_Avoid_Mode;
		PlayerCollision.Radius = Player_Avoid_Radius;
		Player_Item_Radius = Player_Avoid_Radius;
	}
	else
	{
		Player_Avoid = false;
		Player_Speed = Player_Speed_Original;
		PlayerCollision.Radius = Player_Radius;
		Player_Item_Radius = (Player_Width + PLAYER_HEIGHT) * A_Quarter;
	}

	if (KeyLogger_IsPressed(KK_SPACE) || XKeyLogger_IsPadPressed(XINPUT_GAMEPAD_A))
	{
		if (Bullet_Cooldown_Timer <= 0.0)
		{
			float Current_Power = Status_Get_Power();
			int Power_Level = static_cast<int>(Current_Power);

			float Bullet_Center_X		  = PlayerPosition.x + (Player_Width * A_Half);
			float Bullet_Y				  = PlayerPosition.y + (Player_Height * A_One_Tenth) - (BULLET_HEIGHT * A_Half);
			float Bullet_Pow_2_Y		  = PlayerPosition.y + (Player_Height * A_Half) - (BULLET_HEIGHT * A_Half);
			float Bullet_Pow_3_Middle_Y	  = PlayerPosition.y + (Player_Height * A_Third) - (BULLET_HEIGHT * A_Half);
			float Bullet_Pow_3_MAX_Side_Y = PlayerPosition.y + (Player_Height * A_Six_Tenths) - (BULLET_HEIGHT * A_Half);
			float Bullet_Pow_MAX_Y		  = PlayerPosition.y + (Player_Height * A_Half) - (BULLET_HEIGHT * A_Half);

			switch (Power_Level)
			{
			case 0:
				Bullet_Create({ Bullet_Center_X - Bullet_Width_Half, Bullet_Y });
				Bullet_Cooldown_Timer = POWER_MIN_FIRE_INTERVAL;
				break;

			case 1:
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_2_BULLET_L) - Bullet_Width_Half, Bullet_Pow_2_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_2_BULLET_R) - Bullet_Width_Half, Bullet_Pow_2_Y });
				Bullet_Cooldown_Timer = POWER_2_FIRE_INTERVAL;
				break;

			case 2:
				Bullet_Create({ Bullet_Center_X - Bullet_Width_Half, Bullet_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_2_BULLET_L) - Bullet_Width_Half, Bullet_Pow_2_Y});
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_2_BULLET_R) - Bullet_Width_Half, Bullet_Pow_2_Y});
				Bullet_Cooldown_Timer = POWER_3_FIRE_INTERVAL;
				break;

			case 3:
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_4_BULLET_L_1) - Bullet_Width_Half, Bullet_Pow_3_MAX_Side_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_4_BULLET_L_2) - Bullet_Width_Half, Bullet_Pow_3_Middle_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_4_BULLET_R_1) - Bullet_Width_Half, Bullet_Pow_3_Middle_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_4_BULLET_R_2) - Bullet_Width_Half, Bullet_Pow_3_MAX_Side_Y });
				Bullet_Cooldown_Timer = POWER_4_FIRE_INTERVAL;
				break;

			case 4:
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_MAX_BULLET_L_1) - Bullet_Width_Half, Bullet_Pow_3_MAX_Side_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_MAX_BULLET_L_2) - Bullet_Width_Half, Bullet_Pow_MAX_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_MAX_BULLET_R_1) - Bullet_Width_Half, Bullet_Pow_MAX_Y });
				Bullet_Create({ PlayerPosition.x + (Player_Width * POWER_MAX_BULLET_R_2) - Bullet_Width_Half, Bullet_Pow_3_MAX_Side_Y });
				Bullet_Cooldown_Timer = POWER_MAX_BULLET_FIRE_INTERVAL;
				break;
			}
			SM->Play_SFX("Bullet_Shoot");
		}

		if (static_cast<int>(Status_Get_Power()) == 4 && Laser_Cooldown_Timer <= 0.0)
		{
			float Bullet_Center_X = PlayerPosition.x + (Player_Width * A_Half);
			float Bullet_Y = PlayerPosition.y + (Player_Height * A_One_Tenth) - (BULLET_HEIGHT * A_Half);

			Laser_Create({ Bullet_Center_X - Laser_Width_Half, Bullet_Y });
			Laser_Cooldown_Timer = POWER_MAX_LASER_FIRE_INTERVAL;
		}
	}

	if (KeyLogger_IsTrigger(KK_R) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_B)) {
		Player_Used_Bomb = true;
	}

	if (Player_Used_Bomb)
	{
		if (Boom_Cooldown_Timer <= 0.0)
		{
			if (Status_Use_Bomb())
			{
				SM->Play_SFX("Player_Bomb");

				Game_UI_Start_Bomb_Effect();

				Bullet_Destroy_All_Enemy_Bullets();
				Enemy_Destroy_All_Normal_Enemies();

				SM->Play_SFX("Enemy_Dead_Bomb");

				Boom_Cooldown_Timer = BOMB_COOLDOWN_TIME;
			}
		}
		Player_Used_Bomb = false;
	}
}

void Player_Spawn(const XMFLOAT2& Position)
{
	PlayerPosition = Position;
	Player_Set_Enable(true);
}

void Player_Reset_For_Story()
{
	Player_Set_Position({ Game_Offset.x + (Game_Screen_Width * A_Half) - (PLAYER_WIDTH * A_Half),
		static_cast<float>(SCREEN_HEIGHT) + PLAYER_HEIGHT });
	PlayerVelocity = { 0.0f, 0.0f };
	P_State = PLAYER_STATE::NONE;
	Player_Set_Enable(true);
}

void Player_Draw()
{
	// IF Dead, Do Not Draw
	if (!Player_Enable) return;

	float Current_Player_X = PlayerPosition.x + (PlayerSize.x * A_Half);
	float Current_Player_Y = PlayerPosition.y + (PlayerSize.y * A_Half);
	Game_Draw_Bomb_Effect(Current_Player_X, Current_Player_Y);

	if (Status_Is_Player_Invincible() && (!(Get_Now_Stage_Flow()  == STAGE_FLOW_STATE::STORY_PLAYING)))
	{
		if (Status_Is_Bomb_Invincible())
			Player_Alpha = PLAYER_ALPHA_RE_SPAWN;
		else
		{
			if (static_cast<int>(SystemTimer_GetTime() * 10.0) % 2 == 0)
				Player_Alpha = PLAYER_ALPHA_RE_SPAWN;
			else
				Player_Alpha = PLAYER_ALPHA_ORIGINAL;
		}
	}
	else
		Player_Alpha = PLAYER_ALPHA_ORIGINAL;

	switch (P_State)
	{
	case PLAYER_STATE::NONE:
		SpriteAni_Draw(Play_Straight, PlayerPosition.x, PlayerPosition.y, PlayerSize.x, PlayerSize.y, PLAYER_ANGLE,
			{ A_Origin, A_Origin, A_Origin, Player_Alpha });
		break;

	case PLAYER_STATE::LEFT:
		SpriteAni_Draw(Play_Left, PlayerPosition.x, PlayerPosition.y, PlayerSize.x, PlayerSize.y, PLAYER_ANGLE,
			{ A_Origin, A_Origin, A_Origin, Player_Alpha });
		break;

	case PLAYER_STATE::RIGHT:
		SpriteAni_Draw(Play_Right, PlayerPosition.x, PlayerPosition.y, PlayerSize.x, PlayerSize.y, PLAYER_ANGLE,
			{ A_Origin, A_Origin, A_Origin, Player_Alpha });
		break;
	}

	if (Player_Avoid)
	{
		float Diameter = Player_Avoid_Radius * A_Double;

		float Player_Center_X = PlayerPosition.x + PlayerCollision.Center.x;
		float Player_Center_Y = PlayerPosition.y + PlayerCollision.Center.y;

		float Draw_x = Player_Center_X - Player_Avoid_Radius;
		float Draw_y = Player_Center_Y - Player_Avoid_Radius;

		Sprite_Draw(Avoid, Draw_x, Draw_y, Diameter, Diameter, A_Zero);
	}
}

bool Player_Is_Alive()
{
	return Player_Enable;
}

void Player_Set_Avoid_State(bool State)
{
	Player_Avoid = State;
}

Circle Player_Get_Collision()
{
	float Cx = PlayerPosition.x + PlayerCollision.Center.x;
	float Cy = PlayerPosition.y + PlayerCollision.Center.y;
	float Cr = PlayerCollision.Radius;

	if (Status_Is_Player_Invincible())
		Cr = A_Zero;

	return { { Cx, Cy }, Cr };
}

Circle Player_Get_Item_Collision()
{
	float Cx = PlayerPosition.x + PlayerCollision.Center.x;
	float Cy = PlayerPosition.y + PlayerCollision.Center.y;
	float Cr = Player_Item_Radius;

	return { { Cx, Cy }, Cr };
}

void Player_Destroy()
{
	if (Player_Enable) // Defense Multiple Destroy
	{
		SM->Play_SFX("Player_Dead");
		Effect_Create(Effect_Type::PLAYER_EXPLOSION, PlayerPosition, { PlayerSize.x * A_One_And_Half, PlayerSize.x * A_One_And_Half });
		Power_Create_On_Player_Death(PlayerPosition);

		Status_Start_Respawn_Invincibility(); // Start invincibility on death
		Player_Enable = false;
	}
}

void Player_Set_Position(const XMFLOAT2& Position)
{
	PlayerPosition = Position;
}

XMFLOAT2 Player_Get_Position()
{
	return PlayerPosition;
}

void Player_Set_State(PLAYER_STATE State)
{
	P_State = State;
}

void Player_Set_Enable(bool is_enable)
{
	Player_Enable = is_enable;
}

double Get_Bomb_Timer()
{
	return Boom_Cooldown_Timer;
}