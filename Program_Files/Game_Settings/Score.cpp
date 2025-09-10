/*==============================================================================

	Game Logic [Score.h]

	Author : Choi HyungJoon

==============================================================================*/
#include "score.h"
#include "sprite.h"
#include "Texture.h"
#include "Game_Window.h"
#include "Palette.h"    
#include <string>       
#include <algorithm>   
#include "Game_Back.h"
#include "Stage_Select.h"
#include "Save_Data.h"

using namespace PALETTE;

static int Digit = 0;
static int Counter_Stop = 1;

static double Time_Score_Timer = 0.0;
static constexpr double TIME_SCORE_INTERVAL = 0.1;

static bool Score_Set_Done = false;
static bool Score_Increase_Done = false;

static unsigned int Stage_Start_Total_Score = 0;
static unsigned int Stage_End_Total_Score = 0;
static unsigned int Display_Outro_Score = 0;

void Score_Initialize()
{
    Score_Reset_All_Score();

    for (int i = 0; i < 5; ++i)
        Stage_High_Scores[i] = Save_Data_Get_High_Score(i);

}

void Score_Finalize()
{
    Score_Reset_All_Score();
}

void Score_Update(double elapsed_time)
{
    if (View_Score < Score)
    {
        unsigned int Difference = Score - View_Score;
        unsigned int Increase = (Difference / 10) + 1;
        View_Score = std::min(Score, View_Score + Increase);
    }

    Time_Score_Timer += elapsed_time;
    if (Time_Score_Timer >= TIME_SCORE_INTERVAL)
    {
        Score += 1;
        Time_Score_Timer -= TIME_SCORE_INTERVAL;
    }
}

void Score_Increase(int Increase)
{
    Score += Increase;
}

void Score_Draw()
{
    std::string Score_To_Str = std::to_string(View_Score);
    std::string Now_Score = std::string(SCORE_DIGITS - Score_To_Str.length(), '0') + Score_To_Str;

    for (int i = 0; i < SCORE_DIGITS; ++i)
    {
        int Num = Now_Score[i] - '0';
        Draw_Number(Num, i, A_T_Quarters);
    }
}

void Score_End_Score_Draw(float Draw_X, float Draw_Y, float Draw_W, float Draw_H, float Alpha)
{
    std::string Score_To_Str = std::to_string(Display_Outro_Score);
    std::string Now_Score = std::string(SCORE_DIGITS - Score_To_Str.length(), '0') + Score_To_Str;

    for (int i = 0; i < SCORE_DIGITS; ++i)
    {
        int Num = Now_Score[i] - '0';
        Draw_Number(Draw_X, Draw_Y, Draw_W, Draw_H, Num, true, i, Alpha);
    }
}

void Score_Outro_Update(double elapsed_time)
{
    std::stringstream Debug;

    if (!Score_Set_Done)
    {
        Now_Stage = static_cast<int>(Get_Now_Playing_Stage()) - 1;

        Stage_Start_Total_Score = 0;

        if (Now_Stage > 0)
        {
            for (int i = 0; i < Now_Stage; ++i)
            {
                Stage_Start_Total_Score += Stage_Now_Scores[i];

                Debug << "Now Stage : " << i << std::endl;
                Debug << "Score : " << Stage_Now_Scores[Score] << "\n" << std::endl;
                Debug << "Now Total Score : " << Stage_Start_Total_Score << "\n" << std::endl;
            }
        }

        Stage_End_Total_Score = Score_Get_Total_Score();
        Display_Outro_Score = Stage_Start_Total_Score;
        Score_Set_Done = true;
        Score_Increase_Done = false;

        Debug << "Get Total Score : " << Stage_End_Total_Score << std::endl;
    }

    if (Display_Outro_Score < Stage_End_Total_Score)
    {
        unsigned int Get_Score = Stage_End_Total_Score - Display_Outro_Score;
        unsigned int Increase_Amount = std::max(1u, Get_Score / 25) + 1;
        Display_Outro_Score += Increase_Amount;

        Debug << "Score Up : " << Display_Outro_Score << std::endl;

        if (Display_Outro_Score >= Stage_End_Total_Score)
        {
            Display_Outro_Score = Stage_End_Total_Score;
            Score_Increase_Done = true;
        }
    }
    else
        Score_Increase_Done = true;

    OutputDebugStringA(Debug.str().c_str());
}

bool Score_Is_Increase_Done()
{
    return Score_Increase_Done;
}

void Score_Reset()
{
    Score = 0;
    View_Score = 0;
    Time_Score_Timer = 0.0;

    Now_Stage = -1;
    Now_Stage_Score = 0;
    Score_Outro_Reset();
}

void Score_Reset_All_Score()
{
    Score_Reset();

    for (int i = 0; i < 5; ++i)
        Stage_Now_Scores[i] = 0;
}

void Score_Outro_Reset()
{
    Score_Set_Done = false;
    Score_Increase_Done = false;
}

void Score_Reset_Current_Stage_Score()
{
    int stageIndex = static_cast<int>(Get_Now_Playing_Stage()) - 1;
    if (stageIndex >= 0 && stageIndex < 5)
    {
        Stage_Now_Scores[stageIndex] = 0;
    }
    Score_Reset();
}

void Score_Set_Score()
{
    NOW_PLAYING_STAGE Stage = Get_Now_Playing_Stage();
    int Stage_Index = static_cast<int>(Stage) - 1;

    if (Stage_Index >= 0 && Stage_Index < 5)
    {
        Stage_Now_Scores[Stage_Index] = Score;

        Save_Data_Update_High_Score(Stage_Index, Score);

        if (Stage_High_Scores[Stage_Index] < Score)
            Stage_High_Scores[Stage_Index] = Score;

        std::stringstream Debug;
        Debug << "Now Score : " << Score << "\t";
        Debug << "Stage : " << Stage_Index << std::endl;
        OutputDebugStringA(Debug.str().c_str());
    }
    Score_Reset();
}


unsigned int Score_Get_Total_Score()
{
    unsigned int Now_Total = 0;

    for (int i = 0; i < 5; ++i)
        Now_Total += Stage_Now_Scores[i];

    return Now_Total;
}

unsigned int Score_Get_High_Score()
{
    unsigned int High_Total = 0;

    for (int i = 0; i < 5; ++i)
        High_Total += Stage_High_Scores[i];

    return High_Total;
}