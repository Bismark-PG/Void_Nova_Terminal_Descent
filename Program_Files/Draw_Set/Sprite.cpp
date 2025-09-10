/*==============================================================================

	Draw Sprite [Sprite.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include <d3d11.h>
#include <DirectXMath.h>
#include "DirectXTex.h"
using namespace DirectX;
#include "direct3d.h"
#include "shader.h"	
#include "debug_ostream.h"
#include "Sprite.h"
#include "Texture.h"

static constexpr int NUM_VERTEX = 4; // ���_��

static ID3D11Buffer* g_pVertexBuffer = nullptr; // ���_�o�b�t�@
static ID3D11ShaderResourceView* g_pTexture = nullptr;

// ���ӁI�������ŊO������ݒ肳�����́BRelease�s�v�B
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pContext = nullptr;


// ���_�\����
struct Vertex
{
	XMFLOAT3 position; // ���_���W
	XMFLOAT4 color;
	XMFLOAT2 UV;
};

void Sprite_Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̃`�F�b�N
	if (!pDevice || !pContext) {
		Debug::D_Out << "Sprite_Initialize() : �^����ꂽ�f�o�C�X���R���e�L�X�g���s���ł�" << std::endl;
		return;
	}

	// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g�̕ۑ�
	g_pDevice = pDevice;
	g_pContext = pContext;

	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * NUM_VERTEX;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g_pDevice->CreateBuffer(&bd, NULL, &g_pVertexBuffer);
}

void Sprite_Finalize(void)
{
	SAFE_RELEASE(g_pTexture);
	SAFE_RELEASE(g_pVertexBuffer);
}

void Sprite_Begin()
{	
	// ���_������������
	// Make Camera (Zoom In or Out, More etc...)
	const float SCREEN_WIDTH = (float)Direct3D_GetBackBufferWidth();
	const float SCREEN_HEIGHT = (float)Direct3D_GetBackBufferHeight();

	// ���_�V�F�[�_�[�ɕϊ��s���ݒ�
	Shader_SetProjectionMatrix(XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f));
}

// Show All Texture
void Sprite_Draw(int Tex_ID, float dx, float dy, const DirectX::XMFLOAT4& color)
{
	// �V�F�[�_�[��`��p�C�v���C���ɐݒ�
	Shader_Begin();

	// Auto Reset
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// ���_�o�b�t�@�����b�N����
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// ���_�o�b�t�@�ւ̉��z�|�C���^���擾
	Vertex* v = (Vertex*)msr.pData;

	// ���_������������
	unsigned int dw = Texture_Width(Tex_ID);
	unsigned int dh = Texture_Height(Tex_ID);

	// ��ʂ̍��ォ��E���Ɍ�����������`�悷��
	v[0].position = { dx,		dy,      0.0f };
	v[1].position = { dx + dw,	dy,      0.0f };
	v[2].position = { dx,		dy + dh, 0.0f };
	v[3].position = { dx + dw,	dy + dh, 0.0f };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	//	U = Width	||   V = Height
	v[0].UV = { 0.0f, 0.0f };
	v[1].UV = { 1.0f, 0.0f };
	v[2].UV = { 0.0f, 1.0f };
	v[3].UV = { 1.0f, 1.0f };

	// ���_�o�b�t�@�̃��b�N������
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Setting Texture
	Texture_SetTexture(Tex_ID);

	// �|���S���`�施�ߔ��s
	g_pContext->Draw(NUM_VERTEX, 0);
}

// Show All Texture (Can Change Size)
void Sprite_Draw(int Tex_ID, float dx, float dy, float dw, float dh, float angle, const DirectX::XMFLOAT4& color)
{
	// �V�F�[�_�[��`��p�C�v���C���ɐݒ�
	Shader_Begin();

	// ���_�o�b�t�@�����b�N����
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// ���_�o�b�t�@�ւ̉��z�|�C���^���擾
	Vertex* v = (Vertex*)msr.pData;

	// ���_������������
	// ��ʂ̍��ォ��E���Ɍ�����������`�悷��
	v[0].position = { -0.5f,	-0.5f,	0.0f }; // ����
	v[1].position = { +0.5f,	-0.5f,	0.0f }; // �E��
	v[2].position = { -0.5f,	+0.5f,	0.0f }; // ����
	v[3].position = { +0.5f,	+0.5f,	0.0f }; // �E��

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	//	U = Width	||   V = Height
	v[0].UV = { 0.0f, 0.0f };
	v[1].UV = { 1.0f, 0.0f };
	v[2].UV = { 0.0f, 1.0f };
	v[3].UV = { 1.0f, 1.0f };

	// ���_�o�b�t�@�̃��b�N������
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// Rotation Shader Setting
	XMMATRIX Scale = XMMatrixScaling(dw, dh, 1.0f); // Scale
	XMMATRIX Rotation = XMMatrixRotationZ(angle); // Rotation
	XMMATRIX Translation = XMMatrixTranslation(dx + (dw / 2), dy + (dh / 2), 0.0f); // ���s�ړ�

	Shader_SetWorldMatrix(Scale * Rotation * Translation);

	// Auto Reset
	// If You Want Rotate Or Move Many Texture Use This In Main
	// Shader_SetWorldMatrix(XMMatrixIdentity());

	// ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Setting Texture
	Texture_SetTexture(Tex_ID);

	// �|���S���`�施�ߔ��s
	g_pContext->Draw(NUM_VERTEX, 0);
}

// Cut UV
void Sprite_Draw(int Tex_ID, float dx, float dy,
				 int px, int py, int pw, int ph, const DirectX::XMFLOAT4& color)
{
	// �V�F�[�_�[��`��p�C�v���C���ɐݒ�
	Shader_Begin();

	// Auto Reset
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// ���_�o�b�t�@�����b�N����
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// ���_�o�b�t�@�ւ̉��z�|�C���^���擾
	Vertex* v = (Vertex*)msr.pData;

	// ���_������������

	// ��ʂ̍��ォ��E���Ɍ�����������`�悷��
	v[0].position = { dx,		dy,      0.0f };
	v[1].position = { dx + pw,	dy,      0.0f };
	v[2].position = { dx,		dy + ph, 0.0f };
	v[3].position = { dx + pw,	dy + ph, 0.0f };

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	// Get Texture Size
	float tw = (float)Texture_Width(Tex_ID);
	float th = (float)Texture_Height(Tex_ID);

	// Set UV Size
	float U0 = px / tw;
	float U1 = (px + pw) / th;
	float V0 = py / tw;
	float V1 = (py + ph) / th;

	// Cut Texture With Using UV Tool
	//	U = Width	||   V = Height
	v[0].UV = { U0, V0 };
	v[1].UV = { U1, V0 };
	v[2].UV = { U0, V1 };
	v[3].UV = { U1, V1 };

	// ���_�o�b�t�@�̃��b�N������
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Setting Texture
	Texture_SetTexture(Tex_ID);

	// �|���S���`�施�ߔ��s
	g_pContext->Draw(NUM_VERTEX, 0);
}

// Cut UV (Change Texture Size)
void Sprite_Draw(int Tex_ID, float dx, float dy, float dw, float dh,
	int px, int py, int pw, int ph, const DirectX::XMFLOAT4& color)
{
	// �V�F�[�_�[��`��p�C�v���C���ɐݒ�
	Shader_Begin();

	// Auto Reset
	Shader_SetWorldMatrix(XMMatrixIdentity());

	// ���_�o�b�t�@�����b�N����
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// ���_�o�b�t�@�ւ̉��z�|�C���^���擾
	Vertex* v = (Vertex*)msr.pData;

	// ��ʂ̍��ォ��E���Ɍ�����������`�悷��
	v[0].position = { dx,		dy,      0.0f }; // ����
	v[1].position = { dx + dw,	dy,      0.0f }; // �E��
	v[2].position = { dx,		dy + dh, 0.0f }; // ����
	v[3].position = { dx + dw,	dy + dh, 0.0f }; // �E��

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	// Get Texture Size
	float tw = (float)Texture_Width(Tex_ID);
	float th = (float)Texture_Height(Tex_ID);

	// Set UV Size
	float U0 = px / tw;
	float U1 = (px + pw) / tw;
	float V0 = py / th;
	float V1 = (py + ph) / th;

	// Cut Texture With Using UV Tool
	//	U = Width	||   V = Height
	v[0].UV = { U0, V0 };
	v[1].UV = { U1, V0 };
	v[2].UV = { U0, V1 };
	v[3].UV = { U1, V1 };

	// ���_�o�b�t�@�̃��b�N������
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Setting Texture
	Texture_SetTexture(Tex_ID);

	// �|���S���`�施�ߔ��s
	g_pContext->Draw(NUM_VERTEX, 0);
}

// Rotation
void Sprite_Draw(int Tex_ID, float dx, float dy, float dw, float dh,
	float px, float py, float pw, float ph, float angle, const DirectX::XMFLOAT4& color)
{
	// �V�F�[�_�[��`��p�C�v���C���ɐݒ�
	Shader_Begin();

	// ���_�o�b�t�@�����b�N����
	D3D11_MAPPED_SUBRESOURCE msr;
	g_pContext->Map(g_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	// ���_�o�b�t�@�ւ̉��z�|�C���^���擾
	Vertex* v = (Vertex*)msr.pData;

	// ��ʂ̍��ォ��E���Ɍ�����������`�悷��
	v[0].position = { -0.5f,	-0.5f,	0.0f }; // ����
	v[1].position = { +0.5f,	-0.5f,	0.0f }; // �E��
	v[2].position = { -0.5f,	+0.5f,	0.0f }; // ����
	v[3].position = { +0.5f,	+0.5f,	0.0f }; // �E��

	v[0].color = color;
	v[1].color = color;
	v[2].color = color;
	v[3].color = color;

	// Get Texture Size
	float tw = (float)Texture_Width(Tex_ID);
	float th = (float)Texture_Height(Tex_ID);

	// Set UV Size
	float U0 = px / tw;
	float U1 = (px + pw) / tw;
	float V0 = py / th;
	float V1 = (py + ph) / th;

	// Cut Texture With Using UV Tool
	//	U = Width	||   V = Height
	v[0].UV = { U0, V0 };
	v[1].UV = { U1, V0 };
	v[2].UV = { U0, V1 };
	v[3].UV = { U1, V1 };

	// ���_�o�b�t�@�̃��b�N������
	g_pContext->Unmap(g_pVertexBuffer, 0);

	// Rotation Shader Setting
	XMMATRIX Scale = XMMatrixScaling(dw, dh, 1.0f); // Scale
	XMMATRIX Rotation = XMMatrixRotationZ(angle); // Rotation
	XMMATRIX Translation = XMMatrixTranslation(dx + (dw / 2), dy + (dh / 2), 0.0f); // ���s�ړ�

	Shader_SetWorldMatrix(Scale * Rotation * Translation);

	// Auto Reset
	// If You Want Rotate Or Move Many Texture Use This In Main
	// Shader_SetWorldMatrix(XMMatrixIdentity());

	// ���_�o�b�t�@��`��p�C�v���C���ɐݒ�
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g_pContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// �v���~�e�B�u�g�|���W�ݒ�
	g_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Setting Texture
	Texture_SetTexture(Tex_ID);

	// �|���S���`�施�ߔ��s
	g_pContext->Draw(NUM_VERTEX, 0);
}