#include "D3DRenderer.h"

template< class T > void SafeRelease(T*& pVal)
{
	if (pVal)
	{
		pVal->Release();
		pVal = nullptr;
	}
}


D3DRenderer::D3DRenderer()
{
}

D3DRenderer::~D3DRenderer()
{
	SafeRelease(render_target_view_);
	SafeRelease(depth_stencil_buffer_);
	SafeRelease(depth_stencil_view_);

	SafeRelease(g_InputLayout);
	SafeRelease(g_VertexShader);
	SafeRelease(g_PixelShader);
	SafeRelease(g_VertexBuffer);

	SafeRelease(swap_chain_);
	SafeRelease(immediate_context_);
	SafeRelease(device_);

#if !NDEBUG
	if (debug_ != nullptr)
	{
		debug_->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
		debug_ = nullptr;
	}
#endif
}

HRESULT CreateDeviceAndDeviceContext(ID3D11Device*& device, ID3D11DeviceContext*& device_context)
{
	HRESULT hr = S_OK;

	UINT create_divice_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	create_divice_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// vill endast arbeta mot Direct3D 11,
	// kontrollerar därför inte stödet för äldre versioner än Direct3D 11
	D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		//D3D_FEATURE_LEVEL_10_1,
		//D3D_FEATURE_LEVEL_10_0,
		//D3D_FEATURE_LEVEL_9_3,
		//D3D_FEATURE_LEVEL_9_2,
		//D3D_FEATURE_LEVEL_9_1
	};

	// kan användas för att kontrollera den faktiska Direct3D-nivån som blivit satt
	D3D_FEATURE_LEVEL feature_level;

	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_divice_flags,
		feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, &device, &feature_level, &device_context);

	return hr;

}

HRESULT CheckMultiSampleQualityLevels(ID3D11Device* device, DXGI_FORMAT format, UINT sample_count, UINT& out_msaa_quality, bool out_enable_msaa)
{
	HRESULT hr = device->CheckMultisampleQualityLevels(format, sample_count, &out_msaa_quality);
	if (FAILED(hr))
		return hr;

	out_enable_msaa = out_msaa_quality > 0 ? true : false;

	return hr;
}

DXGI_SWAP_CHAIN_DESC GetSwapChainDesc(HWND window, UINT screen_width, UINT screen_height, bool enable_msaa, UINT sample_count, UINT msaa_quality)
{
	DXGI_SWAP_CHAIN_DESC scd;

	scd.BufferDesc.Width = screen_width;
	scd.BufferDesc.Height = screen_height;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	if (enable_msaa)
	{
		scd.SampleDesc.Count = sample_count;
		scd.SampleDesc.Quality = msaa_quality - 1;
	}
	else
	{
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
	}

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = window;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	return scd;
}

HRESULT CreateSwapChain(ID3D11Device* device, DXGI_SWAP_CHAIN_DESC swap_chain_description, IDXGISwapChain*& out_swap_chain)
{
	HRESULT hr = S_OK;

	IDXGIDevice* dxgi_device = nullptr;
	hr = device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
	if (FAILED(hr))
	{
		SafeRelease(dxgi_device);
		return hr;
	}

	IDXGIAdapter* dxgi_adapter = nullptr;
	hr = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);
	if (FAILED(hr))
	{
		SafeRelease(dxgi_device);
		SafeRelease(dxgi_adapter);
		return hr;
	}

	IDXGIFactory* dxgi_factory = nullptr;
	hr = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory);
	if (FAILED(hr))
	{
		SafeRelease(dxgi_device);
		SafeRelease(dxgi_adapter);
		SafeRelease(dxgi_factory);
		return hr;
	}

	hr = dxgi_factory->CreateSwapChain(device, &swap_chain_description, &out_swap_chain);
	SafeRelease(dxgi_device);
	SafeRelease(dxgi_adapter);
	SafeRelease(dxgi_factory);

	return hr;
}

HRESULT CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swap_chain, ID3D11RenderTargetView*& out_render_target_view)
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* back_buffer = nullptr;
	hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	if (FAILED(hr))
	{
		SafeRelease(back_buffer);
		return hr;
	}

	hr = device->CreateRenderTargetView(back_buffer, 0, &out_render_target_view);
	SafeRelease(back_buffer);

	return hr;
}

D3D11_TEXTURE2D_DESC GetDepthStencilDesc(UINT screen_width, UINT screen_height, bool enable_msaa, UINT sample_count, UINT msaa_quality)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC dsd;

	dsd.Width = screen_width;
	dsd.Height = screen_height;
	dsd.MipLevels = 1;
	dsd.ArraySize = 1;
	dsd.Format = DXGI_FORMAT_D32_FLOAT; //DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (enable_msaa)
	{
		dsd.SampleDesc.Count = sample_count;
		dsd.SampleDesc.Quality = msaa_quality - 1;
	}
	else
	{
		dsd.SampleDesc.Count = 1;
		dsd.SampleDesc.Quality = 0;
	}

	dsd.Usage = D3D11_USAGE_DEFAULT;
	dsd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags = 0;
	dsd.MiscFlags = 0;

	return dsd;
}

HRESULT CreateDepthStencilView(ID3D11Device* device, D3D11_TEXTURE2D_DESC depth_stencil_description,
	ID3D11Texture2D*& out_depth_stencil_buffer, ID3D11DepthStencilView*& out_depth_stencil_view)
{
	HRESULT hr = S_OK;

	hr = device->CreateTexture2D(&depth_stencil_description, 0, &out_depth_stencil_buffer);
	if (FAILED(hr))
		return hr;

	hr = device->CreateDepthStencilView(out_depth_stencil_buffer, 0, &out_depth_stencil_view);

	return hr;
}

HRESULT CompileShaderFromFile(WCHAR* file_name, LPCSTR entry_point, LPCSTR shader_model, ID3DBlob** out_blob)
{
	HRESULT hr = S_OK;

	DWORD shader_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	shader_flags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* error_blob;
	hr = D3DCompileFromFile(file_name, nullptr, nullptr, entry_point, shader_model,
		shader_flags, 0, out_blob, &error_blob);
	if (FAILED(hr))
	{
		if (error_blob != nullptr)
			OutputDebugStringA((char*)error_blob->GetBufferPointer());
		SafeRelease(error_blob);
		return hr;
	}
	
	SafeRelease(error_blob);

	return S_OK;
}

//NOT HERE
//---------------------------------------------------------------------------
void D3DRenderer::SetViewport(int width, int height)
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	immediate_context_->RSSetViewports(1, &vp);
}
//---------------------------------------------------------------------------

bool D3DRenderer::Initialize(const HWND& main_window, int screen_width, int screen_height, UINT wanted_sample_count)
{
	if (FAILED(CreateDeviceAndDeviceContext(device_, immediate_context_)))
	{
		MessageBox(nullptr, L"Failed to create device and device context", 0, 0);
		return false;
	}

#if !NDEBUG
	if (FAILED(device_->QueryInterface(IID_PPV_ARGS(&debug_))))
	{
		MessageBox(nullptr, L"Failed to query debug", 0, 0);
		return false;
	}
#endif

	UINT msaa_quality = 0;
	bool enable_msaa = false;
	UINT sample_count = wanted_sample_count;
	if (FAILED(CheckMultiSampleQualityLevels(device_, DXGI_FORMAT_R8G8B8A8_UNORM, sample_count, msaa_quality, enable_msaa)))
	{
		MessageBox(nullptr, L"Failed to check multi saple quality levels", 0, 0);
		return false;
	}

	//
	// Kolla på D3D11CreateDeviceAndSwapChain(...)
	//
	DXGI_SWAP_CHAIN_DESC scd = GetSwapChainDesc(main_window, screen_width, screen_height, enable_msaa, sample_count, msaa_quality);
	if (FAILED(CreateSwapChain(device_, scd, swap_chain_)))
	{
		MessageBox(nullptr, L"Failed to create swap chain.", 0, 0);
		return false;
	}

	if (FAILED(CreateRenderTargetView(device_, swap_chain_, render_target_view_)))
	{
		MessageBox(nullptr, L"Failed to create render target view.", 0, 0);
		return false;
	}

	D3D11_TEXTURE2D_DESC dsd = GetDepthStencilDesc(screen_width, screen_height, enable_msaa, sample_count, msaa_quality);
	if (FAILED(CreateDepthStencilView(device_, dsd, depth_stencil_buffer_, depth_stencil_view_)))
	{
		MessageBox(nullptr, L"Failed to create depth stencil view.", 0, 0);
		return false;
	}

	SetViewport(800, 600);

	immediate_context_->OMSetRenderTargets(1, &render_target_view_, depth_stencil_view_);

	CreateShadersAndInputLayout();
	CreateTriangleVertexBuffer();

	return true;
}

ID3D11Device* D3DRenderer::GetDevice() const
{
	return device_;
}

ID3D11DeviceContext* D3DRenderer::GetDeviceContext() const
{
	return immediate_context_;
}


//NOT HERE
//---------------------------------------------------------------------------

HRESULT D3DRenderer::CreateTriangleVertexBuffer()
{
	struct Vertex
	{
		float Pos[3];
		float Normal[3];
		float TexCoord[2];
	}
	triangle[4] =
	{
		/* pos */-0.25f, -0.25f, 0, /* normal */ 0,0,-1, /* tex */ 0, 1,
		/* pos */-0.25f,  0.25f, 0, /* normal */ 0,0,-1, /* tex */ 0, 0,
		/* pos */0.25f, -0.25f, 0, /* normal */ 0,0,-1,  /* tex */ 1, 1,
		/* pos */0.25f,  0.25f, 0, /* normal */ 0,0,-1,  /* tex */ 1, 0
	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = sizeof(triangle);

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangle;
	return device_->CreateBuffer(&bufferDesc, &data, &g_VertexBuffer);
}

#include <string>
#include <fstream>

HRESULT CompileShader(char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader)
{
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS |
		D3DCOMPILE_IEEE_STRICTNESS;

	std::string shader_code;
	std::ifstream in(shaderFile, std::ios::in | std::ios::binary);
	if (in)
	{
		in.seekg(0, std::ios::end);
		shader_code.resize((UINT)in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&shader_code[0], shader_code.size());
		in.close();
	}

	ID3DBlob* pErrorBlob = nullptr;
	HRESULT hr = D3DCompile(
		shader_code.data(),
		shader_code.size(),
		nullptr,
		pDefines,
		nullptr,
		pEntrypoint,
		pTarget,
		dwShaderFlags,
		0,
		pCompiledShader,
		&pErrorBlob);

	if (pErrorBlob)
	{
		OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		SafeRelease(pErrorBlob);
	}

	return hr;
}

HRESULT D3DRenderer::CreateShadersAndInputLayout()
{
	HRESULT hr = S_OK;

	ID3DBlob* pVertexShader = nullptr;
	if (SUCCEEDED(hr = CompileShader("../../Shaders/DrawTri.vs", "VS_main", "vs_5_0", nullptr, &pVertexShader)))
	{
		if (SUCCEEDED(hr = device_->CreateVertexShader(
			pVertexShader->GetBufferPointer(),
			pVertexShader->GetBufferSize(),
			nullptr,
			&g_VertexShader)))
		{
			D3D11_INPUT_ELEMENT_DESC inputDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hr = device_->CreateInputLayout(
				inputDesc,
				ARRAYSIZE(inputDesc),
				pVertexShader->GetBufferPointer(),
				pVertexShader->GetBufferSize(),
				&g_InputLayout);
		}

		SafeRelease(pVertexShader);
	}
	else
	{
		MessageBoxA(nullptr, "Failed to create vertex shader (check Output window for more info)", 0, 0);
	}

	ID3DBlob* pPixelShader = nullptr;
	if (SUCCEEDED(hr = CompileShader("../../Shaders/DrawTri.ps", "PS_main", "ps_5_0", nullptr, &pPixelShader)))
	{
		hr = device_->CreatePixelShader(
			pPixelShader->GetBufferPointer(),
			pPixelShader->GetBufferSize(),
			nullptr,
			&g_PixelShader);

		SafeRelease(pPixelShader);
	}
	else
	{
		MessageBoxA(nullptr, "Failed to create pixel shader (check Output window for more info)", 0, 0);
	}

	return hr;
}

HRESULT D3DRenderer::Render()
{
	//clear back buffer, black color
	static float ClearColor[4] = { 0, 0, 0, 1 };
	immediate_context_->ClearRenderTargetView(render_target_view_, ClearColor);

	//clear depth buffer
	immediate_context_->ClearDepthStencilView(depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//set topology
	immediate_context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//draw triangle
	UINT32 vertexSize = sizeof(float) * 8;
	UINT32 offset = 0;
	ID3D11Buffer* buffersToSet[] = { g_VertexBuffer };
	immediate_context_->IASetVertexBuffers(0, 1, buffersToSet, &vertexSize, &offset);

	//set vertex description
	immediate_context_->IASetInputLayout(g_InputLayout);

	//set shader stages
	immediate_context_->VSSetShader(g_VertexShader, nullptr, 0);
	immediate_context_->HSSetShader(nullptr, nullptr, 0);
	immediate_context_->DSSetShader(nullptr, nullptr, 0);
	immediate_context_->GSSetShader(nullptr, nullptr, 0);
	immediate_context_->PSSetShader(g_PixelShader, nullptr, 0);

	//draw three vertices from the bound vertex buffer
	immediate_context_->Draw(4, 0);

	//swap front and back buffer
	return swap_chain_->Present(0, 0);
}