/*==============================================================================

	Effect Draw [Effect.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef EFFECT_H
#define EFFECT_H

#include <DirectXMath.h>

enum class Effect_Type
{
	PLAYER_EXPLOSION,
	ENEMY_EXPLOSION,
	REAL_EXPLOSION,
};

void Effect_Initialize();
void Effect_Finalize();

void Effect_Update(double elapsed_time);
void Effect_Draw();

void Effect_Create(Effect_Type type, const DirectX::XMFLOAT2& position, const DirectX::XMFLOAT2& size);

#endif // EFFECT_H