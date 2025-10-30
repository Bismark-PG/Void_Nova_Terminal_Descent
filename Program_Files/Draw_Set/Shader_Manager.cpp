/*==============================================================================

    Management 2D And 3D Shader Resources [Shader_Manager.cpp]

    Author : Choi HyungJoon

    - Note
        > Make With Singleton Pattern
        > Function Type  : Pointer

==============================================================================*/
#include "Shader_Manager.h"
#include "Debug_ostream.h"
#include <fstream>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;

Shader_Manager* Shader_Manager::Get_Shader_Instance()
{
    static Shader_Manager instance;
    return &instance;
}

bool Shader_Manager::Init(ID3D11Device* device, ID3D11DeviceContext* context)
{
    m_device = device;
    m_context = context;

    // --- Initialize 2D Shaders ---
    D3D11_INPUT_ELEMENT_DESC layout2D[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    if (!loadVertexShader("Resource/Shader/shader_vertex_2d.cso", m_vs2D, m_il2D, layout2D, ARRAYSIZE(layout2D))) return false;
    if (!loadPixelShader("Resource/Shader/shader_pixel_2d.cso", m_ps2D)) return false;

    // --- Initialize 3D Shaders ---
    // D3D11_INPUT_ELEMENT_DESC layout3D[] = {
    //     { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //     { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //     { "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    //     { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    // };
    // if (!loadVertexShader("Resource/Shader/shader_vertex_3d.cso", m_vs3D, m_il3D, layout3D, ARRAYSIZE(layout3D))) return false;
    // if (!loadPixelShader("Resource/Shader/shader_pixel_3d.cso", m_ps3D)) return false;
    // if (!loadPixelShader("Resource/Shader/shader_pixel_field.cso", m_ps3D_Field)) return false;

    // --- Create Constant Buffers ---
    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.ByteWidth = sizeof(XMFLOAT4X4);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.Usage = D3D11_USAGE_DEFAULT; // If Update Every Frame Change To DYNAMIC

    m_device->CreateBuffer(&cbDesc, nullptr, &m_cbProjection2D);
    m_device->CreateBuffer(&cbDesc, nullptr, &m_cbWorld2D);

    // m_device->CreateBuffer(&cbDesc, nullptr, &m_cbWorld3D);
    // m_device->CreateBuffer(&cbDesc, nullptr, &m_cbView3D);
    // m_device->CreateBuffer(&cbDesc, nullptr, &m_cbProjection3D);

    // --- Create Ligths Buffers ---
    // cbDesc.ByteWidth = sizeof(XMFLOAT4);
    // m_device->CreateBuffer(&cbDesc, nullptr, &m_cbAmbient3D);
    // cbDesc.ByteWidth = sizeof(Directional_Light);
    // m_device->CreateBuffer(&cbDesc, nullptr, &m_cbDirectional3D);

    // --- Create Sampler States ---
    D3D11_SAMPLER_DESC samplerDesc = {};

    // --- 2D Shaders ---
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samplerDesc.BorderColor[0] = 0.0f;
    samplerDesc.BorderColor[1] = 0.0f;
    samplerDesc.BorderColor[2] = 0.0f;
    samplerDesc.BorderColor[3] = 0.0f;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = 8;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    // --- 3D Shaders ---
    //samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    //samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    //samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    //samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    //samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    //samplerDesc.MaxAnisotropy = 16;

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    m_device->CreateSamplerState(&samplerDesc, &m_sampler_Point);

    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    m_device->CreateSamplerState(&samplerDesc, &m_sampler_Linear);

    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    m_device->CreateSamplerState(&samplerDesc, &m_sampler_AnisoTropic);

    return true;
}

void Shader_Manager::Final()
{
}

// --- 2D Methods ---
void Shader_Manager::Begin2D(Shader_Filter Filter)
{
    m_context->VSSetShader(m_vs2D.Get(), nullptr, 0);
    m_context->PSSetShader(m_ps2D.Get(), nullptr, 0);
    m_context->IASetInputLayout(m_il2D.Get());

    ID3D11Buffer* cbs[] = { m_cbProjection2D.Get(), m_cbWorld2D.Get() };
    m_context->VSSetConstantBuffers(0, 2, cbs);

    switch (Filter)
    {
    case Shader_Filter::MAG_MIP_POINT:
        m_context->PSSetSamplers(0, 1, m_sampler_Point.GetAddressOf());
        break;

    case Shader_Filter::MAG_MIP_LINEAR:
        m_context->PSSetSamplers(0, 1, m_sampler_Linear.GetAddressOf());
        break;

    case Shader_Filter::ANISOTROPIC:
        m_context->PSSetSamplers(0, 1, m_sampler_AnisoTropic.GetAddressOf());
        break;
    }
}

void Shader_Manager::SetProjectionMatrix2D(const XMMATRIX& matrix)
{
    XMFLOAT4X4 transpose;
    XMStoreFloat4x4(&transpose, XMMatrixTranspose(matrix));
    m_context->UpdateSubresource(m_cbProjection2D.Get(), 0, nullptr, &transpose, 0, 0);
}

void Shader_Manager::SetWorldMatrix2D(const XMMATRIX& matrix)
{
    XMFLOAT4X4 transpose;
    XMStoreFloat4x4(&transpose, XMMatrixTranspose(matrix));
    m_context->UpdateSubresource(m_cbWorld2D.Get(), 0, nullptr, &transpose, 0, 0);
}

void Shader_Manager::SetTexture2D(ID3D11ShaderResourceView* textureView)
{
    m_context->PSSetShaderResources(0, 1, &textureView);
}

// --- 3D Methods ---
// void Shader_Manager::Begin3D(Shader_Filter Filter)
// {
//     m_context->VSSetShader(m_vs3D.Get(), nullptr, 0);
//     m_context->PSSetShader(m_ps3D.Get(), nullptr, 0);  
//     m_context->IASetInputLayout(m_il3D.Get());
// 
//     ID3D11Buffer* cbs[] = { m_cbWorld3D.Get(), m_cbView3D.Get(), m_cbProjection3D.Get(),
//         m_cbAmbient3D.Get(), m_cbDirectional3D.Get() };
//     m_context->VSSetConstantBuffers(0, 5, cbs);
// 
//     switch (Filter)
//     {
//     case Shader_Filter::MAG_MIP_POINT:
//         m_context->PSSetSamplers(0, 1, m_sampler_Point.GetAddressOf());
//         break;
// 
//     case Shader_Filter::MAG_MIP_LINEAR:
//         m_context->PSSetSamplers(0, 1, m_sampler_Linear.GetAddressOf());
//         break;
// 
//     case Shader_Filter::ANISOTROPIC:
//         m_context->PSSetSamplers(0, 1, m_sampler_AnisoTropic.GetAddressOf());
//         break;
//     }
// }

// void Shader_Manager::Begin3D_For_Field(Shader_Filter Filter)
// {
//     m_context->VSSetShader(m_vs3D.Get(), nullptr, 0);
//     m_context->PSSetShader(m_ps3D_Field.Get(), nullptr, 0);
//     m_context->IASetInputLayout(m_il3D.Get());
// 
//     ID3D11Buffer* cbs[] = { m_cbWorld3D.Get(), m_cbView3D.Get(), m_cbProjection3D.Get(),
//         m_cbAmbient3D.Get(), m_cbDirectional3D.Get() };
//     m_context->VSSetConstantBuffers(0, 5, cbs);
// 
//     switch (Filter)
//     {
//     case Shader_Filter::MAG_MIP_POINT:
//         m_context->PSSetSamplers(0, 1, m_sampler_Point.GetAddressOf());
//         break;
// 
//     case Shader_Filter::MAG_MIP_LINEAR:
//         m_context->PSSetSamplers(0, 1, m_sampler_Linear.GetAddressOf());
//         break;
// 
//     case Shader_Filter::ANISOTROPIC:
//         m_context->PSSetSamplers(0, 1, m_sampler_AnisoTropic.GetAddressOf());
//         break;
//     }
// }

// void Shader_Manager::SetWorldMatrix3D(const XMMATRIX& matrix)
// {
//     XMFLOAT4X4 transpose;
//     XMStoreFloat4x4(&transpose, XMMatrixTranspose(matrix));
//     m_context->UpdateSubresource(m_cbWorld3D.Get(), 0, nullptr, &transpose, 0, 0);
// }

// void Shader_Manager::SetViewMatrix3D(const XMMATRIX& matrix)
// {
//     XMFLOAT4X4 transpose;
//     XMStoreFloat4x4(&transpose, XMMatrixTranspose(matrix));
//     m_context->UpdateSubresource(m_cbView3D.Get(), 0, nullptr, &transpose, 0, 0);
// }

// void Shader_Manager::SetProjectionMatrix3D(const XMMATRIX& matrix)
// {
//     XMFLOAT4X4 transpose;
//     XMStoreFloat4x4(&transpose, XMMatrixTranspose(matrix));
//     m_context->UpdateSubresource(m_cbProjection3D.Get(), 0, nullptr, &transpose, 0, 0);
// }

// void Shader_Manager::SetTexture3D(ID3D11ShaderResourceView* textureView)
// {
//     m_context->PSSetShaderResources(0, 1, &textureView);
// }

// void Shader_Manager::SetFieldTextures(ID3D11ShaderResourceView* texture0, ID3D11ShaderResourceView* texture1)
// {
//     ID3D11ShaderResourceView* textures[] = { texture0, texture1 };
//     m_context->PSSetShaderResources(0, 2, textures);
// }

// void Shader_Manager::SetLightAmbient(const XMFLOAT4& color)
// {
//     m_context->UpdateSubresource(m_cbAmbient3D.Get(), 0, nullptr, &color, 0, 0);
// }

// void Shader_Manager::SetLightDirectional(const XMFLOAT4& worldDirection, const XMFLOAT4& color)
// {
//     Directional_Light Data{};
//     Data.Direction = worldDirection;
//     Data.Color = color;
// 
//     m_context->UpdateSubresource(m_cbDirectional3D.Get(), 0, nullptr, &Data, 0, 0);
// }


// --- Private Helper Methods ---
bool Shader_Manager::loadVertexShader(const char* filename, ComPtr<ID3D11VertexShader>& vs, ComPtr<ID3D11InputLayout>& il, const D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        Debug::D_Out << "Failed to open shader file: " << filename << std::endl;
        return false;
    }

    ifs.seekg(0, std::ios::end);
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    ifs.read(buffer.data(), size);
    ifs.close();

    HRESULT hr = m_device->CreateVertexShader(buffer.data(), buffer.size(), nullptr, &vs);
    if (FAILED(hr)) {
        Debug::D_Out << "Failed to create vertex shader: " << filename << std::endl;
        return false;
    }

    hr = m_device->CreateInputLayout(layout, numElements, buffer.data(), buffer.size(), &il);
    if (FAILED(hr)) {
        Debug::D_Out << "Failed to create input layout for: " << filename << std::endl;
        return false;
    }

    return true;
}

bool Shader_Manager::loadPixelShader(const char* filename, ComPtr<ID3D11PixelShader>& ps)
{
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        Debug::D_Out << "Failed to open shader file: " << filename << std::endl;
        return false;
    }

    ifs.seekg(0, std::ios::end);
    std::streamsize size = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    ifs.read(buffer.data(), size);
    ifs.close();

    HRESULT hr = m_device->CreatePixelShader(buffer.data(), buffer.size(), nullptr, &ps);
    if (FAILED(hr)) {
        Debug::D_Out << "Failed to create pixel shader: " << filename << std::endl;
        return false;
    }

    return true;
}

