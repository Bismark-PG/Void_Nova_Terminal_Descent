/*==============================================================================

	Keyboard Input Setting [KeyLogger.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "KeyLogger.h"
#pragma comment(lib, "Xinput.lib")

using namespace DirectX;

static Keyboard_State PrevState{};
static Keyboard_State TriggerState{};
static Keyboard_State ReleaseState{};

static XINPUT_STATE Prev_XS{};
static XINPUT_STATE Curr_XS{};
static WORD TriggerButtons = 0;
static WORD ReleaseButtons = 0;
static int X_Vibrate;

static bool Is_Controller_Used;
static bool Is_Controller_Input;
static bool Is_Controller_Output;
static bool Is_Key_INPUT;

static XMFLOAT2 LeftStick{};
static BYTE LeftTrigger = 0;
static BYTE RightTrigger = 0;

void KeyLogger_Initialize()
{
	X_Vibrate = Frame_Rate::Frame_Rate_None;
	Is_Controller_Used = false;
	Is_Controller_Input = false;
	Is_Controller_Output = false;
	Is_Key_INPUT = false;
	Keyboard_Initialize();
}

void KeyLogger_Update()
{

	static bool PrevConnected = false;
	bool IsConnected = false;
	XINPUT_STATE temp{};
	XINPUT_VIBRATION XV{};

	if (XInputGetState(0, &temp) == ERROR_SUCCESS)
	{
		IsConnected = true;

		Is_Controller_Used = (temp.Gamepad.wButtons != 0) ||
						   	 (temp.Gamepad.sThumbLX != 0) ||
						   	 (temp.Gamepad.sThumbLY != 0) ||
						   	 (temp.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ||
						   	 (temp.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ||
							  Is_Controller_Used;
	}
	else
		Is_Controller_Used = false;

	if (Is_Controller_Used)
		Is_Key_INPUT = false;
	                                                                                                                                                                                                  
	if (!PrevConnected && IsConnected)
	{
		Is_Controller_Input = true;
		X_Vibrate = Frame_Rate::Half_Sec;
	}
	else if (PrevConnected && !IsConnected)
		Is_Controller_Output = true;

	if (X_Vibrate > 0)
	{
		XV.wLeftMotorSpeed = 30000;
		XV.wRightMotorSpeed = 30000;
		XInputSetState(0, &XV);
		X_Vibrate--;
	}
	else
	{
		XV.wLeftMotorSpeed = 0;
		XV.wRightMotorSpeed = 0;
		XInputSetState(0, &XV);
	}

	if (Is_Controller_Used && KeyLogger_IsAnyKeyTriggered())
	{
		Is_Controller_Used = false;
		Is_Key_INPUT = true;
	}

	PrevConnected = IsConnected;

	// Get Now Keyboard State
	const Keyboard_State* P_State = Keyboard_GetState();
	
	LPBYTE P_Now	 = (LPBYTE)P_State;
	LPBYTE P_Prev    = (LPBYTE)&PrevState;
	LPBYTE P_Trigger = (LPBYTE)&TriggerState;
	LPBYTE P_Release = (LPBYTE)&ReleaseState;

	for (int i = 0; i < sizeof(Keyboard_State); i++)
	{
		P_Trigger[i] = (P_Prev[i] ^ P_Now[i]) & P_Now[i];
		P_Release[i] = (P_Prev[i] ^ P_Now[i]) & P_Prev[i];
	}

	PrevState = *P_State;

	// Get Now Gamepad State
	Prev_XS = Curr_XS;
	XInputGetState(0, &Curr_XS);

	WORD prev = Prev_XS.Gamepad.wButtons;
	WORD curr = Curr_XS.Gamepad.wButtons;

	TriggerButtons = (prev ^ curr) & curr;
	ReleaseButtons = (prev ^ curr) & prev;

	LeftStick.x = Curr_XS.Gamepad.sThumbLX;
	LeftStick.y = Curr_XS.Gamepad.sThumbLY;
	LeftTrigger = Curr_XS.Gamepad.bLeftTrigger;
	RightTrigger = Curr_XS.Gamepad.bRightTrigger;
}

bool KeyLogger_IsPressed(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key);
}

bool KeyLogger_IsTrigger(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &TriggerState);
}

bool KeyLogger_IsReleased(Keyboard_Keys key)
{
	return Keyboard_IsKeyDown(key, &ReleaseState);
}

bool KeyLogger_IsAnyKeyTriggered()
{
	LPBYTE Any_Trigger = (LPBYTE)&TriggerState;

	for (int i = 0; i < sizeof(Keyboard_State); i++)
	{
		if (Any_Trigger[i] != 0)
			return true;
	}
	return false;
}

bool KeyLogger_IsAnyKeyReleased()
{
	LPBYTE Any_Release = (LPBYTE)&ReleaseState;

	for (int i = 0; i < sizeof(Keyboard_State); i++)
	{
		if (Any_Release[i] != 0)
			return true;
	}
	return false;
}

bool KeyLogger_IsKeyInput()
{
	return Is_Key_INPUT;
}

bool XKeyLogger_IsControllerInput()
{
	return Is_Controller_Used;
}

bool XKeyLogger_GetControllerConnected()
{
	bool State = Is_Controller_Input;
	Is_Controller_Input = false;
	return State;
}

bool XKeyLogger_GetControllerDisconnected()
{
	bool State = Is_Controller_Output;
	Is_Controller_Output = false;
	return State;
}

bool XKeyLogger_IsPadPressed(WORD button)
{
	return Curr_XS.Gamepad.wButtons & button;
}

bool XKeyLogger_IsPadTrigger(WORD button)
{
	return TriggerButtons & button;
}

bool XKeyLogger_IsPadReleased(WORD button)
{
	return ReleaseButtons & button;
}
XMFLOAT2 XKeyLogger_GetLeftStick()
{
	return LeftStick;
}

BYTE XKeyLogger_GetLeftTrigger()
{
	return LeftTrigger;
}

BYTE XKeyLogger_GetRightTrigger()
{
	return RightTrigger;
}

bool XKeyLogger_IsAnyPadReleased()
{
	return ReleaseButtons != 0;
}

void KeyLogger_Reset()
{
	Keyboard_Reset();
}
