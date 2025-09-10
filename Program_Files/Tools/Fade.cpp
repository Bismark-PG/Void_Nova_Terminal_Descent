/*==============================================================================

    Fade In Out [Fade.h]

    Author : Choi HyungJoon

==============================================================================*/
#include "Fade.h"
#include <iostream>

double FadeTime = 0.0;
double FadeStartTime = 0.0;
double AccumulatedTime = 0.0;

FADE_STATE State = FADE_STATE::NONE;
static int FadeTexId = -1;

static float Fade_Alpha = 0.0f;
static Palette Color = Black;

static bool IsDoneFadeOut = false;

void Fade_Initialize(void)
{
   FadeTime = 1.0;
   FadeStartTime = 0.0;
   AccumulatedTime = 0.0;
   Color = Black;
   Fade_Alpha = 0.0f;
   State = FADE_STATE::NONE;

   FadeTexId = Texture_Load(L"Resource/Texture/BG/RGBCMYK/White.png");
}

void Fade_Finalize(void)
{
}

void Fade_Update(double elapsed_time)
{
#if defined(DEBUG) || defined(_DEBUG)
    std::stringstream Debug;
    Debug << "[Fade_Update] State : " << (int)State << std::endl;
    Debug << "Accumulated Time : " << AccumulatedTime << std::endl;
    Debug << "Start Time : " << FadeStartTime << std::endl;
#endif

    if (State == FADE_STATE::FINISHED_IN || State == FADE_STATE::FINISHED_OUT) return;
    if (State == FADE_STATE::NONE) return;

    AccumulatedTime += elapsed_time;

    double Progress = min((AccumulatedTime - FadeStartTime) / FadeTime, 1.0);

    if (Progress >= 1.0)
    {
#if defined(DEBUG) || defined(_DEBUG)
        Debug << "Fade Finished! Previous State : " << (int)State << std::endl;
#endif
        State = State == FADE_STATE::FADE_IN ? FADE_STATE::FINISHED_IN : FADE_STATE::FINISHED_OUT;
#if defined(DEBUG) || defined(_DEBUG)
        Debug << "New State : " << (int)State << std::endl;
#endif
    }

    Fade_Alpha = (float)(State == FADE_STATE::FADE_IN ? 1.0 - Progress : Progress);

#if defined(DEBUG) || defined(_DEBUG)
    OutputDebugStringA(Debug.str().c_str());
#endif
}

void Fade_Draw(void)
{
    if (State == FADE_STATE::NONE || State == FADE_STATE::FINISHED_IN)
          return;

    if (State == FADE_STATE::FINISHED_OUT)
    {
        if (!IsDoneFadeOut)
        {
            Palette F_Color = Color;
            F_Color.w = Fade_Alpha;
            Sprite_Draw(FadeTexId, 0.0f, 0.0f, (float)Direct3D_GetBackBufferWidth(), (float)Direct3D_GetBackBufferHeight(), 0.0f, F_Color);
            IsDoneFadeOut = true;
        }
        return;
    }

    Palette F_Color = Color;
    F_Color.w = Fade_Alpha;
    Sprite_Draw(FadeTexId, 0.0f, 0.0f, (float)Direct3D_GetBackBufferWidth(), (float)Direct3D_GetBackBufferHeight(), 0.0f, F_Color);
}

// True = Fade Out, False = Fade In
void Fade_Start(double Duration, bool IsFadeOut, Palette color)
{
    FadeStartTime = AccumulatedTime;
    FadeTime = Duration;
    State = IsFadeOut ? FADE_STATE::FADE_OUT : FADE_STATE::FADE_IN;
    Color = color;
    Fade_Alpha = IsFadeOut ? 0.0f : 1.0f;
}

FADE_STATE Fade_GetState(void)
{
	return State;
}
