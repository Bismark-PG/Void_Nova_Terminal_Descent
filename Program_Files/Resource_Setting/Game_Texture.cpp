/*==============================================================================

    Management Texture Resource [Game_Texture.h]

    Author : Choi HyungJoon

==============================================================================*/
#include "Game_Texture.h"
#include "Texture_Manager.h"

void Game_Texture_Initialize()
{
    Game_Texture_Load();
}

void Game_Texture_Finalize()
{
}

void Game_Texture_Load()
{
    //------------------------------------------------Debug Texture------------------------------------------------//
    Texture_M->Load("Debug_Text"     , L"Resource/Texture/Other/Console_ASCII_512.png");
    Texture_M->Load("Debug_Collision", L"Resource/Texture/Other/Debug.png");

    //----------------------------------------------Debug BG Texture----------------------------------------------//
    Texture_M->Load("W", L"Resource/Texture/BG/RGBCMYK/White.png");
    Texture_M->Load("R", L"Resource/Texture/BG/RGBCMYK/Red.png");
    Texture_M->Load("G", L"Resource/Texture/BG/RGBCMYK/Green.png");
    Texture_M->Load("B", L"Resource/Texture/BG/RGBCMYK/Blue.png");
    Texture_M->Load("C", L"Resource/Texture/BG/RGBCMYK/Cyan.png");
    Texture_M->Load("M", L"Resource/Texture/BG/RGBCMYK/Magenta.png");
    Texture_M->Load("Y", L"Resource/Texture/BG/RGBCMYK/Yellow.png");
    Texture_M->Load("K", L"Resource/Texture/BG/RGBCMYK/Black.png");


    //-------------------------------------------<< In Game Texture >>-------------------------------------------//
    //-----------------------Player Texture-----------------------//
    Texture_M->Load("Player_Straight"  , L"Resource/Texture/Player/Player_Mk_3_Fixed_Animation_Fixed.png");
    Texture_M->Load("Player_Left"      , L"Resource/Texture/Player/Player_Mk_3_Rotate_L_Animation_Fixed.png");
    Texture_M->Load("Player_Right"     , L"Resource/Texture/Player/Player_Mk_3_Rotate_R_Animation_Fixed.png");
    Texture_M->Load("Player_Avoid_Mode", L"Resource/Texture/Player/Player_Avoid_Collision.png");

    Texture_M->Load("Player_Main_BG", L"Resource/Texture/Player/Player_Mk_3_Main_Animation_Fixed.png");
    Texture_M->Load("Player_Ending_BG", L"Resource/Texture/Player/Player_Mk_3_Main_Animation_Fixed.png");

    //-------------------Player Bullet Texture-------------------//
    Texture_M->Load("Player_Bullet", L"Resource/Texture/Player/Bullet_Power_Standard.png");
    Texture_M->Load("Player_Laser" , L"Resource/Texture/Player/Bullet_Power_Max.png");

    Texture_M->Load("Player_Bomb_Effect_Ring", L"Resource/Texture/Player/Player_Bomb.png");
    Texture_M->Load("Player_Bomb_Effect_Man" , L"Resource/Texture/Player/UI_Bomb_Man.png");

    //-----------------------Item Texture-----------------------//
    Texture_M->Load("Item_Power", L"Resource/Texture/UI/UI_Item P_Animeition.png");
    Texture_M->Load("Item_Score", L"Resource/Texture/UI/UI_Item S_Animeition.png");
    Texture_M->Load("Item_Bomb" , L"Resource/Texture/UI/UI_Item B_Animeition.png");
    Texture_M->Load("Item_Life" , L"Resource/Texture/UI/UI_Item L_Animeition.png");

    //----------------------Effects Texture----------------------//
    Texture_M->Load("Player_Explosion"    , L"Resource/Texture/Animation/Explosion_Light_1.png");
    Texture_M->Load("Enemy_Explosion"     , L"Resource/Texture/Animation/Explosion_Light_Big_Fixed.png");
    Texture_M->Load("Enemy_Real_Explosion", L"Resource/Texture/Animation/Explosion.png");

    //-----------------------Enemy Texture-----------------------//
    Texture_M->Load("Enemy_Origin"            , L"Resource/Texture/Enemy/Enemy_Original_Black_Fixed.png");
    Texture_M->Load("Enemy_Saber"             , L"Resource/Texture/Enemy/Enemy_Saber_Black.png");
    Texture_M->Load("Enemy_Spear"             , L"Resource/Texture/Enemy/Enemy_Spear_Origin.png");
    Texture_M->Load("Enemy_Armor"             , L"Resource/Texture/Enemy/Enemy_Armor.png");
    Texture_M->Load("Enemy_Stage_3_Boss"      , L"Resource/Texture/Enemy/Enemy_Middle_Original.png");
    Texture_M->Load("Enemy_Stage_4_Boss"      , L"Resource/Texture/Enemy/Enemy_Middle_2_Black.png");
    Texture_M->Load("Enemy_Final_Boss_Eyes", L"Resource/Texture/Enemy/Enemy_Boss_Eyes.png");
    Texture_M->Load("Enemy_Final_Boss_Phase_1", L"Resource/Texture/Enemy/Enemy_Boss_W_Fixed.png");
    Texture_M->Load("Enemy_Final_Boss_Phase_2", L"Resource/Texture/Enemy/Enemy_Boss_B_Fixed.png");

    //-------------------Enemy Bullet Texture-------------------//
    Texture_M->Load("Enemy_Bullet_Normal", L"Resource/Texture/Enemy/Bullet_Orange.png");
    Texture_M->Load("Enemy_Bullet_Long"  , L"Resource/Texture/Enemy/Bullet_Straight.png");
    Texture_M->Load("Enemy_Bullet_Spire"  , L"Resource/Texture/Enemy/Bullet_Spire.png");

    //----------------------------------------------<< BG Texture >>----------------------------------------------//
    //-----------------------Back Ground Texture-----------------------//
    Texture_M->Load("BG_Title" , L"Resource/Texture/BG/Main_BG_Fixed.png");
    Texture_M->Load("BG_Select", L"Resource/Texture/BG/Game_Select_BG_Fixed.png");

    //--------------------Stage Back Ground Texture--------------------//
    Texture_M->Load("Stage_1"    , L"Resource/Texture/BG/Game_Stage_1_BG_Fixed.png");
    Texture_M->Load("Stage_2"    , L"Resource/Texture/BG/Game_Stage_2_BG.png");
    Texture_M->Load("Stage_3"    , L"Resource/Texture/BG/Game_Stage_3_BG.png");
    Texture_M->Load("Stage_4"    , L"Resource/Texture/BG/Game_Stage_4_BG.png");
    Texture_M->Load("Stage_5"    , L"Resource/Texture/BG/Game_Stage_5_BG_Fixed.png");
    Texture_M->Load("Stage_Final", L"Resource/Texture/BG/Game_Stage_BG_BOSS_Anime.png");

    Texture_M->Load("Space_1", L"Resource/Texture/BG/Game_Stage_1_BG_Space.png");
    Texture_M->Load("Space_2", L"Resource/Texture/BG/Game_Stage_2_BG_Space.png");
    Texture_M->Load("Space_3", L"Resource/Texture/BG/Game_Stage_3_BG_Space.png");
    Texture_M->Load("Space_4", L"Resource/Texture/BG/Game_Stage_4_BG_Space.png");
    Texture_M->Load("Space_5", L"Resource/Texture/BG/Game_Stage_5_BG_Space.png");

    //----------------------------------------------<< UI Texture >>----------------------------------------------//
    //-----------------------UI : In Game Texture-----------------------//
    Texture_M->Load("UI_BG_Left"        , L"Resource/Texture/BG/Game_UI_L_BG_Fixed.png");
    Texture_M->Load("UI_BG_Right"       , L"Resource/Texture/BG/Game_UI_R_BG_Fixed_.png");
    Texture_M->Load("UI_BG_FullScreen_L", L"Resource/Texture/BG/Game_UI_BG_Full_Screen_L.png");
    Texture_M->Load("UI_BG_FullScreen_R", L"Resource/Texture/BG/Game_UI_BG_Full_Screen_R.png");
    Texture_M->Load("UI_Title"          , L"Resource/Texture/BG/Title_UI.png");
    
    Texture_M->Load("UI_Boss_HP"      , L"Resource/Texture/UI/UI_Boss_HP_Bar.png");
    Texture_M->Load("UI_Boss_HP_Fill" , L"Resource/Texture/UI/UI_Boss_HP_Bar_Fill.png");
    Texture_M->Load("UI_Boss_HP_Frame", L"Resource/Texture/UI/UI_Boss_HP_Bar_Frame.png");

    Texture_M->Load("UI_Text_Life" , L"Resource/Texture/UI/UI_Life.png");
    Texture_M->Load("UI_Text_Bomb" , L"Resource/Texture/UI/UI_BOMB.png");
    Texture_M->Load("UI_Text_Score", L"Resource/Texture/UI/UI_Score.png");
    Texture_M->Load("UI_Text_Power", L"Resource/Texture/UI/UI_Power.png");

    Texture_M->Load("UI_Life", L"Resource/Texture/UI/UI_Item Life.png");
    Texture_M->Load("UI_Bomb", L"Resource/Texture/UI/UI_Item Bomb.png");

    Texture_M->Load("UI_Power_Bar_Default", L"Resource/Texture/UI/UI_Power_Bar_Fixed.png");
    Texture_M->Load("UI_Power_Bar_Charge" , L"Resource/Texture/UI/UI_Power_Bar_Charging.png");
    Texture_M->Load("UI_Power_Bar_Max"    , L"Resource/Texture/UI/UI_Power_Bar_Max.png");

    //-----------------------UI : Stage Texture-----------------------//
    Texture_M->Load("UI_Alert_Deployment"   , L"Resource/Texture/BG/Stage_Start/Deployment_11.png");
    Texture_M->Load("UI_Alert_Initiated"    , L"Resource/Texture/BG/Stage_Start/Initiated_10.png");
    Texture_M->Load("UI_Alert_Mission_Start", L"Resource/Texture/BG/Stage_Start/Mission_Start_8.png");

    Texture_M->Load("UI_Alert_Anime_Deployment"   , L"Resource/Texture/BG/Stage_Start/Deployment_Animation.png");
    Texture_M->Load("UI_Alert_Anime_Initiated"    , L"Resource/Texture/BG/Stage_Start/Initiated_Anime_Fixed.png");
    Texture_M->Load("UI_Alert_Anime_Mission_Start", L"Resource/Texture/BG/Stage_Start/Mission_Start_Animation.png");

    Texture_M->Load("UI_Alert_Final_Mission_1", L"Resource/Texture/BG/Stage_Done/Boss_Ready_1.png");
    Texture_M->Load("UI_Alert_Final_Mission_2", L"Resource/Texture/BG/Stage_Done/Boss_Ready_2.png");
    Texture_M->Load("UI_Alert_Anime_Final_Mission_1", L"Resource/Texture/BG/Stage_Done/Boss_Ready_Animation_1.png");
    Texture_M->Load("UI_Alert_Anime_Final_Mission_2", L"Resource/Texture/BG/Stage_Done/Boss_Ready_Animation_2.png");

    //------------------------UI : Story Texture------------------------//
    Texture_M->Load("UI_Story_BG"      , L"Resource/Texture/Story/Story_BG_Extend.png");
    Texture_M->Load("UI_Enemy_Story_BG", L"Resource/Texture/Story/Story_BG_Enemy.png");
    
    Texture_M->Load("UI_Story_Commander_BG"   , L"Resource/Texture/Story/Story_Man_BG.png");
    Texture_M->Load("UI_Story_Commander_Intro", L"Resource/Texture/Story/Story_Man_Fixed.png");
    Texture_M->Load("UI_Story_Commander_Noise", L"Resource/Texture/Story/Story_Man_Animation_Extend_Fixed.png");

    Texture_M->Load("UI_Story_Button", L"Resource/Texture/Story/Button_Anime_Fixed.png");

    //-------------------UI : Game Over Menu Texture-------------------//
	Texture_M->Load("UI_Operation", L"Resource/Texture/UI/UI_Operation.png");

	Texture_M->Load("UI_Failed"   , L"Resource/Texture/UI/UI_Failed.png");
	Texture_M->Load("UI_Failed_JP", L"Resource/Texture/UI/UI_Failed_JP.png");

	Texture_M->Load("UI_Return"   , L"Resource/Texture/UI/Menu_Return.png");
    Texture_M->Load("UI_Return_JP", L"Resource/Texture/UI/Menu_Return_JP.png");

	Texture_M->Load("UI_Abort"   , L"Resource/Texture/UI/Menu_Abort.png");
    Texture_M->Load("UI_Abort_JP", L"Resource/Texture/UI/Menu_Abort_JP.png");

	Texture_M->Load("UI_Complete"   , L"Resource/Texture/UI/UI_Complete.png");
    Texture_M->Load("UI_Complete_JP", L"Resource/Texture/UI/UI_Complete_JP.png");

    //-----------------------UI : Number Texture-----------------------//
    Texture_M->Load("UI_Num_MIN", L"Resource/Texture/UI/Num/UI_Num_Min.png");
    Texture_M->Load("UI_Num_1"  , L"Resource/Texture/UI/Num/UI_Num_1.png");
    Texture_M->Load("UI_Num_2"  , L"Resource/Texture/UI/Num/UI_Num_2.png");
    Texture_M->Load("UI_Num_3"  , L"Resource/Texture/UI/Num/UI_Num_3.png");
    Texture_M->Load("UI_Num_4"  , L"Resource/Texture/UI/Num/UI_Num_4.png");
    Texture_M->Load("UI_Num_5"  , L"Resource/Texture/UI/Num/UI_Num_5.png");
    Texture_M->Load("UI_Num_6"  , L"Resource/Texture/UI/Num/UI_Num_6.png");
    Texture_M->Load("UI_Num_7"  , L"Resource/Texture/UI/Num/UI_Num_7.png");
    Texture_M->Load("UI_Num_8"  , L"Resource/Texture/UI/Num/UI_Num_8.png");
    Texture_M->Load("UI_Num_9"  , L"Resource/Texture/UI/Num/UI_Num_9.png");
    Texture_M->Load("UI_Num_MAX", L"Resource/Texture/UI/Num/UI_Num_Max.png");

    Texture_M->Load("UI_Num_Button_L", L"Resource/Texture/UI/Num/UI_Button_L.png");
    Texture_M->Load("UI_Num_Button_R", L"Resource/Texture/UI/Num/UI_Button_R.png");
    Texture_M->Load("UI_Score_Number", L"Resource/Texture/UI/Num/UI_Num_Animation.png");

    //------------------------UI : Menu Texture------------------------//
    Texture_M->Load("UI_Title_No_Line", L"Resource/Texture/BG/Title_Text_No_Line.png");
    Texture_M->Load("UI_Title_Line"   , L"Resource/Texture/BG/Main_Title_Line_Fixed.png");
    
    Texture_M->Load("UI_Intro_Text"           , L"Resource/Texture/BG/Title_Menu.png");
    Texture_M->Load("UI_Intro_Text_Controller", L"Resource/Texture/BG/Title_Menu_Controller.png");
    
    Texture_M->Load("UI_Menu_Start"   , L"Resource/Texture/UI/Menu_Start.png");
    Texture_M->Load("UI_Menu_Settings", L"Resource/Texture/UI/Menu_Settings.png");
    Texture_M->Load("UI_Menu_Exit"    , L"Resource/Texture/UI/Menu_Exit.png");
    
    Texture_M->Load("UI_New_Game"        , L"Resource/Texture/UI/UI_New_Game.png");
    Texture_M->Load("UI_Continue"        , L"Resource/Texture/UI/UI_Continue.png");
    Texture_M->Load("UI_Stage"           , L"Resource/Texture/UI/UI_Stage.png");
    Texture_M->Load("UI_Select"          , L"Resource/Texture/UI/UI_Select.png");
    Texture_M->Load("UI_BGM"             , L"Resource/Texture/UI/Settings_BGM.png");
    Texture_M->Load("UI_SFX"             , L"Resource/Texture/UI/Settings_SFX.png");
    Texture_M->Load("UI_Window_Mode"     , L"Resource/Texture/UI/Settings_W_Mode_Fixed.png");
    Texture_M->Load("UI_Full_Screen_Mode", L"Resource/Texture/UI/Settings_FS_Mode.png");
    Texture_M->Load("UI_Back"            , L"Resource/Texture/UI/Menu_Back_Fixed.png");
    
    //---------------------UI : Controller Texture---------------------//
    Texture_M->Load("UI_Controller_BG"         , L"Resource/Texture/Controller/Controller_Input_BG_Fixed.png");
    Texture_M->Load("UI_Controller_Input"      , L"Resource/Texture/Controller/Controller_Input_Alert_Fixed.png");
    Texture_M->Load("UI_Controller_Output"     , L"Resource/Texture/Controller/Controller_Output_Alert_Fixed.png");
    Texture_M->Load("UI_Controller_Button_Up"  , L"Resource/Texture/Controller/A_Pressed_UP.png");
    Texture_M->Load("UI_Controller_Button_Down", L"Resource/Texture/Controller/A_Pressed_Fixed.png");
    Texture_M->Load("UI_Controller_Enter_Up"   , L"Resource/Texture/Controller/Enter_Alpha.png");
    Texture_M->Load("UI_Controller_Enter_Down" , L"Resource/Texture/Controller/Enter_Alpha_Press.png");

    //--------------------------------------------<< Story Texture >>--------------------------------------------//
    //-----------------------------Stage 1-----------------------------//
    Texture_M->Load("Stage_1_1", L"Resource/Texture/Story/Script/Story_Stage_1_1.png");
    Texture_M->Load("Stage_1_2", L"Resource/Texture/Story/Script/Story_Stage_1_2.png");
    Texture_M->Load("Stage_1_3", L"Resource/Texture/Story/Script/Story_Stage_1_3.png");
    Texture_M->Load("Stage_1_4", L"Resource/Texture/Story/Script/Story_Stage_1_4.png");
    Texture_M->Load("Stage_1_5", L"Resource/Texture/Story/Script/Story_Stage_1_5.png");

    //-----------------------------Stage 2-----------------------------//
    Texture_M->Load("Stage_2_1", L"Resource/Texture/Story/Script/Story_Stage_2_1.png");
    Texture_M->Load("Stage_2_2", L"Resource/Texture/Story/Script/Story_Stage_2_2.png");
    Texture_M->Load("Stage_2_3", L"Resource/Texture/Story/Script/Story_Stage_2_3.png");
    Texture_M->Load("Stage_2_4", L"Resource/Texture/Story/Script/Story_Stage_2_4.png");
    Texture_M->Load("Stage_2_5", L"Resource/Texture/Story/Script/Story_Stage_2_5.png");
    Texture_M->Load("Stage_2_6", L"Resource/Texture/Story/Script/Story_Stage_2_6.png");
    Texture_M->Load("Stage_2_7", L"Resource/Texture/Story/Script/Story_Stage_2_7.png");

    //-----------------------------Stage 3-----------------------------//
    Texture_M->Load("Stage_3_1", L"Resource/Texture/Story/Script/Story_Stage_3_1.png");
    Texture_M->Load("Stage_3_2", L"Resource/Texture/Story/Script/Story_Stage_3_2.png");
    Texture_M->Load("Stage_3_3", L"Resource/Texture/Story/Script/Story_Stage_3_3.png");
    Texture_M->Load("Stage_3_4", L"Resource/Texture/Story/Script/Story_Stage_3_4.png");
    Texture_M->Load("Stage_3_5", L"Resource/Texture/Story/Script/Story_Stage_3_5.png");
    Texture_M->Load("Stage_3_6", L"Resource/Texture/Story/Script/Story_Stage_3_6.png");

    Texture_M->Load("Stage_3_Boss_1", L"Resource/Texture/Story/Script/Story_Stage_3_Boss_1.png");
    Texture_M->Load("Stage_3_Boss_2", L"Resource/Texture/Story/Script/Story_Stage_3_Boss_2.png");
    Texture_M->Load("Stage_3_Boss_3", L"Resource/Texture/Story/Script/Story_Stage_3_Boss_3.png");
    Texture_M->Load("Stage_3_Boss_4", L"Resource/Texture/Story/Script/Story_Stage_3_Boss_4.png");

	//-----------------------------Stage 4-----------------------------//
	Texture_M->Load("Stage_4_1", L"Resource/Texture/Story/Script/Story_Stage_4_1.png");
	Texture_M->Load("Stage_4_2", L"Resource/Texture/Story/Script/Story_Stage_4_2.png");
	Texture_M->Load("Stage_4_3", L"Resource/Texture/Story/Script/Story_Stage_4_3.png");

    Texture_M->Load("Stage_4_Boss_1", L"Resource/Texture/Story/Script/Story_Stage_4_Boss_1.png");
    Texture_M->Load("Stage_4_Boss_2", L"Resource/Texture/Story/Script/Story_Stage_4_Boss_2.png");
    Texture_M->Load("Stage_4_Boss_3", L"Resource/Texture/Story/Script/Story_Stage_4_Boss_3.png");
    Texture_M->Load("Stage_4_Boss_4", L"Resource/Texture/Story/Script/Story_Stage_4_Boss_4.png");
    Texture_M->Load("Stage_4_Boss_5", L"Resource/Texture/Story/Script/Story_Stage_4_Boss_5.png");

    //-----------------------------Stage 5-----------------------------//
	Texture_M->Load("Stage_5_1", L"Resource/Texture/Story/Script/Story_Stage_5_1.png");
	Texture_M->Load("Stage_5_2", L"Resource/Texture/Story/Script/Story_Stage_5_2.png");

    Texture_M->Load("Stage_5_Boss_1", L"Resource/Texture/Story/Script/Story_Stage_5_Boss_1.png");
    Texture_M->Load("Stage_5_Boss_2", L"Resource/Texture/Story/Script/Story_Stage_5_Boss_2.png");
    Texture_M->Load("Stage_5_Boss_3", L"Resource/Texture/Story/Script/Story_Stage_5_Boss_3.png");
    Texture_M->Load("Stage_5_Boss_4", L"Resource/Texture/Story/Script/Story_Stage_5_Boss_4.png");

    Texture_M->Load("Stage_Final_1", L"Resource/Texture/Story/Script/Story_Stage_5_Final_1.png");
    Texture_M->Load("Stage_Final_2", L"Resource/Texture/Story/Script/Story_Stage_5_Final_2.png");
    Texture_M->Load("Stage_Final_3", L"Resource/Texture/Story/Script/Story_Stage_5_Final_3.png");
    Texture_M->Load("Stage_Final_4", L"Resource/Texture/Story/Script/Story_Stage_5_Final_4.png");

    Texture_M->Load("Story_Monologue", L"Resource/Texture/Story/Script/Story_Monologue.png");

    //-------------------------------------------<< Ending Texture >>-------------------------------------------//
    Texture_M->Load("BG_Ending", L"Resource/Texture/BG/Game_BG_Ending.png");

    Texture_M->Load("Ending_Text_1" , L"Resource/Texture/Story/Script/Ending_1.png");
    Texture_M->Load("Ending_Text_2" , L"Resource/Texture/Story/Script/Ending_2.png");
    Texture_M->Load("Ending_Text_3" , L"Resource/Texture/Story/Script/Ending_3.png");
    Texture_M->Load("Ending_Text_4" , L"Resource/Texture/Story/Script/Ending_4.png");
    Texture_M->Load("Ending_Text_5" , L"Resource/Texture/Story/Script/Ending_5.png");
    Texture_M->Load("Ending_Text_6" , L"Resource/Texture/Story/Script/Ending_6.png");
    Texture_M->Load("Ending_Text_7" , L"Resource/Texture/Story/Script/Ending_7.png");
    Texture_M->Load("Ending_Text_8" , L"Resource/Texture/Story/Script/Ending_8.png");
    Texture_M->Load("Ending_Text_9" , L"Resource/Texture/Story/Script/Ending_9.png");
    Texture_M->Load("Ending_Text_10" , L"Resource/Texture/Story/Script/Ending_10.png");
    Texture_M->Load("Ending_Text_Commander" , L"Resource/Texture/Story/Script/Ending_Commander.png");
    Texture_M->Load("Ending_Text_Thanks" , L"Resource/Texture/Story/Script/Ending_Thanks.png");

    Texture_M->Load("Ending_Base" , L"Resource/Texture/Story/Ending_Base.png");

    //-------------------------------------------------Others Texture-------------------------------------------------//
    Texture_M->Load("Pixel_Withe" , L"Resource/Texture/Other/W_Pixel.png");
    Texture_M->Load("Pixel_Black" , L"Resource/Texture/Other/B_Pixel.png");
    Texture_M->Load("Bismark_LOGO", L"Resource/Texture/Other/Bismark_Black.jpg");
    Texture_M->Load("Bismark_Name", L"Resource/Texture/Other/Name_W.png");

    //Texture_DB["Debug_Collision"] = Texture_M->Load;
}