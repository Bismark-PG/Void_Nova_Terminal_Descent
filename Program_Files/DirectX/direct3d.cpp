/*==============================================================================

	Reset Direct3D [direct3d.cpp]

	Author : Choi HyungJoon

==============================================================================*/
#include <d3d11.h>
#include "direct3d.h"
#include "debug_text.h"
#include "debug_ostream.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#if defined(DEBUG) || defined(_DEBUG)
	#pragma comment(lib, "DirectXTex_Debug.lib")
#else
	#pragma comment(lib, "DirectXTex_Release.lib")
#endif

/* 各種インターフェース */
static ID3D11Device* g_pDevice = nullptr;
static ID3D11DeviceContext* g_pDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11BlendState* g_pBlendStateMultiply = nullptr;
static ID3D11DepthStencilState* g_pDepthStencilStateDepthDisable = nullptr;

/* バックバッファ関連 */
static ID3D11RenderTargetView* g_pRenderTargetView = nullptr;
static ID3D11Texture2D* g_pDepthStencilBuffer = nullptr;
static ID3D11DepthStencilView* g_pDepthStencilView = nullptr;
static D3D11_TEXTURE2D_DESC g_BackBufferDesc{};
static D3D11_VIEWPORT g_Viewport{};

static bool configureBackBuffer(); // バックバッファの設定・生成
static void releaseBackBuffer(); // バックバッファの解放

bool Direct3D_Initialize(HWND hWnd)
{
    /* デバイス、スワップチェーン、コンテキスト生成 */
    DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.BufferCount = 2;
    // swap_chain_desc.BufferDesc.Width = 0;
    // swap_chain_desc.BufferDesc.Height = 0;
	// ⇒ ウィンドウサイズに合わせて自動的に設定される
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;

	// DISCARD or SEQUENTIAL>> Normal
	// Use FILP >> Skip Adaptive V-Sync
	// IF Want to Benchmark or Test Something Else etc...
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    //swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
    swap_chain_desc.OutputWindow = hWnd;

	/*
	IDXGIFactory1* pFactory;
	CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
	IDXGIAdapter1* pAdapter;
	pFactory->EnumAdapters1(1, &pAdapter); // セカンダリアダプタを取得
	pFactory->Release();
	DXGI_ADAPTER_DESC1 desc;
	pAdapter->GetDesc1(&desc); // アダプタの情報を取得して確認したい場合
	pAdapter->Release(); // D3D11CreateDeviceAndSwapChain()の第１引数に渡して利用し終わったら解放する
	*/

	UINT device_flags = 0;

#if defined(DEBUG) || defined(_DEBUG)
    device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };
    
    D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
 
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        device_flags,
        levels,
        ARRAYSIZE(levels),
        D3D11_SDK_VERSION,
        &swap_chain_desc,
        &g_pSwapChain,
        &g_pDevice,
        &feature_level,
        &g_pDeviceContext);

	if (FAILED(hr)) {
		Debug::D_Out << "Direct3D_Initialize : D3D11 Create Device And Swap Chain failed. hr = " << std::hex << hr << std::dec << "\n";
		MessageBox(hWnd, L"Direct3Dの初期化に失敗しました", L"エラー", MB_OK);
		return false;
	}

	Debug::D_Out << "Direct3D_Initialize : Device/swapchain/context created\n";

	if (!configureBackBuffer()) {
		Debug::D_Out << "Direct3D_Initialize : Configure Back Buffer failed.\n";
		MessageBox(hWnd, L"バックバッファの設定に失敗しました", L"エラー", MB_OK);
		return false;
	}

	Debug::D_Out << "Direct3D_Initialize : Back buffer configured\n";

	// aブレンド(Blend)
	// RGBA A >> 好きに使っても良い
	// aデスト(Dest), aブレンド(Blend)

	// ブレンドステート設定
	D3D11_BLEND_DESC bd = {};
	bd.AlphaToCoverageEnable = FALSE;
	bd.IndependentBlendEnable = FALSE;
	bd.RenderTarget[0].BlendEnable = TRUE; // aブレンドするしない (Do Not Blend)

	// src >> ソース(今から描く絵 (色)) / dest >> すでに描かれた絵(色)

	// RGB
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // OP + >> Add Operand(演算子)
	// SrcRGB * SrcA + DestRGB * (1 - SrcA)

	// A
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;	  // ONE  (1)
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO; // ZERO (0)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD; // OP + >> Add Operand(演算子)
	// SrcA * 1 + DestA * 0

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	g_pDevice->CreateBlendState(&bd, &g_pBlendStateMultiply);

	float blend_factor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pDeviceContext->OMSetBlendState(g_pBlendStateMultiply, blend_factor, 0xffffffff);

	// Depth Setting
	// 深度ステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC dsd = {};
	dsd.DepthFunc = D3D11_COMPARISON_LESS;
	dsd.StencilEnable = FALSE;
	dsd.DepthEnable = FALSE; // 無効にする
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	g_pDevice->CreateDepthStencilState(&dsd, &g_pDepthStencilStateDepthDisable);

	// dsd.DepthEnable = TRUE;
	// dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	// g_pDevice->CreateDepthStencilState(&dsd, &g_pDepthStencilStateDepthEnable);

	g_pDeviceContext->OMSetDepthStencilState(g_pDepthStencilStateDepthDisable, NULL);

    return true;
}

void Direct3D_Finalize()
{
	Debug::D_Out << "Direct3D_Finalize : Start\n";

	if (g_pDeviceContext) {
		// ensure render targets are unbound before releasing swapchain/backbuffers
		ID3D11RenderTargetView* nullRTV[1] = { nullptr };
		g_pDeviceContext->OMSetRenderTargets(0, nullRTV, nullptr);
		g_pDeviceContext->Flush();
		Debug::D_Out << "Direct3D_Finalize : Unbound render targets and flushed context\n";
	}

	releaseBackBuffer();

	SAFE_RELEASE(g_pDepthStencilStateDepthDisable);
	SAFE_RELEASE(g_pBlendStateMultiply);

	SAFE_RELEASE(g_pSwapChain);
	SAFE_RELEASE(g_pDeviceContext);
	SAFE_RELEASE(g_pDevice);

	Text::DebugText::ReleaseAllTextures();
	Debug::D_Out << "Direct3D_Finalize : Released all static debug text textures.\n";

	Debug::D_Out << "Direct3D_Finalize : Finished\n";
}

void Direct3D_Clear()
{
	// Defensive : Ensure device context exists
	if (!g_pDeviceContext) {
		Debug::D_Out << "Direct3D_Clear : Device context is null - skipping clear\n";
		return;
	}

	// Defensive : If RTV or DSV missing, Set Log and Skip to avoid Draw without RTV
	if (!g_pRenderTargetView || !g_pDepthStencilView) {
		Debug::D_Out << "Direct3D_Clear : RTV or DSV is null (RTV = " << g_pRenderTargetView << ", DSV = " << g_pDepthStencilView << ") - skipping clear & binding\n";
		return;
	}

	// Bind RTV/DSV (guarantee before Clear / Draw)
	ID3D11RenderTargetView* rtvs[1] = { g_pRenderTargetView };
	g_pDeviceContext->OMSetRenderTargets(1, rtvs, g_pDepthStencilView);
	
	// Set Default Window Color
	float clear_color[4] = { 0.2f, 0.4f, 0.2f, 1.0f };
	g_pDeviceContext->ClearRenderTargetView(g_pRenderTargetView, clear_color);
	g_pDeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Direct3D_Present()
{
	if (!g_pSwapChain) {
		Debug::D_Out << "Direct3D_Present : SwapChain is null - skipping Present\n";
		return;
	}

	// Present; using VSync = 1
	HRESULT hr = g_pSwapChain->Present(1, 0);
	if (FAILED(hr)) {
		Debug::D_Out << "Direct3D_Present : Present failed hr=" << std::hex << hr << std::dec << "\n";
	}
}

unsigned int Direct3D_GetBackBufferWidth()
{
	return g_BackBufferDesc.Width;
}

unsigned int Direct3D_GetBackBufferHeight()
{
	return g_BackBufferDesc.Height;
}

ID3D11Device* Direct3D_GetDevice()
{
	return g_pDevice;
}

ID3D11DeviceContext* Direct3D_GetContext()
{
	return g_pDeviceContext;
}

void Direct3D_Re_Size(UINT width, UINT height)
{
	if (!g_pSwapChain) return;

	releaseBackBuffer();

	HRESULT hr = g_pSwapChain->ResizeBuffers(
		0,
		width,
		height,
		DXGI_FORMAT_UNKNOWN,
		0);

	if (FAILED(hr)) {
		OutputDebugStringA("[Direct3D] ResizeBuffers failed!\n");
		return;
	}

	configureBackBuffer();

	OutputDebugStringA("[Direct3D] OnResize complete.\n");
}

bool configureBackBuffer()
{
    HRESULT hr;

    ID3D11Texture2D* back_buffer_pointer = nullptr;

	Debug::D_Out << "[Function] configureBackBuffer : start\n";

	if (!g_pSwapChain || !g_pDevice || !g_pDeviceContext) {
		Debug::D_Out << "configureBackBuffer : SwapChain/device/context null (swap = " << g_pSwapChain << ", dev = " << g_pDevice << ", ctx = " << g_pDeviceContext << ")\n";
		return false;
	}

    // Get Back Buffer
    hr = g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer_pointer);

    if (FAILED(hr) || back_buffer_pointer == nullptr) {
        Debug::D_Out << "configureBackBuffer : GetBuffer failed hr = " << std::hex << hr << std::dec << "\n";
        return false;
    }

	// Create Render Target View
	hr = g_pDevice->CreateRenderTargetView(back_buffer_pointer, nullptr, &g_pRenderTargetView);

	if (FAILED(hr) || g_pRenderTargetView == nullptr) {
		Debug::D_Out << "configureBackBuffer : CreateRenderTargetView failed hr = " << std::hex << hr << std::dec << "\n";
		if (back_buffer_pointer) back_buffer_pointer->Release();
		return false;
	}

	// Save Back Buffer Information
    back_buffer_pointer->GetDesc(&g_BackBufferDesc);

	// Release Back Buffer Pointer
	back_buffer_pointer->Release();

	back_buffer_pointer = nullptr;
	Debug::D_Out << "configureBackBuffer: created RTV (ptr = " << g_pRenderTargetView
		<< "), BackBuffer size = " << g_BackBufferDesc.Width
		<< " x " << g_BackBufferDesc.Height << "\n";

	// Mack Depth Stencil Buffer 
	D3D11_TEXTURE2D_DESC depth_stencil_desc{};
	depth_stencil_desc.Width = g_BackBufferDesc.Width;
	depth_stencil_desc.Height = g_BackBufferDesc.Height;
	depth_stencil_desc.MipLevels = 1;
	depth_stencil_desc.ArraySize = 1;
	depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_desc.SampleDesc.Count = 1;
	depth_stencil_desc.SampleDesc.Quality = 0;
	depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	// Double Safety Device
	depth_stencil_desc.CPUAccessFlags = 0;
	depth_stencil_desc.MiscFlags = 0;

	hr = g_pDevice->CreateTexture2D(&depth_stencil_desc, nullptr, &g_pDepthStencilBuffer);

	if (FAILED(hr) || g_pDepthStencilBuffer == nullptr) {
		Debug::D_Out << "configureBackBuffer : CreateTexture2D (depth) failed hr = " << std::hex << hr << std::dec << "\n";
		SAFE_RELEASE(g_pRenderTargetView);
		return false;
	}

	// Mack Depth Stencil View 
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
	depth_stencil_view_desc.Format = depth_stencil_desc.Format;
	depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	// Double Safety Device
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	depth_stencil_view_desc.Flags = 0;

	hr = g_pDevice->CreateDepthStencilView(g_pDepthStencilBuffer, &depth_stencil_view_desc, &g_pDepthStencilView);

	if (FAILED(hr) || g_pDepthStencilView == nullptr) {
		Debug::D_Out << "configureBackBuffer : CreateDepthStencilView failed hr = " << std::hex << hr << std::dec << "\n";
		SAFE_RELEASE(g_pDepthStencilBuffer);
		SAFE_RELEASE(g_pRenderTargetView);
		return false;
	}

	Debug::D_Out << "configureBackBuffer : created DSV (ptr = " << g_pDepthStencilView << ")\n";


	// Set Viewport
	g_Viewport.TopLeftX = 0.0f;
	g_Viewport.TopLeftY = 0.0f;
	g_Viewport.Width = static_cast<FLOAT>(g_BackBufferDesc.Width);
	g_Viewport.Height = static_cast<FLOAT>(g_BackBufferDesc.Height);
	g_Viewport.MinDepth = 0.0f;
	g_Viewport.MaxDepth = 1.0f;
	g_pDeviceContext->RSSetViewports(1, &g_Viewport);

	ID3D11RenderTargetView* rtvs[1] = { g_pRenderTargetView };
	g_pDeviceContext->OMSetRenderTargets(1, rtvs, g_pDepthStencilView);

	Debug::D_Out << "configureBackBuffer : Bound RTV and DSV to OM\n";
	return true;
}

void releaseBackBuffer()
{
	Debug::D_Out << "releaseBackBuffer : Releasing back buffer resources\n";
	SAFE_RELEASE(g_pRenderTargetView);
	SAFE_RELEASE(g_pDepthStencilBuffer);
	SAFE_RELEASE(g_pDepthStencilView);

	// Make sure pointers are null after release (defensive)
	g_pRenderTargetView = nullptr;
	g_pDepthStencilBuffer = nullptr;
	g_pDepthStencilView = nullptr;
}
