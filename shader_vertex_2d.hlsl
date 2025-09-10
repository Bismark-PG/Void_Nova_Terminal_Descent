/*==============================================================================

    2D�`��p���_�V�F�[�_�[ [shader_vertex_2d.hlsl]

    Author : Choi HyungJoon

==============================================================================*/

// �萔�o�b�t�@
cbuffer VS_CONSTANT_BUFFER : register(b0)
{
    float4x4 Projection;
};

cbuffer VS_CONSTANT_BUFFER : register(b1)
{
    float4x4 World;
};

struct VS_IN
{
    float4 posL  : POSITION0;
    float4 color : COLOR0;
    float2 UV    : TEXCOORD0;
};

struct VS_OUT
{
    float4 posH  : SV_POSITION;
    float4 color : COLOR0;
    float2 UV    : TEXCOORD0;
};

// ���_�V�F�[�_
VS_OUT main(VS_IN vi)
{
    VS_OUT vo;
    
    float4x4 mtx = mul(World, Projection); // Same, p
    vo.posH = mul(vi.posL, mtx); // Change Position

    vo.color = vi.color;
    
    vo.UV = vi.UV;
    
    return vo;
}
