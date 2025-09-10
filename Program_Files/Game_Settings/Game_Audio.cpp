/*==============================================================================

	Manage Game Audio Resource [Game_Audio.h]

	Author : Choi HyungJoon

--------------------------------------------------------------------------------
	- Reference

		¡ BGM ¡

		Ÿ DOVA-SYNDROME
		> https://dova-s.jp/

		¡ SFX ¡

		Ÿ  Sound Effect Lab (Œø‰Ê‰¹ƒ‰ƒ{)
		> https://soundeffect-lab.info/

==============================================================================*/
#include "Game_Audio.h"
#include "Audio_Manager.h"

void Game_Audio_Initialize()
{
	SM->Init();

	Game_Audio_BGM();
	Game_Audio_SFX();
}

void Game_Audio_Finalize()
{
	SM->UnInit();
}

void Game_Audio_BGM()
{
	SM->Load_BGM("Title", "Resource/BGM/Title_Ancient_memories.wav");

	SM->Load_BGM("Stage_1_BGM", "Resource/BGM/Stage_1_Tense_Tactics_Loop.wav");

	SM->Load_BGM("Stage_2_BGM", "Resource/BGM/Stage_2_Night_Howling.wav");

	SM->Load_BGM("Stage_3_BGM", "Resource/BGM/Stage_3_Archetype_Moonlight.wav");
	SM->Load_BGM("Stage_3_Boss_BGM", "Resource/BGM/Stage_3_Boss_Flash_Shadow.wav");

	SM->Load_BGM("Stage_4_BGM", "Resource/BGM/Stage_4_Disturbance.wav");
	SM->Load_BGM("Stage_4_Boss_BGM", "Resource/BGM/Stage_4_Boss_Mechanical_Circuit.wav");

	SM->Load_BGM("Stage_5_BGM", "Resource/BGM/Stage_5_Graviton_Down.wav");
	SM->Load_BGM("Stage_5_Boss_Phase_1_BGM", "Resource/BGM/Stage_5_Boss_Phase_1_The_Fourth_Tragedy.wav");
	SM->Load_BGM("Stage_5_Boss_Phase_2_BGM", "Resource/BGM/Stage_5_Boss_Phase_2_Negative_Deed.wav");

	SM->Load_BGM("Ending", "Resource/BGM/Stage_5_Boss_Phase_2_Negative_Deed.wav");
}

void Game_Audio_SFX()
{
	SM->Load_SFX("Logo_Draw", "Resource/SFX/Logo_Draw_Fixed.wav");

	SM->Load_SFX("Buffer_Move", "Resource/SFX/Buffer_Move.wav");
	SM->Load_SFX("Buffer_Select", "Resource/SFX/Buffer_Select.wav");
	SM->Load_SFX("Buffer_Back", "Resource/SFX/Buffer_Back.wav");
	SM->Load_SFX("Buffer_Denied", "Resource/SFX/Buffer_Denied.wav");
	SM->Load_SFX("Controller_Alert", "Resource/SFX/Controller_Alert.wav");

	SM->Load_SFX("Stage_Start", "Resource/SFX/Stage_Start.wav");
	SM->Load_SFX("Stage_Battle_Start", "Resource/SFX/Stage_Battle_Start.wav");
	SM->Load_SFX("Stage_Story_BG", "Resource/SFX/Stage_Story_BG.wav");
	SM->Load_SFX("Stage_Story_Enter", "Resource/SFX/Stage_Story_Enter.wav");
	SM->Load_SFX("Stage_Story_Enemy", "Resource/SFX/Stage_Story_Enemy.wav");
	SM->Load_SFX("Stage_Show_Score", "Resource/SFX/Stage_Show_Score.wav");
	SM->Load_SFX("Stage_Score_Up", "Resource/SFX/Stage_Score_Up.wav");
	SM->Load_SFX("Stage_Failed", "Resource/SFX/Stage_Failed.wav");

	SM->Load_SFX("Player_Bomb", "Resource/SFX/Player_Bomb.wav");
	SM->Load_SFX("Player_Dead", "Resource/SFX/Player_Dead.wav");

	SM->Load_SFX("Player_Get_Power", "Resource/SFX/Player_Get_Power.wav");
	SM->Load_SFX("Player_Get_Power_Max", "Resource/SFX/Player_Get_Power_Max.wav");
	SM->Load_SFX("Player_Get_Score", "Resource/SFX/Player_Get_Score.wav");
	SM->Load_SFX("Player_Ger_Bomb", "Resource/SFX/Player_Ger_Bomb.wav");
	SM->Load_SFX("Player_Get_Lives", "Resource/SFX/Player_Get_Lives.wav");

	SM->Load_SFX("Bullet_Hit", "Resource/SFX/Bullet_Hit.wav");
	SM->Load_SFX("Bullet_Shoot", "Resource/SFX/Bullet_Shoot.wav");

	SM->Load_SFX("Enemy_Dead", "Resource/SFX/Enemy_Dead.wav");
	SM->Load_SFX("Enemy_Dead_Bomb", "Resource/SFX/Enemy_Dead_Bomb.wav");
	SM->Load_SFX("Enemy_Mini_Boss_Dead", "Resource/SFX/Enemy_Mini_Boss_Dead.wav");
	SM->Load_SFX("Enemy_Boss_Dead", "Resource/SFX/Enemy_Boss_Dead.wav");
	SM->Load_SFX("Enemy_Shoot_Normal", "Resource/SFX/Enemy_Shoot_Normal.wav");
	SM->Load_SFX("Enemy_Shoot_Big", "Resource/SFX/Enemy_Shoot_Big.wav");
	SM->Load_SFX("Enemy_Shoot_Ligth", "Resource/SFX/Enemy_Shoot_Ligth.wav");
}