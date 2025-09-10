/*==============================================================================

	Recreate Window And Manage [Window_Re_Creator.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "Window_Re_Creator.h"
#include "Saver_And_Re_Create.h"
#include "System_Logic_Manager.h"
#include "Game_Window.h"


void Window_Re_Create(ScreenMode Mode)
{
	std::stringstream Debug;
	Debug << "[Regeneration_Manager] Start Window Re-Create\n";

	Save_State_And_Buffer();
	Debug << " - State & Buffer saved.\n";

	System_Finalize();
	Debug << " - System finalized.\n";

	WM.Game_Window_Re_Create(Mode, Handle_Instance);
	if (Game_Window && IsWindow(Game_Window))
		Debug << " - Window re-created successfully.\n";
	else
	{
		Debug << "\t\t\t\t*>> Fetal ERROR <<*\n [Regeneration_Manager] Window RE_Create Failed.\n";
		OutputDebugStringA(Debug.str().c_str());

		return;
	}

	System_Initialize(Game_Window);
	Debug << " - System re-initialized.\\n";

	Restore_State_And_Buffer();
	Debug << " - State & Buffer restored.\\n";

	Debug << "[Regeneration_Manager] Screen Re_Create is Succeed With Safely\\n";

	OutputDebugStringA(Debug.str().c_str());
}