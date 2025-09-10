/*==============================================================================

	Save / Restore Game State And Buffer [Saver_And_Re_Create.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include "Saver_And_Re_Create.h"
#include "../Programe_Settings/Audio_Manager.h"
#include <string>

State_And_Buffer_Storage Saver = {};

void Save_State_And_Buffer()
{
	Saver.Main_State = GM.Get_Current_Main_Screen();
	Saver.Sub_State = GM.Get_Current_Sub_Screen();
	Saver.Game_State = GM.Get_Current_Game_Select_Screen();
	Saver.Main_Draw_State = Get_Main_Draw_State();
	Saver.Main_Select_Buffer = Get_Main_Select_Buffer();
	Saver.Setting_Select_Buffer = Get_Setting_Buffer();
	Saver.Sound_Setting_Buffer = Get_Setting_State();
	Saver.BGM_Scale_Buffer = Get_BGM_Scale_Buffer();
	Saver.SFX_Scale_Buffer = Get_SFX_Scale_Buffer();

	if (SM)
		Saver.Last_Playing_BGM = SM->Get_Playing_BGM_Name();
}

void Restore_State_And_Buffer()
{
	Update_Main_Screen(Saver.Main_State);
	Update_Sub_Screen(Saver.Sub_State);
	Update_Game_Select_Screen(Saver.Game_State);
	Update_Main_Buffer(Saver.Main_Draw_State);
	Update_Main_Select_Buffer(Saver.Main_Select_Buffer);
	Update_Setting_Buffer(Saver.Setting_Select_Buffer);
	Update_Sound_Setting_State(Saver.Sound_Setting_Buffer);
	Update_BGM_Scale_Buffer(Saver.BGM_Scale_Buffer);
	Update_SFX_Scale_Buffer(Saver.SFX_Scale_Buffer);

	if (SM && !Saver.Last_Playing_BGM.empty())
		SM->Play_BGM(Saver.Last_Playing_BGM);
}