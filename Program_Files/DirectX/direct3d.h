/*==============================================================================

	Reset Direct3D [direct3d.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef DIRECT3D_H
#define DIRECT3D_H


#include <Windows.h>
#include <d3d11.h>


// �Z�[�t�����[�X�}�N��
#define SAFE_RELEASE(o) if (o) { (o)->Release(); o = NULL; }


bool Direct3D_Initialize(HWND hWnd); // Direct3D�̏�����
void Direct3D_Finalize(); // Direct3D�̏I������

void Direct3D_Clear(); // �o�b�N�o�b�t�@�̃N���A
void Direct3D_Present(); // �o�b�N�o�b�t�@�̕\��

unsigned int Direct3D_GetBackBufferWidth();
unsigned int Direct3D_GetBackBufferHeight();

//Earn D3D Device
ID3D11Device* Direct3D_GetDevice();
//Earn D3D Context
ID3D11DeviceContext* Direct3D_GetContext();

void Direct3D_Re_Size(UINT width, UINT height);

#endif // DIRECT3D_H
