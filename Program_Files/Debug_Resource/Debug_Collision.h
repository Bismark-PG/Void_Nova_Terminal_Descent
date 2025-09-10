/*==============================================================================

	Debug Draw Collision [Debug_Collision.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef DEBUG_COLLISION_H
#define DEBUG_COLLISION_H
#include "Collision.h"
#include <DirectXMath.h>

void Debug_Collision_Initialize();

void Debug_Collision_Finalize();

void Debug_Collision_Draw(const Circle& circle, const DirectX::XMFLOAT4& color);

#endif // DEBUG_COLLISION_H
