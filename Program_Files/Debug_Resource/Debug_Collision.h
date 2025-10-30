/*==============================================================================

	Manage Debug Collision [Debug_Collision.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef DEBUG_SETTINGS_H
#define DEBUG_SETTINGS_H
#include "Collision.h"
#include <DirectXMath.h>

void Debug_Initialize();
void Debug_Collision_Draw(const Circle& circle, const DirectX::XMFLOAT4& color);

#endif // DEBUG_SETTINGS_H
