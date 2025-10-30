/*==============================================================================

    Management 2D And 3D Shader Resources [Shader_Manager.h]

    Author : Choi HyungJoon

==============================================================================*/
#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

enum class Shader_Filter
{
    MAG_MIP_POINT,   // No filtering (Pixelated look)
    MAG_MIP_LINEAR,  // Smooth bilinear filtering
    ANISOTROPIC      // High-quality filtering for textures at sharp angles
};

struct Directional_Light
{
    alignas(16) DirectX::XMFLOAT4 Direction;
    alignas(16) DirectX::XMFLOAT4 Color;
};

class Shader_Manager
{
public:
    static Shader_Manager* Get_Shader_Instance();

    Shader_Manager(const Shader_Manager&) = delete;
    Shader_Manager& operator=(const Shader_Manager&) = delete;

    // Initializes all shaders
    bool Init(ID3D11Device* device, ID3D11DeviceContext* context);
    void Final();

    // --- Methods for 2D Shader ---
    void Begin2D(Shader_Filter Filter = Shader_Filter::MAG_MIP_POINT);
    void SetWorldMatrix2D(const DirectX::XMMATRIX& matrix);
    void SetProjectionMatrix2D(const DirectX::XMMATRIX& matrix);
    void SetTexture2D(ID3D11ShaderResourceView* textureView);


    // --- Methods for 3D Shader ---
    // void Begin3D(Shader_Filter Filter = Shader_Filter::ANISOTROPIC);
    // void Begin3D_For_Field(Shader_Filter Filter = Shader_Filter::ANISOTROPIC);
    // void SetWorldMatrix3D(const DirectX::XMMATRIX& matrix);
    // void SetViewMatrix3D(const DirectX::XMMATRIX& matrix);
    // void SetProjectionMatrix3D(const DirectX::XMMATRIX& matrix);
    // void SetTexture3D(ID3D11ShaderResourceView* textureView);
    // void SetFieldTextures(ID3D11ShaderResourceView* texture0, ID3D11ShaderResourceView* texture1); // For Multy Texture
    
    // void SetLightAmbient(const DirectX::XMFLOAT4& color);
    // void SetLightDirectional(const DirectX::XMFLOAT4& worldDirection, const DirectX::XMFLOAT4& color);

private:
    Shader_Manager() = default;
    ~Shader_Manager() = default;

    // Helper method to load shaders from .cso files
    bool loadVertexShader(const char* filename,
        Microsoft::WRL::ComPtr<ID3D11VertexShader>& vs,
        Microsoft::WRL::ComPtr<ID3D11InputLayout>& il,
        const D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements);

    bool loadPixelShader(const char* filename, Microsoft::WRL::ComPtr<ID3D11PixelShader>& ps);

    // Common device interfaces
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_context = nullptr;

    // --- 2D Shader Resources ---
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs2D;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_il2D;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_ps2D;
    Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cbProjection2D; // b0 for projection matrix
    Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cbWorld2D;      // b1 for world matrix

    // --- 3D Shader Resources ---
    // Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vs3D;
    // Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_il3D;
    // Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_ps3D;
    // Microsoft::WRL::ComPtr<ID3D11PixelShader>  m_ps3D_Field;      // Pixel Shader For Field
    // Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cbWorld3D;       // b0 for world
    // Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cbView3D;        // b1 for view
    // Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cbProjection3D;  // b2 for projection
    // Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cbAmbient3D;     // b3 for Ambient Color
    // Microsoft::WRL::ComPtr<ID3D11Buffer>       m_cbDirectional3D; // b4 for World Directional

    // Sampler states for different filtering modes
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_Point;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_Linear;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampler_AnisoTropic;
};

extern Shader_Manager* Shader_M;

#endif // SHADER_MANAGER_H

