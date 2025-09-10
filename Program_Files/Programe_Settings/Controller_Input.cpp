/*==============================================================================

	Controller Input Draw [Controller_Input.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Controller_Input.h"
#include "Audio_Manager.h"

static int Controller_Alert_BG = -1;
static int Controller_Alert_Input = -1;
static int Controller_Alert_Output = -1;
static int Controller_Alert_Button_Up = -1;
static int Controller_Alert_Button_Press = -1;
static int Controller_Alert_Enter_Up = -1;
static int Controller_Alert_Enter_Press = -1;

static bool Is_Pressed = false;
static double Pressed_Time = 0.0f;

static bool Controller_Setup_State;

CONTROLLER_STATE STATE;

void Controller_Set_Initialize()
{
	Controller_Setup_State = false;

	Controller_Alert_BG				= Texture_Load(L"Resource/Texture/Controller/Controller_Input_BG_Fixed.png");
	Controller_Alert_Input			= Texture_Load(L"Resource/Texture/Controller/Controller_Input_Alert_Fixed.png");
	Controller_Alert_Output			= Texture_Load(L"Resource/Texture/Controller/Controller_Output_Alert_Fixed.png");
	Controller_Alert_Button_Up		= Texture_Load(L"Resource/Texture/Controller/A_Pressed_UP.png");
	Controller_Alert_Button_Press	= Texture_Load(L"Resource/Texture/Controller/A_Pressed_Fixed.png");
	Controller_Alert_Enter_Up		= Texture_Load(L"Resource/Texture/Controller/Enter_Alpha.png");
	Controller_Alert_Enter_Press	= Texture_Load(L"Resource/Texture/Controller/Enter_Alpha_Press.png");

	STATE = CONTROLLER_STATE::NONE;

	Alert_BG_Width	= SCREEN_WIDTH	* 0.75f;
	Alert_BG_Height = SCREEN_HEIGHT * 0.75f;
	Alert_BG_X = (SCREEN_WIDTH  - Alert_BG_Width)  * 0.5f;
	Alert_BG_y = (SCREEN_HEIGHT - Alert_BG_Height) * 0.5f;

	Alert_Width	 = Alert_BG_Width * 0.9f;
	Alert_Height = Alert_Width	  * 0.15f;
	Alert_X = Alert_BG_X + ((Alert_BG_Width  - Alert_Width)  * 0.5f);
	Alert_y = Alert_BG_y + ((Alert_BG_Height - Alert_Height) * 0.5f);

	Button_X = Alert_BG_Width * 0.95f;
	Button_y = Alert_BG_Height * 0.95f;
	Button_Size = Alert_BG_Width * 0.1f;

	Pressed_Time = SystemTimer_GetTime();
}

void Controller_Set_Finalize()
{
	Controller_Setup_State = false;

	Alert_BG_X	= 0;
	Alert_BG_y	= 0;
	Alert_BG_Width	= 0;
	Alert_BG_Height	= 0;

	Alert_X	= 0;
	Alert_y	= 0;
	Alert_Width  = 0;
	Alert_Height = 0;

	Button_X = 0;
	Button_y = 0;
	Button_Size = 0;
}

void Controller_Set_Update()
{
	switch (STATE)
	{
	case CONTROLLER_STATE::NONE:
		if (XKeyLogger_GetControllerConnected())
		{
			STATE = CONTROLLER_STATE::INPUT;
			Controller_Setup_State = true;
			SM->Play_SFX("Controller_Alert");
		}

		if (XKeyLogger_GetControllerDisconnected())
		{
			STATE = CONTROLLER_STATE::OUTPUT;
			Controller_Setup_State = true;
			SM->Play_SFX("Controller_Alert");
		}
		break;

	case CONTROLLER_STATE::INPUT:
		if (XKeyLogger_IsPadReleased(XINPUT_GAMEPAD_A))
		{
			Controller_Setup_State = false;
			STATE = CONTROLLER_STATE::NONE;
			SM->Play_SFX("Buffer_Denied");
		}

		if ((int)(SystemTimer_GetTime() - Pressed_Time) % 2 == 0)
			Is_Pressed = false;
		else if ((int)(SystemTimer_GetTime() - Pressed_Time) % 2 == 1)
			Is_Pressed = true;
		break;

	case CONTROLLER_STATE::OUTPUT:
		if (KeyLogger_IsReleased(KK_ENTER))
		{
			Controller_Setup_State = false;
			STATE = CONTROLLER_STATE::NONE;
			SM->Play_SFX("Buffer_Denied");
		}

		if ((int)(SystemTimer_GetTime() - Pressed_Time) % 2 == 0)
			Is_Pressed = false;
		else if ((int)(SystemTimer_GetTime() - Pressed_Time) % 2 == 1)
			Is_Pressed = true;
		break;
	}
}

void Controller_Set_Draw()
{
	switch (STATE)
	{
	case CONTROLLER_STATE::NONE:
		break;

	case CONTROLLER_STATE::INPUT:
		Sprite_Draw(Controller_Alert_BG, Alert_BG_X, Alert_BG_y, Alert_BG_Width, Alert_BG_Height, 0.f);
		Sprite_Draw(Controller_Alert_Input, Alert_X, Alert_y, Alert_Width, Alert_Height, 0.f);

		if(Is_Pressed)
			Sprite_Draw(Controller_Alert_Button_Up, Button_X, Button_y, Button_Size, Button_Size, 0.f);
		else
			Sprite_Draw(Controller_Alert_Button_Press, Button_X, Button_y, Button_Size, Button_Size, 0.f);
		break;

	case CONTROLLER_STATE::OUTPUT:
		Sprite_Draw(Controller_Alert_BG, Alert_BG_X, Alert_BG_y, Alert_BG_Width, Alert_BG_Height, 0.f);
		Sprite_Draw(Controller_Alert_Output, Alert_X, Alert_y, Alert_Width, Alert_Height, 0.f);

		if (Is_Pressed)
			Sprite_Draw(Controller_Alert_Enter_Up, Button_X, Button_y, Button_Size, Button_Size, 0.f);
		else
			Sprite_Draw(Controller_Alert_Enter_Press, Button_X, Button_y, Button_Size, Button_Size, 0.f);
		break;
	}
}

bool Controller_Set_UP()
{
	return Controller_Setup_State;
}
