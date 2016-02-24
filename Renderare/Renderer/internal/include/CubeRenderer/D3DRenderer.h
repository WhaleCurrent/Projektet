#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")


class D3DRenderer
{
private:
	ID3D11Device* device_ = nullptr;
	ID3D11DeviceContext* immediate_context_ = nullptr;

	IDXGISwapChain* swap_chain_ = nullptr;
	ID3D11RenderTargetView* render_target_view_ = nullptr;

	ID3D11Texture2D* depth_stencil_buffer_ = nullptr;
	ID3D11DepthStencilView* depth_stencil_view_ = nullptr;

	ID3D11Debug* debug_ = nullptr;

	//---------------------------------------------------------------------------
	ID3D11InputLayout*		g_InputLayout = nullptr;
	ID3D11VertexShader*		g_VertexShader = nullptr;
	ID3D11PixelShader*		g_PixelShader = nullptr;
	ID3D11Buffer*			g_VertexBuffer = nullptr;
	//---------------------------------------------------------------------------
public:
	D3DRenderer();
	~D3DRenderer();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	//---------------------------------------------------------------------------
	HRESULT Render();
	HRESULT CreateTriangleVertexBuffer();
	HRESULT CreateShadersAndInputLayout();
	void SetViewport(int x, int y);
	//---------------------------------------------------------------------------

	bool Initialize(const HWND& main_window, int screen_width, int screen_height, UINT wanted_sample_count);
};

