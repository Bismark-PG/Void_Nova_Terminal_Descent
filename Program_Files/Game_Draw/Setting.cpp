/*==============================================================================

	Game Setting [Setting.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Setting.h"
#include "Game_Window.h"
#include "Palette.h"
#include "Audio_Manager.h"

using namespace PALETTE;

//----------------Number Texture----------------//
static int Num_Min	= -1;
static int Num_1	= -1;
static int Num_2	= -1;
static int Num_3	= -1;
static int Num_4	= -1;
static int Num_5	= -1;
static int Num_6	= -1;
static int Num_7	= -1;
static int Num_8	= -1;
static int Num_9	= -1;
static int Num_MAX	= -1;

static int NOW_BGM_TexID	= -1;
static int NOW_SFX_TexID	= -1;

//------------------UI Texture------------------//
static int UI_BGM = -1;
static int UI_SFX = -1;
static int UI_Button_L = -1;
static int UI_Button_R = -1;
static int UI_Window_Mode = -1;
static int UI_Full_Screan_Mode = -1;
static int UI_Back = -1;

SOUND_SETTING_STATE Sound_State;
SETTING_BUFFER Setting_Buffer;
static int BGM_Buffer;
static int SFX_Buffer;

static bool Sound_Setting_Now = false;
static bool Sound_Setting_Done = false;

ScreenMode Now_W_Mode_Buffer;
ScreenMode Pre_W_Mode_Buffer;
static bool Screen_Mode_Change_Now = false;
static int  Screen_Mode_Request = -1;

static bool Back_Triggered = false;
static bool Reset_KeyLogger = false;

static bool Setting_Done = false;

void Setting_Initialize()
{
	Setting_Texture();

	Update_Sub_Screen(Sub_Screen::S_WAIT);

	if (Get_Mode_Buffer() == Get_Mode_Pre_Buffer())
	{
		Setting_Buffer = SETTING_BUFFER::SETTING_NONE;
		Sound_State = SOUND_SETTING_STATE::NONE;
		BGM_Buffer = SOUND_SCALE_BUFFER::FIVE;
		NOW_BGM_TexID = Num_5;
		SFX_Buffer = SOUND_SCALE_BUFFER::FIVE;
		NOW_SFX_TexID = Num_5;
	}

	SOUND_UI_WIDTH    *= SETTING_UI_SCALE;
	W_MODE_UI_WIDTH   *= SETTING_UI_SCALE;
	FS_MODE_UI_WIDTH  *= SETTING_UI_SCALE;
	SETTING_UI_HEIGHT *= SETTING_UI_SCALE;

	BACK_UI_WIDTH	  *= BACK_UI_SCALE;
	BACK_UI_HEIGHT    *= BACK_UI_SCALE;

	NUM_WIDTH  *= NUM_SCALE;
	NUM_HEIGHT *= NUM_SCALE;
	
	SOUND_UI_WIDTH    *= Game_Scale;
	W_MODE_UI_WIDTH   *= Game_Scale;
	FS_MODE_UI_WIDTH  *= Game_Scale;
	SETTING_UI_HEIGHT *= Game_Scale;

	BACK_UI_WIDTH     *= Game_Scale;
	BACK_UI_HEIGHT    *= Game_Scale;

	NUM_WIDTH		*= Game_Scale;
	NUM_HEIGHT		*= Game_Scale;
	NUM_MAX_WIDTH	= NUM_WIDTH + NUM_WIDTH;

	SOUND_BGM_X     =  (BG_WIDTH  * SOUND)    - SOUND_UI_WIDTH;
	SOUND_BGM_Y     =  (BG_HEIGHT * BGM)	  - (SETTING_UI_HEIGHT * A_Half);	      				    
	SOUND_SFX_X     =  (BG_WIDTH  * SOUND)    - SOUND_UI_WIDTH;
	SOUND_SFX_Y     =  (BG_HEIGHT * SFX)	  - (SETTING_UI_HEIGHT * A_Half);
				      				    
	L_BUTTON_X      =  (BG_WIDTH  * L_BUTTON) - (NUM_WIDTH  * A_Half);
	R_BUTTON_X		=  (BG_WIDTH  * R_BUTTON) - (NUM_WIDTH  * A_Half);
	BGM_BUTTON_Y	=  (BG_HEIGHT * BGM)      - (NUM_HEIGHT * A_Half);
	SFX_BUTTON_Y	=  (BG_HEIGHT * SFX)      - (NUM_HEIGHT * A_Half);

	NUM_X			=  (BG_WIDTH  * NUMBER)   - (NUM_WIDTH		* A_Half);
	NUM_MAX_X		=  (BG_WIDTH  * NUMBER)   - (NUM_MAX_WIDTH  * A_Half);
	BGM_NUM_Y       =  (BG_HEIGHT * BGM)	  - (NUM_HEIGHT		* A_Half);
	SFX_NUM_Y		=  (BG_HEIGHT * SFX)	  - (NUM_HEIGHT		* A_Half);
				       
	S_W_MODE_X      =  (BG_WIDTH  * A_Half)     - (W_MODE_UI_WIDTH   * A_Half);
	S_W_MODE_Y      =  (BG_HEIGHT * W_MODE)   - (SETTING_UI_HEIGHT * A_Half);		       
	S_FS_MODE_X     =  (BG_WIDTH  * A_Half)     - (FS_MODE_UI_WIDTH  * A_Half);
	S_FS_MODE_Y     =  (BG_HEIGHT * FS_MODE)  - (SETTING_UI_HEIGHT * A_Half);

	BACK_X			=  (BG_WIDTH  * A_Half)	  - (BACK_UI_WIDTH     * A_Half);
	BACK_Y			=  (BG_HEIGHT * BACK)	  - (SETTING_UI_HEIGHT * A_Half);
}

void Setting_Finalize()
{
	Update_Sub_Screen(Sub_Screen::S_DONE);

	SOUND_BGM_X = 0;
	SOUND_BGM_Y = 0;
	SOUND_SFX_X = 0;
	SOUND_SFX_Y = 0;

	S_W_MODE_X	= 0;
	S_W_MODE_Y	= 0;
	S_FS_MODE_X = 0;
	S_FS_MODE_Y = 0;

	BACK_X = 0;
	BACK_Y = 0;

	SOUND_UI_WIDTH		= 2;
	W_MODE_UI_WIDTH		= 7;
	FS_MODE_UI_WIDTH	= 9;
	SETTING_UI_HEIGHT	= 1;

	BACK_UI_WIDTH	= 3;
	BACK_UI_HEIGHT	= 1;

	BACK_UI_SCALE	= 50;

	L_BUTTON_X		= 0;
	R_BUTTON_X		= 0;
	BGM_BUTTON_Y	= 0;
	SFX_BUTTON_Y	= 0;

	NUM_X		= 0;
	NUM_MAX_X	= 0;
	BGM_NUM_Y	= 0;
	SFX_NUM_Y	= 0;

	NUM_WIDTH		= 3;
	NUM_HEIGHT		= 5;
	NUM_MAX_WIDTH	= 0;
}

void Setting_Update()
{
	Set_Mode_Buffer(Window_M.Get_Now_Screen_Mode());

	if (Reset_KeyLogger)
	{
		if (KeyLogger_IsAnyKeyReleased() || XKeyLogger_IsAnyPadReleased())
			Reset_KeyLogger = false;
		return;
	}

	if (Sound_Setting_Now)
	{
		switch (Sound_State)
		{
		case SOUND_SETTING_STATE::NONE:
			Sound_Setting_Done = false;
			break;

		case SOUND_SETTING_STATE::BGM_SETTING:
			if (!Sound_Setting_Done && (KeyLogger_IsTrigger(KK_ENTER) || KeyLogger_IsTrigger(KK_BACK)
				|| XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_B)))
			{
				Update_Sound_Setting_State(SOUND_SETTING_STATE::SET_DONE);
				Update_Setting_Buffer(SETTING_BUFFER::BGM);
				Sound_M->Play_SFX("Buffer_Back");

				Sound_Setting_Done = true;
				Reset_KeyLogger = true;
			}

			if (KeyLogger_IsTrigger(KK_A) || KeyLogger_IsTrigger(KK_LEFT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				if (BGM_Buffer > SOUND_SCALE_BUFFER::SOUND_MIN)
				{
					BGM_Buffer--;
                    float Volume = static_cast<float>(BGM_Buffer) / static_cast<float>(SOUND_SCALE_BUFFER::SOUND_MAX);
                    Sound_M->Set_Target_BGM_Volume(Volume);
					Sound_M->Play_SFX("Buffer_Move");
				}
			}
			else if (KeyLogger_IsTrigger(KK_D) || KeyLogger_IsTrigger(KK_RIGHT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				if (BGM_Buffer < SOUND_SCALE_BUFFER::SOUND_MAX)
				{
					BGM_Buffer++;
                    float Volume = static_cast<float>(BGM_Buffer) / static_cast<float>(SOUND_SCALE_BUFFER::SOUND_MAX);
                    Sound_M->Set_Target_BGM_Volume(Volume);
					Sound_M->Play_SFX("Buffer_Move");
				}
			}

			Setting_Sound_TexID(BGM_Buffer, Get_Setting_State());
			break;

		case SOUND_SETTING_STATE::SFX_SETTING:
			if (!Sound_Setting_Done && (KeyLogger_IsTrigger(KK_ENTER) || KeyLogger_IsTrigger(KK_BACK)
				|| XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_B)))
			{
				Update_Setting_Buffer(SETTING_BUFFER::SFX);
				Update_Sound_Setting_State(SOUND_SETTING_STATE::SET_DONE);
				Sound_M->Play_SFX("Buffer_Back");

				Sound_Setting_Done = true;
				Reset_KeyLogger = true;
			}

			if (KeyLogger_IsTrigger(KK_A) || KeyLogger_IsTrigger(KK_LEFT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				if (SFX_Buffer > SOUND_SCALE_BUFFER::SOUND_MIN)
				{
					SFX_Buffer--;
					float Volume = static_cast<float>(SFX_Buffer) / static_cast<float>(SOUND_SCALE_BUFFER::SOUND_MAX);
					Sound_M->Set_Target_SFX_Volume(Volume);
					Sound_M->Play_SFX("Buffer_Move");
				}
			}
			else if (KeyLogger_IsTrigger(KK_D) || KeyLogger_IsTrigger(KK_RIGHT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				if (SFX_Buffer < SOUND_SCALE_BUFFER::SOUND_MAX)
				{
					SFX_Buffer++;
					float Volume = static_cast<float>(SFX_Buffer) / static_cast<float>(SOUND_SCALE_BUFFER::SOUND_MAX);
					Sound_M->Set_Target_SFX_Volume(Volume);
					Sound_M->Play_SFX("Buffer_Move");
				}
			}

			Setting_Sound_TexID(SFX_Buffer, Get_Setting_State());
			break;

		case SOUND_SETTING_STATE::SET_DONE:
			Sound_Setting_Done = false;
			Sound_Setting_Now = false;
			break;
		}
	}

	switch (Setting_Buffer)
	{
	case SETTING_BUFFER::SETTING_NONE:
		Setting_Done = false;
		break;

	case SETTING_BUFFER::SETTING_WAIT:
		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SETTING_BACK);
			Sound_M->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Setting_Buffer(SETTING_BUFFER::BGM);
			Sound_M->Play_SFX("Buffer_Move");
		}
		break;

	case SETTING_BUFFER::BGM:
		if (!Sound_Setting_Now && (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A)))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SOUND_SETTING);
			Update_Sound_Setting_State(SOUND_SETTING_STATE::BGM_SETTING);
			Sound_M->Play_SFX("Buffer_Select");

			Sound_Setting_Now = true;
			Reset_KeyLogger = true;
		}

		if (!Sound_Setting_Now)
		{
			if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				Update_Setting_Buffer(SETTING_BUFFER::SFX);
				Sound_M->Play_SFX("Buffer_Move");
			}
		}
		break;

	case SETTING_BUFFER::SFX:
		if (!Sound_Setting_Now && (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A)))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SOUND_SETTING);
			Update_Sound_Setting_State(SOUND_SETTING_STATE::SFX_SETTING);
			Sound_M->Play_SFX("Buffer_Select");

			Sound_Setting_Now = true;
			Reset_KeyLogger = true;
		}

		if (!Sound_Setting_Now)
		{
			if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
			{
				Update_Setting_Buffer(SETTING_BUFFER::BGM);
				Sound_M->Play_SFX("Buffer_Move");
			}
			else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				Update_Setting_Buffer(SETTING_BUFFER::WINDOW_MODE);
				Sound_M->Play_SFX("Buffer_Move");
			}
		}
		break;

	case SETTING_BUFFER::SOUND_SETTING:
		break;

	case SETTING_BUFFER::WINDOW_MODE:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Set_Mode_Buffer(Window_M.Get_Now_Screen_Mode());
			Set_Mode_Pre_Buffer(Now_W_Mode_Buffer);
			Set_Mode_Buffer(ScreenMode::Window_Mode);

			if (Window_M.Get_Now_Screen_Mode() != Get_Mode_Buffer())
				Request_Screen_Mode_Change(Get_Mode_Buffer());
			else 
				Sound_M->Play_SFX("Buffer_Denied");
		}
		else if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SFX);
			Sound_M->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Setting_Buffer(SETTING_BUFFER::FULL_SCREEN_MODE);
			Sound_M->Play_SFX("Buffer_Move");
		}

		break;

	case SETTING_BUFFER::FULL_SCREEN_MODE:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Set_Mode_Pre_Buffer(Now_W_Mode_Buffer);
			Set_Mode_Buffer(ScreenMode::FullScreen_Mode);

			if (Window_M.Get_Now_Screen_Mode() != Get_Mode_Buffer())
				Request_Screen_Mode_Change(Get_Mode_Buffer());
			else
				Sound_M->Play_SFX("Buffer_Denied");
		}
		else if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Setting_Buffer(SETTING_BUFFER::WINDOW_MODE);
			Sound_M->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SETTING_BACK);
			Sound_M->Play_SFX("Buffer_Move");
		}

		break;

	case SETTING_BUFFER::SETTING_BACK:
		if (!Back_Triggered && (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A)))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SETTING_DONE);

			Update_Main_Buffer(UI_STATE::MENU_UI);
			Update_Main_Select_Buffer(MENU_BUFFER::SETTINGS);

			Update_Main_Screen(Main_Screen::MENU_SELECT);
			Update_Sub_Screen(Sub_Screen::S_DONE);

			Sound_M->Play_SFX("Buffer_Back");

			Back_Triggered = true;
			Reset_KeyLogger = true;
		}
		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Setting_Buffer(SETTING_BUFFER::FULL_SCREEN_MODE);
			Sound_M->Play_SFX("Buffer_Move");
		}
		break;

	case SETTING_BUFFER::SETTING_DONE:
		Setting_Done = true;
		break;

	case SETTING_BUFFER::DONE:
		break;
	}

	if (Setting_Done)
	{
		Back_Triggered = false;
		Reset_KeyLogger = true;
		Setting_Done = false;
	}
}

void Setting_Draw()
{
	Menu_BG();
	Setting_UI_Draw();
}

void Setting_UI_Draw()
{
	// BGM
	if (Get_Setting_Buffer() == SETTING_BUFFER::BGM || Get_Setting_Buffer() == SETTING_BUFFER::SOUND_SETTING)
	{
		Sprite_Draw(UI_BGM, SOUND_BGM_X, SOUND_BGM_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);

		if (NOW_BGM_TexID == Num_MAX)
			Sprite_Draw(NOW_BGM_TexID, NUM_MAX_X, BGM_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f);
		else
			Sprite_Draw(NOW_BGM_TexID, NUM_X, BGM_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
	}
	else
	{
		Sprite_Draw(UI_BGM, SOUND_BGM_X, SOUND_BGM_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

		if (NOW_BGM_TexID == Num_MAX)
			Sprite_Draw(NOW_BGM_TexID, NUM_MAX_X, BGM_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
		else
			Sprite_Draw(NOW_BGM_TexID, NUM_X, BGM_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
	}

	if (Get_Setting_State() == SOUND_SETTING_STATE::BGM_SETTING)
	{
		if (NOW_BGM_TexID == Num_Min)
			Sprite_Draw(UI_Button_R, R_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else if (NOW_BGM_TexID == Num_MAX)
			Sprite_Draw(UI_Button_L, L_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else
		{
			Sprite_Draw(UI_Button_L, L_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
			Sprite_Draw(UI_Button_R, R_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		}
	}

	// SFX
	if (Get_Setting_Buffer() == SETTING_BUFFER::SFX || Get_Setting_Buffer() == SETTING_BUFFER::SOUND_SETTING)
	{
		Sprite_Draw(UI_SFX, SOUND_SFX_X, SOUND_SFX_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);

		if (NOW_SFX_TexID == Num_MAX)
			Sprite_Draw(NOW_SFX_TexID, NUM_MAX_X, SFX_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f);
		else
			Sprite_Draw(NOW_SFX_TexID, NUM_X, SFX_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
	}
	else
	{
		Sprite_Draw(UI_SFX, SOUND_SFX_X, SOUND_SFX_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

		if (NOW_SFX_TexID == Num_MAX)
			Sprite_Draw(NOW_SFX_TexID, NUM_MAX_X, SFX_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
		else
			Sprite_Draw(NOW_SFX_TexID, NUM_X, SFX_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
	}

	if (Get_Setting_State() == SOUND_SETTING_STATE::SFX_SETTING)
	{
		if (NOW_SFX_TexID == Num_Min)
			Sprite_Draw(UI_Button_R, R_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else if (NOW_SFX_TexID == Num_MAX)
			Sprite_Draw(UI_Button_L, L_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else
		{
			Sprite_Draw(UI_Button_L, L_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
			Sprite_Draw(UI_Button_R, R_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		}
	}

	// Mode
	if (Get_Setting_Buffer() == SETTING_BUFFER::WINDOW_MODE)
		Sprite_Draw(UI_Window_Mode, S_W_MODE_X, S_W_MODE_Y, W_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);
	else
		Sprite_Draw(UI_Window_Mode, S_W_MODE_X, S_W_MODE_Y, W_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, {A_Origin, A_Origin, A_Origin, A_Half});

	if (Get_Setting_Buffer() == SETTING_BUFFER::FULL_SCREEN_MODE)
		Sprite_Draw(UI_Full_Screan_Mode, S_FS_MODE_X, S_FS_MODE_Y, FS_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);
	else
		Sprite_Draw(UI_Full_Screan_Mode, S_FS_MODE_X, S_FS_MODE_Y, FS_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, {A_Origin, A_Origin, A_Origin, A_Half});

	// Back
	if (Get_Setting_Buffer() == SETTING_BUFFER::SETTING_BACK)
		Sprite_Draw(UI_Back, BACK_X, BACK_Y, BACK_UI_WIDTH, BACK_UI_HEIGHT, 0.f);
	else
		Sprite_Draw(UI_Back, BACK_X, BACK_Y, BACK_UI_WIDTH, BACK_UI_HEIGHT, 0.f, {A_Origin, A_Origin, A_Origin, A_Half});

}

void Update_Setting_Buffer(SETTING_BUFFER Buffer)
{
	Setting_Buffer = Buffer;
}

SETTING_BUFFER Get_Setting_Buffer()
{
	return Setting_Buffer;
}

void Update_Sound_Setting_State(SOUND_SETTING_STATE State)
{
	Sound_State = State;
}

SOUND_SETTING_STATE Get_Setting_State()
{
	return Sound_State;
}

void Update_BGM_Scale_Buffer(SOUND_SCALE_BUFFER BGM_Scale)
{
	BGM_Buffer = BGM_Scale;
}

SOUND_SCALE_BUFFER Get_BGM_Scale_Buffer()
{
	return static_cast<SOUND_SCALE_BUFFER>(BGM_Buffer);
}

void Update_SFX_Scale_Buffer(SOUND_SCALE_BUFFER SFX_Scale)
{
	SFX_Buffer = SFX_Scale;
}

SOUND_SCALE_BUFFER Get_SFX_Scale_Buffer()
{
	return static_cast<SOUND_SCALE_BUFFER>(SFX_Buffer);
}

void Set_Mode_Buffer(ScreenMode Mode)
{
	Now_W_Mode_Buffer = Mode;
}

ScreenMode Get_Mode_Buffer()
{
	return Now_W_Mode_Buffer;
}

void Set_Mode_Pre_Buffer(ScreenMode Mode)
{
	Pre_W_Mode_Buffer = Mode;
}

ScreenMode Get_Mode_Pre_Buffer()
{
	return Pre_W_Mode_Buffer;
}

void Request_Screen_Mode_Change(ScreenMode Mode)
{
	Screen_Mode_Request = static_cast<int>(Mode);
}

ScreenMode Screen_Mode_Change_Request()
{
	if (Screen_Mode_Request != -1)
	{
		ScreenMode requestedMode = static_cast<ScreenMode>(Screen_Mode_Request);
		Screen_Mode_Request = -1; // Request Clear
		return requestedMode;
	}

	return (ScreenMode)-1;
}

void Setting_Sound_TexID(int Now_Scale, SOUND_SETTING_STATE Now_State)
{
	int Return_TexID = -1;

	switch (Now_Scale)
	{
	case SOUND_MIN:
		Return_TexID = Num_Min;
		break;

	case ONE:
		Return_TexID = Num_1;
		break;

	case TWO:
		Return_TexID = Num_2;
		break;

	case THREE:
		Return_TexID = Num_3;
		break;

	case FOUR:
		Return_TexID = Num_4;
		break;

	case FIVE:
		Return_TexID = Num_5;
		break;

	case SIX:
		Return_TexID = Num_6;
		break;

	case SEVEN:
		Return_TexID = Num_7;
		break;

	case EIGHT:
		Return_TexID = Num_8;
		break;

	case NINE:
		Return_TexID = Num_9;
		break;

	case SOUND_MAX:
		Return_TexID = Num_MAX;
		break;
	}

	switch (Now_State)
	{
	case SOUND_SETTING_STATE::BGM_SETTING:
		NOW_BGM_TexID = Return_TexID;
		break;

	case SOUND_SETTING_STATE::SFX_SETTING:
		NOW_SFX_TexID = Return_TexID;
		break;
	}
}

void Setting_Texture()
{
	//----------------Number Texture----------------//
	Num_Min = Texture_M->GetID("UI_Num_MIN");
	Num_1	= Texture_M->GetID("UI_Num_1");
	Num_2	= Texture_M->GetID("UI_Num_2");
	Num_3	= Texture_M->GetID("UI_Num_3");
	Num_4	= Texture_M->GetID("UI_Num_4");
	Num_5	= Texture_M->GetID("UI_Num_5");
	Num_6	= Texture_M->GetID("UI_Num_6");
	Num_7	= Texture_M->GetID("UI_Num_7");
	Num_8	= Texture_M->GetID("UI_Num_8");
	Num_9	= Texture_M->GetID("UI_Num_9");
	Num_MAX = Texture_M->GetID("UI_Num_MAX");

	//------------------UI Texture------------------//
	UI_BGM = Texture_M->GetID("UI_BGM");
	UI_SFX = Texture_M->GetID("UI_SFX");
	UI_Button_L = Texture_M->GetID("UI_Num_Button_L");
	UI_Button_R = Texture_M->GetID("UI_Num_Button_R");
	UI_Window_Mode = Texture_M->GetID("UI_Window_Mode");
	UI_Full_Screan_Mode = Texture_M->GetID("UI_Full_Screen_Mode");
	UI_Back = Texture_M->GetID("UI_Back");
}
