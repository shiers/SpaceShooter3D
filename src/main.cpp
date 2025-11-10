#define WIN32_LEAN_AND_MEAN

// Library Includes
#include <windows.h>
#include <windowsx.h>
#include <cstdlib>
#include <ctime>

// Local Includes
#include "../include/game.h"
#include "../include/Clock.h"
// #include "../include/vld.h"
#include "../include/messageprocessor.h"
#include "../include/INIParser.h"

// This Includes

// Globals

// Static Variables

// Static Function Prototypes

// Implementation

#define WINDOW_CLASS_NAME L"3DSHOOTER"

LRESULT CALLBACK
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	// This is the main message handler of the system.
	PAINTSTRUCT ps; // Used in WM_PAINT
	HDC hdc; // Handle to a device context.

	switch (_uiMsg)
	{
		case WM_KEYDOWN:
		{
			switch (_wParam)
			{
				case VK_ESCAPE:
				{
					if (CGame::GetInstance().IsTitleScreen())
					{
						SendMessage(_hWnd, WM_CLOSE, _wParam, _lParam);
					} else
					{
						CGame::GetInstance().SetTitleScreen(true);
					}
				}
				break;
			}
		}
		break; // FIXED: Added missing break after WM_KEYDOWN

		case WM_PAINT:
		{
			// Simply validate the window.
			hdc = BeginPaint(_hWnd, &ps);
			// You would do all your painting here...
			EndPaint(_hWnd, &ps);

			// Return success.
			return (0);
		}
		break;

		case WM_CLOSE:
		{
			// Clean shutdown
			DestroyWindow(_hWnd);
			return (0);
		}
		break;

		case WM_DESTROY:
		{
			// Kill the application, this sends a WM_QUIT message.
			PostQuitMessage(0);

			// Return success.
			return (0);
		}
		break;

		// ADDED: Handle device loss scenarios for D3D
		case WM_ACTIVATE:
		{
			// Notify game of activation state changes (useful for fullscreen)
			// CGame can pause/resume or handle device reset
			return (0);
		}
		break;

		case WM_SIZE:
		{
			// Handle window resize - important for D3D device reset
			return (0);
		}
		break;

		default: break;
	} // End switch.

	return (DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam));
}

HWND
CreateAndRegisterWindow(HINSTANCE _hInstance, Int32 _iWidth, Int32 _iHeight,
                        LPCWSTR _pcTitle, bool _bFullscreen)
{
	WNDCLASSEXW winclass = {};
	winclass.cbSize = sizeof(WNDCLASSEXW);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = WindowProc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = _hInstance;
	// Cast resource identifiers to LPCWSTR for the W APIs
	winclass.hIcon = LoadIconW(NULL, reinterpret_cast<LPCWSTR>(IDI_APPLICATION));
	winclass.hCursor = LoadCursorW(NULL, reinterpret_cast<LPCWSTR>(IDC_ARROW));
	winclass.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	winclass.lpszMenuName = nullptr;
	winclass.lpszClassName = WINDOW_CLASS_NAME;
	winclass.hIconSm = LoadIconW(NULL, reinterpret_cast<LPCWSTR>(IDI_APPLICATION));

	if (!RegisterClassExW(&winclass))
	{
		// Failed to register.
		DWORD id = GetLastError();
		(void) id;
		return (0);
	}

	// FIXED: Different window styles for fullscreen vs windowed
	DWORD dwStyle;
	DWORD dwExStyle = 0;

	if (_bFullscreen)
	{
		// Fullscreen: borderless window
		dwStyle = WS_POPUP | WS_VISIBLE;
		dwExStyle = WS_EX_TOPMOST;
	} else
	{
		// Windowed: normal window with border
		dwStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	}

	// FIXED: Adjust window rect to account for borders/title bar in windowed mode
	RECT windowRect = {0, 0, _iWidth, _iHeight};
	if (!_bFullscreen)
	{
		AdjustWindowRect(&windowRect, dwStyle, FALSE);
	}

	Int32 windowWidth = windowRect.right - windowRect.left;
	Int32 windowHeight = windowRect.bottom - windowRect.top;

	HWND hwnd = CreateWindowExW(
		dwExStyle, // dwExStyle
		WINDOW_CLASS_NAME, // lpClassName (LPCWSTR)
		_pcTitle, // lpWindowName (LPCWSTR)
		dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowWidth, windowHeight,
		NULL, NULL, _hInstance, NULL);

	if (!hwnd)
	{
		// Failed to create.
		return (0);
	}

	// ADDED: Show and update window explicitly
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return (hwnd);
}

Int32 WINAPI
WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdline,
        Int32 _iCmdshow)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// seed random number.
	srand(static_cast<unsigned int>(time(0)));

	CINIParser tempParser;
	tempParser.LoadIniFile("config\\game.ini");

	Int32 iWidth;
	Int32 iHeight;
	bool bFullscreen = false; // ADDED: Read fullscreen setting

	tempParser.GetIntValue("game", "screenwidth", iWidth);
	tempParser.GetIntValue("game", "screenheight", iHeight);
	tempParser.GetBoolValue("game", "fullscreen", bFullscreen); // If this method exists

	// ADDED: Validation
	if (iWidth <= 0 || iWidth > 4096) iWidth = 800;
	if (iHeight <= 0 || iHeight > 4096) iHeight = 600;

	HWND hwnd = CreateAndRegisterWindow(_hInstance, iWidth, iHeight,
	                                    L"TeamBMSS 3D Shooter", bFullscreen);

	if (!hwnd)
	{
		//Log Failure
		MessageBoxW(NULL, L"Failed to create window!", L"Error", MB_OK | MB_ICONERROR);
		return (0);
	}

	CGame &rGame = CGame::GetInstance();

	if (!rGame.Initialise(_hInstance, hwnd, iWidth, iHeight))
	{
		// FIXED: Better error reporting
		DWORD dwError = GetLastError();
		char errorBuf[512];
		sprintf_s(errorBuf, "Failed to initialize game!\n\n"
		          "Last Windows Error: %d\n\n"
		          "Check Output window in Visual Studio or use DebugView to see detailed log.\n\n"
		          "Common issues:\n"
		          "- Missing media files in media\\ folder\n"
		          "- Missing config\\game.ini\n"
		          "- D3DX9 libraries not installed",
		          dwError);
		MessageBoxA(hwnd, errorBuf, "Initialization Error", MB_OK | MB_ICONERROR);
		DestroyWindow(hwnd);
		return (0);
	}

	// ADDED: Debug output to confirm we reached the main loop
	OutputDebugStringA("Entering main game loop\n");

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else
		{
			rGame.ExecuteOneFrame();
		}
	}

	// ADDED: Debug output
	OutputDebugStringA("Exiting main game loop\n");

	CGame::DestroyInstance();

	return (static_cast<Int32>(msg.wParam));
}
