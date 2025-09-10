/*==============================================================================

	Game Setting [Setting.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Setting.h"
#include "Game_Window.h"
#include "Palette.h"
#include "Audio_Manager.h"

using namespace PALETTE;

static int BGM_TexID = -1;
static int SFX_TexID = -1;
static int W_Mode_TexID = -1;
static int FS_Mode_TexID = -1;
static int Back_TexID = -1;
static int L_Button_TexID = -1;
static int R_Button_TexID = -1;

static int Sound_MIN_TexID  = -1;
static int Sound_1_TexID	= -1;
static int Sound_2_TexID	= -1;
static int Sound_3_TexID	= -1;
static int Sound_4_TexID	= -1;
static int Sound_5_TexID	= -1;
static int Sound_6_TexID	= -1;
static int Sound_7_TexID	= -1;
static int Sound_8_TexID	= -1;
static int Sound_9_TexID	= -1;
static int Sound_MAX_TexID	= -1;

static int NOW_BGM_TexID	= -1;
static int NOW_SFX_TexID	= -1;

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
	Update_Sub_Screen(Sub_Screen::S_WAIT);

	BGM_TexID		= Texture_Load(L"Resource/Texture/UI/Settings_BGM.png");
	SFX_TexID		= Texture_Load(L"Resource/Texture/UI/Settings_SFX.png");
	W_Mode_TexID	= Texture_Load(L"Resource/Texture/UI/Settings_W_Mode_Fixed.png");
	FS_Mode_TexID	= Texture_Load(L"Resource/Texture/UI/Settings_FS_Mode.png");
	Back_TexID		= Texture_Load(L"Resource/Texture/UI/Menu_Back_Fixed.png");
	L_Button_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Button_L.png");
	R_Button_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Button_R.png");

	Sound_MIN_TexID = Texture_Load(L"Resource/Texture/UI/Num/UI_Num_Min.png");
	Sound_1_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_1.png");
	Sound_2_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_2.png");
	Sound_3_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_3.png");
	Sound_4_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_4.png");
	Sound_5_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_5.png");
	Sound_6_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_6.png");
	Sound_7_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_7.png");
	Sound_8_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_8.png");
	Sound_9_TexID	= Texture_Load(L"Resource/Texture/UI/Num/UI_Num_9.png");
	Sound_MAX_TexID = Texture_Load(L"Resource/Texture/UI/Num/UI_Num_Max.png");

	if (Get_Mode_Buffer() == Get_Mode_Pre_Buffer())
	{
		Setting_Buffer = SETTING_BUFFER::SETTING_NONE;
		Sound_State = SOUND_SETTING_STATE::NONE;
		BGM_Buffer = SOUND_SCALE_BUFFER::SOUND_MAX;
		NOW_BGM_TexID = Sound_MAX_TexID;
		SFX_Buffer = SOUND_SCALE_BUFFER::SOUND_MAX;
		NOW_SFX_TexID = Sound_MAX_TexID;
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
	Set_Mode_Buffer(WM.Get_Now_Screen_Mode());

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
				SM->Play_SFX("Buffer_Back");

				Sound_Setting_Done = true;
				Reset_KeyLogger = true;
			}

			if (KeyLogger_IsTrigger(KK_A) || KeyLogger_IsTrigger(KK_LEFT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				if (BGM_Buffer <= SOUND_SCALE_BUFFER::SOUND_MIN)
					BGM_Buffer = SOUND_SCALE_BUFFER::SOUND_MIN;
				else
				{
					BGM_Buffer--;
					SM->Set_BGM_Volume(BGM_Buffer);
					SM->Play_SFX("Buffer_Move");
				}
			}
			else if (KeyLogger_IsTrigger(KK_D) || KeyLogger_IsTrigger(KK_RIGHT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				if (BGM_Buffer >= SOUND_SCALE_BUFFER::SOUND_MAX)
					BGM_Buffer = SOUND_SCALE_BUFFER::SOUND_MAX;
				else
				{
					BGM_Buffer++;
					SM->Set_BGM_Volume(BGM_Buffer);
					SM->Play_SFX("Buffer_Move");
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
				SM->Play_SFX("Buffer_Back");

				Sound_Setting_Done = true;
				Reset_KeyLogger = true;
			}

			if (KeyLogger_IsTrigger(KK_A) || KeyLogger_IsTrigger(KK_LEFT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_LEFT))
			{
				if (SFX_Buffer <= SOUND_SCALE_BUFFER::SOUND_MIN)
					SFX_Buffer = SOUND_SCALE_BUFFER::SOUND_MIN;
				else
				{
					SFX_Buffer--;
					SM->Set_SFX_Volume(SFX_Buffer);
					SM->Play_SFX("Buffer_Move");
				}
			}
			else if (KeyLogger_IsTrigger(KK_D) || KeyLogger_IsTrigger(KK_RIGHT) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_RIGHT))
			{
				if (SFX_Buffer >= SOUND_SCALE_BUFFER::SOUND_MAX)
					SFX_Buffer = SOUND_SCALE_BUFFER::SOUND_MAX;
				else
				{
					SFX_Buffer++;
					SM->Set_SFX_Volume(SFX_Buffer);
					SM->Play_SFX("Buffer_Move");
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
			SM->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Setting_Buffer(SETTING_BUFFER::BGM);
			SM->Play_SFX("Buffer_Move");
		}
		break;

	case SETTING_BUFFER::BGM:
		if (!Sound_Setting_Now && (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A)))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SOUND_SETTING);
			Update_Sound_Setting_State(SOUND_SETTING_STATE::BGM_SETTING);
			SM->Play_SFX("Buffer_Select");

			Sound_Setting_Now = true;
			Reset_KeyLogger = true;
		}

		if (!Sound_Setting_Now)
		{
			if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				Update_Setting_Buffer(SETTING_BUFFER::SFX);
				SM->Play_SFX("Buffer_Move");
			}
		}
		break;

	case SETTING_BUFFER::SFX:
		if (!Sound_Setting_Now && (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A)))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SOUND_SETTING);
			Update_Sound_Setting_State(SOUND_SETTING_STATE::SFX_SETTING);
			SM->Play_SFX("Buffer_Select");

			Sound_Setting_Now = true;
			Reset_KeyLogger = true;
		}

		if (!Sound_Setting_Now)
		{
			if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
			{
				Update_Setting_Buffer(SETTING_BUFFER::BGM);
				SM->Play_SFX("Buffer_Move");
			}
			else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
			{
				Update_Setting_Buffer(SETTING_BUFFER::WINDOW_MODE);
				SM->Play_SFX("Buffer_Move");
			}
		}
		break;

	case SETTING_BUFFER::SOUND_SETTING:
		break;

	case SETTING_BUFFER::WINDOW_MODE:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Set_Mode_Buffer(WM.Get_Now_Screen_Mode());
			Set_Mode_Pre_Buffer(Now_W_Mode_Buffer);
			Set_Mode_Buffer(ScreenMode::Window_Mode);

			if (WM.Get_Now_Screen_Mode() != Get_Mode_Buffer())
				Request_Screen_Mode_Change(Get_Mode_Buffer());
			else 
				SM->Play_SFX("Buffer_Denied");
		}
		else if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SFX);
			SM->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Setting_Buffer(SETTING_BUFFER::FULL_SCREEN_MODE);
			SM->Play_SFX("Buffer_Move");
		}

		break;

	case SETTING_BUFFER::FULL_SCREEN_MODE:
		if (KeyLogger_IsTrigger(KK_ENTER) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_A))
		{
			Set_Mode_Pre_Buffer(Now_W_Mode_Buffer);
			Set_Mode_Buffer(ScreenMode::FullScreen_Mode);

			if (WM.Get_Now_Screen_Mode() != Get_Mode_Buffer())
				Request_Screen_Mode_Change(Get_Mode_Buffer());
			else
				SM->Play_SFX("Buffer_Denied");
		}
		else if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Setting_Buffer(SETTING_BUFFER::WINDOW_MODE);
			SM->Play_SFX("Buffer_Move");
		}
		else if (KeyLogger_IsTrigger(KK_S) || KeyLogger_IsTrigger(KK_DOWN) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_DOWN))
		{
			Update_Setting_Buffer(SETTING_BUFFER::SETTING_BACK);
			SM->Play_SFX("Buffer_Move");
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

			SM->Play_SFX("Buffer_Back");

			Back_Triggered = true;
			Reset_KeyLogger = true;
		}
		if (KeyLogger_IsTrigger(KK_W) || KeyLogger_IsTrigger(KK_UP) || XKeyLogger_IsPadTrigger(XINPUT_GAMEPAD_DPAD_UP))
		{
			Update_Setting_Buffer(SETTING_BUFFER::FULL_SCREEN_MODE);
			SM->Play_SFX("Buffer_Move");
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
		Sprite_Draw(BGM_TexID, SOUND_BGM_X, SOUND_BGM_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);

		if (NOW_BGM_TexID == Sound_MAX_TexID)
			Sprite_Draw(NOW_BGM_TexID, NUM_MAX_X, BGM_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f);
		else
			Sprite_Draw(NOW_BGM_TexID, NUM_X, BGM_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
	}
	else
	{
		Sprite_Draw(BGM_TexID, SOUND_BGM_X, SOUND_BGM_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

		if (NOW_BGM_TexID == Sound_MAX_TexID)
			Sprite_Draw(NOW_BGM_TexID, NUM_MAX_X, BGM_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
		else
			Sprite_Draw(NOW_BGM_TexID, NUM_X, BGM_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
	}

	if (Get_Setting_State() == SOUND_SETTING_STATE::BGM_SETTING)
	{
		if (NOW_BGM_TexID == Sound_MIN_TexID)
			Sprite_Draw(R_Button_TexID, R_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else if (NOW_BGM_TexID == Sound_MAX_TexID)
			Sprite_Draw(L_Button_TexID, L_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else
		{
			Sprite_Draw(L_Button_TexID, L_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
			Sprite_Draw(R_Button_TexID, R_BUTTON_X, BGM_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		}
	}

	// SFX
	if (Get_Setting_Buffer() == SETTING_BUFFER::SFX || Get_Setting_Buffer() == SETTING_BUFFER::SOUND_SETTING)
	{
		Sprite_Draw(SFX_TexID, SOUND_SFX_X, SOUND_SFX_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);

		if (NOW_SFX_TexID == Sound_MAX_TexID)
			Sprite_Draw(NOW_SFX_TexID, NUM_MAX_X, SFX_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f);
		else
			Sprite_Draw(NOW_SFX_TexID, NUM_X, SFX_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
	}
	else
	{
		Sprite_Draw(SFX_TexID, SOUND_SFX_X, SOUND_SFX_Y, SOUND_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

		if (NOW_SFX_TexID == Sound_MAX_TexID)
			Sprite_Draw(NOW_SFX_TexID, NUM_MAX_X, SFX_NUM_Y, NUM_MAX_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
		else
			Sprite_Draw(NOW_SFX_TexID, NUM_X, SFX_NUM_Y, NUM_WIDTH, NUM_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });
	}

	if (Get_Setting_State() == SOUND_SETTING_STATE::SFX_SETTING)
	{
		if (NOW_SFX_TexID == Sound_MIN_TexID)
			Sprite_Draw(R_Button_TexID, R_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else if (NOW_SFX_TexID == Sound_MAX_TexID)
			Sprite_Draw(L_Button_TexID, L_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		else
		{
			Sprite_Draw(L_Button_TexID, L_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
			Sprite_Draw(R_Button_TexID, R_BUTTON_X, SFX_BUTTON_Y, NUM_WIDTH, NUM_HEIGHT, 0.f);
		}
	}

	// Mode
	if (Get_Setting_Buffer() == SETTING_BUFFER::WINDOW_MODE)
		Sprite_Draw(W_Mode_TexID, S_W_MODE_X, S_W_MODE_Y, W_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);
	else
		Sprite_Draw(W_Mode_TexID, S_W_MODE_X, S_W_MODE_Y, W_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

	if (Get_Setting_Buffer() == SETTING_BUFFER::FULL_SCREEN_MODE)
		Sprite_Draw(FS_Mode_TexID, S_FS_MODE_X, S_FS_MODE_Y, FS_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f);
	else
		Sprite_Draw(FS_Mode_TexID, S_FS_MODE_X, S_FS_MODE_Y, FS_MODE_UI_WIDTH, SETTING_UI_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

	// Back
	if (Get_Setting_Buffer() == SETTING_BUFFER::SETTING_BACK)
		Sprite_Draw(Back_TexID, BACK_X, BACK_Y, BACK_UI_WIDTH, BACK_UI_HEIGHT, 0.f);
	else
		Sprite_Draw(Back_TexID, BACK_X, BACK_Y, BACK_UI_WIDTH, BACK_UI_HEIGHT, 0.f, { A_Origin, A_Origin, A_Origin, A_Half });

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
		Return_TexID = Sound_MIN_TexID;
		break;

	case ONE:
		Return_TexID = Sound_1_TexID;
		break;

	case TWO:
		Return_TexID = Sound_2_TexID;
		break;

	case THREE:
		Return_TexID = Sound_3_TexID;
		break;

	case FOUR:
		Return_TexID = Sound_4_TexID;
		break;

	case FIVE:
		Return_TexID = Sound_5_TexID;
		break;

	case SIX:
		Return_TexID = Sound_6_TexID;
		break;

	case SEVEN:
		Return_TexID = Sound_7_TexID;
		break;

	case EIGHT:
		Return_TexID = Sound_8_TexID;
		break;

	case NINE:
		Return_TexID = Sound_9_TexID;
		break;

	case SOUND_MAX:
		Return_TexID = Sound_MAX_TexID;
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