#include <cassert>
#include <algorithm>
#include <cstdio>
#include <windows.h>
#include <d3d9.h>

// Local Includes

// This Includes
#include "messageprocessor.h"
#include "../include/D3DRenderer.h"

// Static Variables

// Static Function Prototypes
static void ValidatePresentParams(D3DPRESENT_PARAMETERS &p)
{
	// Ensure fields are fully initialized / sane.
	// Windowed mode: let D3D choose backbuffer size/format.
	if (p.Windowed)
	{
		p.BackBufferWidth = 0;
		p.BackBufferHeight = 0;
		p.BackBufferFormat = D3DFMT_UNKNOWN;
		p.FullScreen_RefreshRateInHz = 0;
	} else
	{
		// Fullscreen: enforce reasonable defaults if caller provided bad values.
		if (p.BackBufferWidth <= 0 || p.BackBufferWidth > 16384)
		{
			OutputDebugStringA("ValidatePresentParams: invalid BackBufferWidth, clamping to 800\n");
			p.BackBufferWidth = 800;
		}
		if (p.BackBufferHeight <= 0 || p.BackBufferHeight > 16384)
		{
			OutputDebugStringA("ValidatePresentParams: invalid BackBufferHeight, clamping to 600\n");
			p.BackBufferHeight = 600;
		}
		if (p.BackBufferFormat == 0)
		{
			OutputDebugStringA("ValidatePresentParams: missing BackBufferFormat, selecting D3DFMT_X8R8G8B8\n");
			p.BackBufferFormat = D3DFMT_X8R8G8B8;
		}
		if (p.AutoDepthStencilFormat == 0)
		{
			p.AutoDepthStencilFormat = D3DFMT_D24S8;
		}
	}

	// Ensure multisample, flags and depth/stencil fields are set to sensible values.
	if (p.MultiSampleType < 0 || p.MultiSampleType > D3DMULTISAMPLE_16_SAMPLES)
	{
		p.MultiSampleType = D3DMULTISAMPLE_NONE;
	}
	if (p.AutoDepthStencilFormat == 0)
	{
		// Prefer 24/8 but fallback to 16-bit if driver doesn't support 24/8.
		p.AutoDepthStencilFormat = D3DFMT_D24S8;
	}
	// BackBufferCount sanity
	p.BackBufferCount = std::max(1, std::min<int>(p.BackBufferCount ? p.BackBufferCount : 1, 4));
}

static void LogPresentParams(const D3DPRESENT_PARAMETERS &p)
{
#ifdef _DEBUG
	char buf[512];
	std::snprintf(buf, sizeof(buf),
	              "D3D PresentParams: BackBufferW=%d H=%d Format=0x%08X Count=%d Windowed=%d Swap=%d AutoDepth=%d ADSFormat=0x%08X Refresh=%d MS=%d Flags=0x%08X\n",
	              p.BackBufferWidth, p.BackBufferHeight, static_cast<unsigned int>(p.BackBufferFormat),
	              p.BackBufferCount, p.Windowed, static_cast<int>(p.SwapEffect), p.EnableAutoDepthStencil,
	              static_cast<unsigned int>(p.AutoDepthStencilFormat), p.FullScreen_RefreshRateInHz,
	              p.MultiSampleType, p.Flags);
	OutputDebugStringA(buf);
#else
	(void) p; // suppress unused parameter warning in release
#endif
}

static void ValidateAndFixPresentParams(D3DPRESENT_PARAMETERS &p, HWND hWnd, IDirect3D9 *d3d)
{
	char buf[256];

	// If HWND invalid or minimized, force safe windowed params
	if (!hWnd || !IsWindow(hWnd) || IsIconic(hWnd))
	{
		OutputDebugStringA("ValidatePresentParams: invalid/minimized HWND, forcing safe windowed params\n");
		p.Windowed = TRUE;
		p.BackBufferWidth = 0;
		p.BackBufferHeight = 0;
		p.BackBufferFormat = D3DFMT_UNKNOWN;
		p.FullScreen_RefreshRateInHz = 0;
		p.SwapEffect = D3DSWAPEFFECT_DISCARD;
		p.EnableAutoDepthStencil = TRUE;
		p.AutoDepthStencilFormat = D3DFMT_D16;
	} else if (p.Windowed)
	{
		// Windowed: use safe defaults (let D3D choose desktop format/size)
		p.BackBufferWidth = 0;
		p.BackBufferHeight = 0;
		p.BackBufferFormat = D3DFMT_UNKNOWN;
		p.FullScreen_RefreshRateInHz = 0;
		if (p.SwapEffect == D3DSWAPEFFECT_FLIP)
			p.SwapEffect = D3DSWAPEFFECT_DISCARD;
	} else
	{
		// Fullscreen: clamp sizes and ensure a valid backbuffer format
		if (p.BackBufferWidth <= 0 || p.BackBufferWidth > 16384) p.BackBufferWidth = 800;
		if (p.BackBufferHeight <= 0 || p.BackBufferHeight > 16384) p.BackBufferHeight = 600;
		if (p.BackBufferFormat == 0) p.BackBufferFormat = D3DFMT_X8R8G8B8;

		// Prefer adapter display format if available
		if (d3d)
		{
			D3DDISPLAYMODE mode;
			if (SUCCEEDED(d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
			{
				if (p.BackBufferFormat != mode.Format)
				{
					std::snprintf(buf, sizeof(buf),
					              "ValidatePresentParams: adjusting BackBufferFormat to adapter format 0x%08X\n",
					              static_cast<unsigned int>(mode.Format));
					OutputDebugStringA(buf);
					p.BackBufferFormat = mode.Format;
				}
				if (p.FullScreen_RefreshRateInHz == 0)
					p.FullScreen_RefreshRateInHz = mode.RefreshRate;
			}
		}

		// Use conservative depth/stencil if unset
		if (p.AutoDepthStencilFormat == 0) p.AutoDepthStencilFormat = D3DFMT_D24S8;
	}

	// Sanity for multisample / counts
	if (p.MultiSampleType < 0 || p.MultiSampleType > D3DMULTISAMPLE_16_SAMPLES) p.MultiSampleType = D3DMULTISAMPLE_NONE;
	p.BackBufferCount = std::max(1, std::min<int>(p.BackBufferCount ? p.BackBufferCount : 1, 4));

	// Final log
#ifdef _DEBUG
	std::snprintf(buf, sizeof(buf),
	              "ValidatePresentParams: BackBufferW=%d H=%d Format=0x%08X Count=%d Windowed=%d Swap=%d ADS=0x%08X Refresh=%d MS=%d Flags=0x%08X\n",
	              p.BackBufferWidth, p.BackBufferHeight, static_cast<unsigned int>(p.BackBufferFormat),
	              p.BackBufferCount, p.Windowed ? 1 : 0, static_cast<int>(p.SwapEffect),
	              static_cast<unsigned int>(p.AutoDepthStencilFormat),
	              p.FullScreen_RefreshRateInHz, p.MultiSampleType, p.Flags);
	OutputDebugStringA(buf);
	std::fprintf(stderr, "%s", buf);
#endif
}

// Implementation

CD3DRenderer::CD3DRenderer()
	: m_bShutDown(false)
	  , m_pDirect3D(0)
{
	//Constructor.
}

CD3DRenderer::~CD3DRenderer()
{
	//Deconstructor.
	if (!m_bShutDown)
	{
		Shutdown();
	}
}

/**
*
* Calculates an orthogonal projection matrix based on the values past in.
*
* @param _fNear: specifies the near plane. Any geometry
* in front of the near plane gets culled.
* @param _fFar: specifies the value for the far plane.
* Any geometry behind the far plane gets culled.
* @return void.
*
*/
void
CD3DRenderer::CalculateOrthogonalMatrix(Float32 _fNear, Float32 _fFar)
{
	D3DXMATRIX orthomatrix;
	D3DXMatrixOrthoLH(&orthomatrix,
	                  static_cast<Float32>(m_iScreenWidth),
	                  static_cast<Float32>(m_iScreenHeight),
	                  _fNear,
	                  _fFar);
	SetProjectionMatrix(orthomatrix);
}

/**
*
* Calculates a perspective projection matrix based on the values past in.
*
* @param _fFov: specifies the field of view, in radians.
* @param _fNear: specifies the near plane. Any geometry
* in front of the near plane gets culled.
* @param _fFar: specifies the value for the far plane.
* Any geometry behind the far plane gets culled.
* @return void.
*
*/
void
CD3DRenderer::CalculateProjectionMatrix(Float32 _fFov, Float32 _fNear, Float32 _fFar)
{
	D3DXMATRIX projectMatrix;
	// Ensure floating point division
	Float32 fAspectRatio = static_cast<Float32>(m_iScreenWidth) / static_cast<Float32>(m_iScreenHeight);
	D3DXMatrixPerspectiveFovLH(&projectMatrix, _fFov, fAspectRatio, _fNear, _fFar);
	SetProjectionMatrix(projectMatrix);
}

/**
*
* clears the specified buffers.
*
* @param _bTarget: clears the frame buffer.
* @param _bDepth: clears the depth buffer.
* @param _bStencil: clears the stencil buffer.
* @return void.
*
*/
void
CD3DRenderer::Clear(bool _bTarget, bool _bDepth, bool _bStencil)
{
	m_bScreenCleared = true;
	DWORD flags = 0x00000000;
	HRESULT hr;

	if (_bTarget)
	{
		flags |= D3DCLEAR_TARGET;
	}
	if (_bDepth)
	{
		flags |= D3DCLEAR_ZBUFFER;
	}
	if (_bStencil)
	{
		flags |= D3DCLEAR_STENCIL;
	}
	hr = m_pDevice->Clear(0, 0, flags, m_clearColour, 1.0f, 0);
	if (FAILED(hr))
	{
		assert(0);
	}
}

/**
*
* ends the current render cycle and renders the frame buffer to the screen.
* buffer to the screen.
*
* @return void.
*
*/
void
CD3DRenderer::EndRender()
{
	m_bScreenCleared = false;
	m_bRenderingScene = false;

	m_pDevice->EndScene();

	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

/**
*
* Returns the current projection matrix.
*
* @return Returns the current projection matrix.
*
*/
D3DXMATRIX &
CD3DRenderer::GetProjectionMatrix()
{
	return (m_matProjection);
}

/**
*
* Returns the current view matrix.
*
* @return Returns the current view matrix.
*
*/
D3DXMATRIX &
CD3DRenderer::GetViewMatrix()
{
	return (m_matView);
}

/**
*
* Returns the current world matrix.
*
* @return Returns the current world matrix.
*
*/
D3DXMATRIX &
CD3DRenderer::GetWorldMatrix()
{
	return (m_matWorld);
}

/**
*
* Initialises the Renderer.
*
* @param _iWidth: width of the frame and z buffers, in pixels.
* @param _iHeight: height of the frame and z buffers , in pixels.
* @param _hWindow: handle to a window in which the frame buffer
* will render.
* @param _bFullscreen: boolean flag, if true, the program will run
* in full screen mode.
* @return Returns true if the function call succeeded.
*
*/
bool
CD3DRenderer::Initialise(Int32 _iWidth, Int32 _iHeight, HWND _hWindow, bool _bFullscreen)
{
	bool bFailure = false;

	m_iScreenWidth = _iWidth;
	m_iScreenHeight = _iHeight;

	m_hWindow = _hWindow;
	m_bFullScreen = _bFullscreen;

	m_fFogStart = FOG_START;
	m_fFogEnd = FOG_END;

	m_pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pDirect3D)
	{
		OutputDebugStringA("CD3DRenderer: Direct3DCreate9 returned null\n");
		CMessageProcessor::GetInstance()->SendMessage("CD3DRenderer: Direct3DCreate9 returned null");
		return bFailure;
	}

	D3DPRESENT_PARAMETERS myParams;
	PopulateParams(myParams, _iWidth, _iHeight, m_hWindow, m_bFullScreen);

	// Validate and fix obvious problems (prevents corrupted or out-of-range values causing INVALIDCALL).
	ValidateAndFixPresentParams(myParams, m_hWindow, m_pDirect3D);

	// Log params to help debug D3DERR_INVALIDCALL.
	LogPresentParams(myParams);

	HRESULT result = E_FAIL;

	// Try creating HAL device with several vertex processing modes.
	DWORD vpFlags[] = {
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		D3DCREATE_MIXED_VERTEXPROCESSING,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING
	};

	for (DWORD flag: vpFlags)
	{
		result = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
		                                   D3DDEVTYPE_HAL,
		                                   m_hWindow,
		                                   flag,
		                                   &myParams,
		                                   &m_pDevice);
		if (SUCCEEDED(result))
			break;
	}

	// If invalid-call, attempt safe fallbacks: simpler presentation parameters and try HAL again.
	if (FAILED(result) && result == D3DERR_INVALIDCALL)
	{
		OutputDebugStringA("CD3DRenderer: CreateDevice returned D3DERR_INVALIDCALL, retrying with safer params\n");
		CMessageProcessor::GetInstance()->SendMessage(
			"CD3DRenderer: CreateDevice returned D3DERR_INVALIDCALL, retrying with safer params");
		// Safer params
		myParams.AutoDepthStencilFormat = D3DFMT_D16;
		myParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		myParams.MultiSampleType = D3DMULTISAMPLE_NONE;
		myParams.Flags = 0;
		LogPresentParams(myParams);

		// For windowed mode ensure BackBuffer size is 0 (already set in PopulateParams).
		if (myParams.Windowed)
		{
			myParams.BackBufferWidth = 0;
			myParams.BackBufferHeight = 0;
			myParams.FullScreen_RefreshRateInHz = 0;
		}

		LogPresentParams(myParams);

		for (DWORD flag: vpFlags)
		{
			result = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
			                                   D3DDEVTYPE_HAL,
			                                   m_hWindow,
			                                   flag,
			                                   &myParams,
			                                   &m_pDevice);
			if (SUCCEEDED(result))
				break;
		}
	}

	// If still failing and we were trying fullscreen, force windowed-safe params and retry HAL before REF.
	if (FAILED(result) && !myParams.Windowed)
	{
		OutputDebugStringA("CD3DRenderer: HAL failed in fullscreen, forcing windowed-safe params and retrying HAL\n");
		// Update renderer state to reflect windowed fallback
		m_bFullScreen = false;
		myParams.Windowed = TRUE;
		myParams.BackBufferWidth = 0;
		myParams.BackBufferHeight = 0;
		myParams.BackBufferFormat = D3DFMT_UNKNOWN;
		myParams.FullScreen_RefreshRateInHz = 0;
		myParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
		myParams.AutoDepthStencilFormat = D3DFMT_D16;
		myParams.EnableAutoDepthStencil = TRUE;
		myParams.MultiSampleType = D3DMULTISAMPLE_NONE;
		myParams.Flags = 0;
		LogPresentParams(myParams);

		for (DWORD flag: vpFlags)
		{
			result = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
			                                   D3DDEVTYPE_HAL,
			                                   m_hWindow,
			                                   flag,
			                                   &myParams,
			                                   &m_pDevice);
			if (SUCCEEDED(result))
				break;
		}
	}

	// Final fallback: try reference device (software) if HAL fails.
	if (FAILED(result))
	{
		OutputDebugStringA("CD3DRenderer: HAL failed, attempting REF device as last resort\n");
		CMessageProcessor::GetInstance()->SendMessage("CD3DRenderer: HAL failed, attempting REF device as last resort");
		LogPresentParams(myParams);
		result = m_pDirect3D->CreateDevice(D3DADAPTER_DEFAULT,
		                                   D3DDEVTYPE_REF,
		                                   m_hWindow,
		                                   D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		                                   &myParams,
		                                   &m_pDevice);
	}

	if (FAILED(result))
	{
		char buf[256];
		std::snprintf(buf, sizeof(buf), "CD3DRenderer: CreateDevice failed at %s:%d, hr=0x%08X\n",
		              __FILE__, __LINE__, static_cast<unsigned int>(result));
		OutputDebugStringA(buf);
		std::fprintf(stderr, "%s", buf);

		if (m_pDirect3D)
		{
			m_pDirect3D->Release();
			m_pDirect3D = nullptr;
		}

		return bFailure;
	}

	// Device created successfully, set up common render states.
	m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
	m_pDevice->SetRenderState(D3DRS_AMBIENT, 0xFF00FF00);

	m_fFogStart = FOG_START;
	m_fFogEnd = FOG_END;

	m_clearColour = defaultClearColour;

	return !bFailure;
}

/**
*
* Populates the presentation parameters structure based on the supplied values.
*
* @param _myParams: Reference to a D3DPRESENT_PARAMETERS stucture to be populated.
* @param _width: Width of the frame buffer, in pixels.
* @param _height: Height of the frame buffer, in pixels.
* @param _hWnd: Handle to a window, in which Direct 3d will render.
* @param _bFullScreen: If true, the application will run in full screen mode.
* @return void.
*
*/
void
CD3DRenderer::PopulateParams(D3DPRESENT_PARAMETERS &_myParams, Int32 _width, Int32 _height, HWND _hWnd,
                             bool _bFullScreen)
{
	ZeroMemory(&_myParams, sizeof(D3DPRESENT_PARAMETERS));
	_myParams.BackBufferCount = 1;
	_myParams.hDeviceWindow = _hWnd;
	_myParams.EnableAutoDepthStencil = TRUE;
	_myParams.AutoDepthStencilFormat = D3DFMT_D24S8; // reasonable default, fallback may change it
	_myParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	_myParams.Flags = 0;

	_myParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	if (_bFullScreen)
	{
		// Fullscreen: use adapter format and set requested size/refresh rate explicitly.
		// if (m_pDirect3D)
		// {
		D3DDISPLAYMODE mode;
		if (SUCCEEDED(m_pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
		{
			_myParams.BackBufferFormat = mode.Format;
			_myParams.FullScreen_RefreshRateInHz = mode.RefreshRate;
		} else
		{
			_myParams.BackBufferFormat = D3DFMT_X8R8G8B8;
			_myParams.FullScreen_RefreshRateInHz = 0;
		}
		// } else
		// {
		// 	_myParams.BackBufferFormat = D3DFMT_X8R8G8B8;
		// 	_myParams.FullScreen_RefreshRateInHz = 0;
		// }

		// Use the requested explicit resolution in fullscreen.
		_myParams.BackBufferWidth = _width;
		_myParams.BackBufferHeight = _height;
		_myParams.Windowed = FALSE;
		// Use DISCARD for widest compatibility; FLIP requires additional constraints (e.g., multiple backbuffers).
		_myParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	} else
	{
		// Windowed: let D3D choose the backbuffer size and format for best compatibility.
		_myParams.BackBufferFormat = D3DFMT_UNKNOWN;
		_myParams.BackBufferWidth = 0; // let D3D choose (avoids mismatch with window chrome)
		_myParams.BackBufferHeight = 0;
		_myParams.FullScreen_RefreshRateInHz = 0;
		_myParams.Windowed = TRUE;
		_myParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	}
}

/**
*
* Sets the clear colour for the frame buffer.
*
* @param _fRed: red component of the frame buffer ranging from 0 to 255.
* @param _fGreen: green component of the frame buffer ranging from 0 to 255.
* @param _fBlue: blue component of the frame buffer ranging from 0 to 255.
* @return void.
*
*/
void
CD3DRenderer::SetClearColour(Float32 _fRed, Float32 _fGreen, Float32 _fBlue)
{
	if (_fRed > 255)
	{
		_fRed = 255;
	}
	if (_fRed < 0)
	{
		_fRed = 0;
	}
	if (_fGreen > 255)
	{
		_fGreen = 255;
	}
	if (_fGreen < 0)
	{
		_fGreen = 0;
	}
	if (_fBlue > 255)
	{
		_fBlue = 255;
	}
	if (_fBlue < 0)
	{
		_fBlue = 0;
	}
	Int32 newRed = static_cast<Int32>(_fRed);
	Int32 newGreen = static_cast<Int32>(_fGreen);
	Int32 newBlue = static_cast<Int32>(_fBlue);
	m_clearColour = D3DCOLOR_XRGB(newRed, newGreen, newBlue);
}

/**
*
* Sets the projection matrix.
*
* @param _rProjection: reference to a D3DXMATRIX that contains the projection matrix.
* @return void.
*
*/
void
CD3DRenderer::SetProjectionMatrix(D3DXMATRIX &_rProjection)
{
	m_matProjection = _rProjection;
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);
}

/**
*
* Sets the view matrix.
*
* @param _rView: reference to a D3DXMATRIX that contains the view matrix.
* @return void.
*
*/
void
CD3DRenderer::SetViewMatrix(D3DXMATRIX &_rView)
{
	m_matView = _rView;
	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
}

/**
*
* Sets the world matrix.
*
* @param _rWorld: reference to a D3DXMATRIX that contains the world matrix.
* @return void.
*
*/
void
CD3DRenderer::SetWorldMatrix(D3DXMATRIX &_rWorld)
{
	m_matWorld = _rWorld;
	m_pDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
}

/**
*
* Shuts down the renderer.
*
* @return Returns true if the function call succeeded.
*
*/
bool
CD3DRenderer::Shutdown()
{
	m_bShutDown = true;

	m_pDirect3D->Release();
	m_pDirect3D = 0;
	m_pDevice->Release();
	m_pDevice = 0;

	return (true);
}

/**
*
* prepares the renderer for a new render cycle and clears the specified buffers.
*
* @param _bTarget: clears the frame buffer.
* @param _bDepth: clears the depth buffer.
* @param _bStencil: clears the stencil buffer.
* @return void.
*
*/
void
CD3DRenderer::StartRender(bool _bTarget, bool _bDepth, bool _bStencil)
{
	HRESULT hr;
	if (!m_bScreenCleared)
	{
		Clear(_bTarget, _bDepth, _bStencil);
		m_bScreenCleared = true;
	}
	hr = m_pDevice->BeginScene();
	if (SUCCEEDED(hr))
	{
		m_bRenderingScene = true;
	}
}

/**
*
* Takes a "screen shot" of the application at run time.
*
* @param _pcFilename: Specifies the name of the file the
* back buffer will be saved to.
* @return void.
*
*/
void
CD3DRenderer::DumpBackBufferToDisk(const Int8 *_pcFilename)
{
	IDirect3DSurface9 *backBuffer = nullptr;
	if (FAILED(m_pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer)))
	{
		return;
	}

	// Convert ANSI/MB file name to wide string and call the wide API
	int required = MultiByteToWideChar(CP_ACP, 0, _pcFilename, -1, nullptr, 0);
	if (required > 0)
	{
		auto *tempWChar = new wchar_t[required];
		MultiByteToWideChar(CP_ACP, 0, _pcFilename, -1, tempWChar, required);

		HRESULT hr = D3DXSaveSurfaceToFileW(tempWChar, D3DXIFF_BMP, backBuffer, NULL, NULL);
		(void) hr; // preserve existing behavior; handle failure if needed

		delete[] tempWChar;
	}

	backBuffer->Release();
}

/**
*
* Returns a pointer to the current device.
*
* @return Returns a pointer to the current Direct 3d device.
*
*/
IDirect3DDevice9 *
CD3DRenderer::GetDevice()
{
	return (m_pDevice);
}

/**
*
* Sets up fog.
*
* @param _bFogState: If true, fog will be turned on. If false,
*		 fog will be turned off.
* @return void.
*
*/
void
CD3DRenderer::SetFog(bool _bFogState)
{
	D3DCAPS9 caps;
	ZeroMemory(&caps, sizeof(caps));

	// Use the default adapter constant and check return value in real code if needed.
	m_pDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);

	// Enabling pixel fog.
	if ((caps.RasterCaps & D3DPRASTERCAPS_FOGTABLE) == D3DPRASTERCAPS_FOGTABLE)
	{
		m_pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		m_pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB(255, 128, 64));
		m_pDevice->SetRenderState(D3DRS_FOGSTART, *reinterpret_cast<DWORD *>(&m_fFogStart));
		m_pDevice->SetRenderState(D3DRS_FOGEND, *reinterpret_cast<DWORD *>(&m_fFogEnd));
		m_pDevice->SetRenderState(D3DRS_FOGDENSITY, static_cast<DWORD>(kfFOG_DENSITY));
	} else
	{
		if (!_bFogState)
		{
			m_pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
	}
}



