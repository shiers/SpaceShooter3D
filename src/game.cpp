#include <cassert>
#include <cmath>

// Local Includes
#include "../include/Clock.h"
#include "../include/renderer.h"
#include "../include/D3DRenderer.h"
#include "../include/camera.h"
#include "../include/igviewport.h"
#include "../include/DebugCamera.h"
#include "../include/topdowncamera.h"
#include "../include/messageprocessor.h"
#include "../include/playerinput.h"
#include "../include/mesh.h"
#include "../include/titlescreen.h"
#include "../include/INIParser.h"
#include "../include/level.h"

// This Includes
#include "../include/game.h"

// Static Variables
CGame *CGame::s_pGame = 0;

// Static Function Prototypes
static bool FileExists(const wchar_t *filename)
{
	struct _stat buffer;
	return (_wstat(filename, &buffer) == 0);
}

#define RANDOMCOLOUR D3DCOLOR_XRGB((rand() % 255), (rand() % 255), (rand() % 255))

// Implementation
CGame::CGame()
	: m_pClock(0)
	  , m_fDegrees(0)
	  , m_pD3DRenderer(0)
	  , m_pMessageProcessor(0)
	  , m_pPlayerInput(0)
	  , m_pTitleScreen(0)
	  , m_arraypViewports(0)
	  , m_bIsTitleScreen(0)
	  , m_pIniParser(0)
	  , m_pLevel(0)
	  , m_bWasBackButtonPressedLastFrame(0)
	  , m_bBoundingKeyPressed(0)
	  , m_bWireKeyPressed(0)
	  , m_bDebugKeyPressed(0)
	  , m_bIsFullScreen(true)
{
	//This is private, and so should never be called outside game.
}

CGame::~CGame()
{
	delete m_pClock;
	m_pClock = 0;
	delete m_pD3DRenderer;
	m_pD3DRenderer = 0;
	delete m_pTitleScreen;
	m_pTitleScreen = 0;

	CMessageProcessor::DestroyInstance();
	m_pMessageProcessor = 0;

	CPlayerInput::DestroyInstance();

	delete m_pIniParser;
	m_pIniParser = 0;

	delete[] m_arraypViewports;
	m_arraypViewports = 0;

	delete m_pLevel;
	m_pLevel = 0;
}

CGame::CGame(const CGame &_kr)
{
	//This is private, and so should never be called.
}

CGame &
CGame::operator=(const CGame &)
{
	//This is private, and so should never be called.
	return (*s_pGame);
}

/**
*
* Initialises the CGame class.
*
* @param _hInstance: handle to the application instance.
* @param _hWnd: handle to the active window.
* @param _iWidth: Width of the window, in pixels.
* @param _iHeight: Height of the window, in pixels.
* @return Returns true if the function succeeds.
*
*/
bool
CGame::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	OutputDebugStringA("=== CGame::Initialise START ===\n");

	bool bFailure = false;
	m_bIsTitleScreen = true;
	// Setting member data.
	m_iWindowWidth = _iWidth;
	m_iWindowHeight = _iHeight;
	m_hApplicationInstance = _hInstance;
	m_hMainWindow = _hWnd;

	OutputDebugStringA("CGame: Creating Clock...\n");
	m_pClock = new CClock();
	m_pClock->Initialise();
	m_pClock->Process();

	if (!m_pIniParser)
	{
		OutputDebugStringA("CGame: Loading INI Parser...\n");
		m_pIniParser = new CINIParser;
		m_pIniParser->LoadIniFile("config\\game.ini");
		m_pIniParser->GetBoolValue("game", "fullscreen", m_bIsFullScreen);

		char buf[256];
		sprintf_s(buf, "CGame: Fullscreen = %s\n", m_bIsFullScreen ? "TRUE" : "FALSE");
		OutputDebugStringA(buf);
	}

	OutputDebugStringA("CGame: Initializing Renderer...\n");
	if (InitialiseRenderer())
	{
		OutputDebugStringA("CGame: Renderer initialized successfully!\n");

		OutputDebugStringA("CGame: Initializing Logging...\n");
		if (!InitialiseLogging())
		{
			OutputDebugStringA("CGame: ERROR - Failed to initialize logging!\n");
			bFailure = true;
		} else
		{
			CMessageProcessor::GetInstance()->SendMessage("GAME:Initialised Logging Successfully.");

			OutputDebugStringA("CGame: Initializing Viewport...\n");
			if (!InitialiseViewport())
			{
				OutputDebugStringA("CGame: ERROR - Failed to initialize viewport!\n");
				bFailure = true;
			} else
			{
				CMessageProcessor::GetInstance()->SendMessage("GAME:Initialised Viewport Successfully.");

				OutputDebugStringA("CGame: Initializing Input...\n");
				if (!InitialiseInput())
				{
					OutputDebugStringA("CGame: ERROR - Failed to initialize input!\n");
					bFailure = true;
				} else
				{
					CMessageProcessor::GetInstance()->SendMessage("GAME:Initialised Input Successfully.");

					OutputDebugStringA("CGame: Initializing Level...\n");
					if (!InitialiseLevel())
					{
						OutputDebugStringA("CGame: ERROR - Failed to initialize level!\n");
						bFailure = true;
					} else
					{
						CMessageProcessor::GetInstance()->SendMessage("GAME:Initialised Level Successfully.");
					}
				}
			}
		}
	} else
	{
		OutputDebugStringA("CGame: ERROR - Failed to initialize renderer!\n");
		bFailure = true;
	}

	if (!bFailure)
	{
		OutputDebugStringA("=== CGame::Initialise SUCCESS ===\n");
	} else
	{
		OutputDebugStringA("=== CGame::Initialise FAILED ===\n");
	}

	return (!bFailure);
}

/**
*
* Executes one frame of the game.
*
* @return void.
*
*/
void
CGame::ExecuteOneFrame()
{
	static int frameCount = 0;
	frameCount++;
	// Safety check
	if (!m_pD3DRenderer || !m_pClock)
	{
		OutputDebugStringA("ExecuteOneFrame: ERROR - Critical objects null!\n");
		return;
	}

	if (frameCount <= 5)
	{
		char buf[128];
		sprintf_s(buf, "ExecuteOneFrame: Frame %d\n", frameCount);
		OutputDebugStringA(buf);
	}

	// EXECUTE ONE FRAME
	m_pClock->Process();
	float fDT = m_pClock->GetDeltaTick();

	m_pD3DRenderer->StartRender(true, true, false);

	Process(fDT);

	// Safety check for viewports
	if (m_arraypViewports)
	{
		for (Int32 i = 0; i < MAX_PLAYERS; ++i)
		{
			m_arraypViewports[i].ProcessActiveCamera(fDT);

			m_pD3DRenderer->Clear(true, true, false);

			Draw();
		}
	} else
	{
		OutputDebugStringA("ExecuteOneFrame: WARNING - No viewports!\n");
		m_pD3DRenderer->Clear(true, true, false);
	}

	m_pD3DRenderer->EndRender();

	Sleep(1);
}

/**
*
* Processes everything in the game.
*
* @param _fDeltaTick: elapsed time (in seconds) since the previous frame.
* @return void.
*
*/
void
CGame::Process(float _fDeltaTick)
{
	// Safety checks
	if (!m_pMessageProcessor)
	{
		OutputDebugStringA("Process: ERROR - MessageProcessor is null!\n");
		return;
	}

	if (!m_pPlayerInput)
	{
		OutputDebugStringA("Process: ERROR - PlayerInput is null!\n");
		return;
	}

	//PROCESS STUFF HERE
	ProcessKeyboardControls(_fDeltaTick);

	m_pMessageProcessor->Process(_fDeltaTick);

	m_pPlayerInput->Process(_fDeltaTick);

	if (m_bIsTitleScreen)
	{
#ifdef _DEBUG
		if (m_arraypViewports)
		{
			m_arraypViewports[1].SetActiveCamera(0);
		}
#endif
		m_pD3DRenderer->GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);

		CTopDownCamera *pTempCamera = 0;
		D3DXMATRIX tempMatrix;
		D3DXMatrixIdentity(&tempMatrix);
		for (int i = 0; i < MAX_PLAYERS; ++i)
		{
			pTempCamera = reinterpret_cast<CTopDownCamera *>(m_arraypViewports[i].GetActiveCamera());
			pTempCamera->SetHeight(0);
			pTempCamera->SetFollow(&tempMatrix);
			pTempCamera->SetViewMatrix(tempMatrix);
		}
		if (m_pLevel)
		{
			delete m_pLevel;
			m_pLevel = 0;
		}
		if (m_pTitleScreen)
		{
			ProcessTitleScreen(_fDeltaTick);
		}
	} else
	{
		m_pD3DRenderer->GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
		if (m_pLevel)
		{
			m_pLevel->Process(_fDeltaTick);
		}
	}
	// Controller back button handling
	if (m_pMessageProcessor->GetXInputControllerState(0)->bBackButton)
	{
		if (!m_bWasBackButtonPressedLastFrame)
		{
			SendMessage(m_hMainWindow, WM_KEYDOWN, VK_ESCAPE, 0);
		}
	}
	m_bWasBackButtonPressedLastFrame = m_pMessageProcessor->GetXInputControllerState(0)->bBackButton;
}

/**
*
* Draws everything in the game.
*
* @return void.
*
*/
void
CGame::Draw()
{
	// Safety checks
	if (!m_pMessageProcessor)
	{
		return;
	}
	//DRAW STUFF HERE
	if (m_bIsTitleScreen)
	{
		if (m_pTitleScreen)
		{
			m_pTitleScreen->Draw();
		} else
		{
			// No title screen - just show a colored background for testing
			// The screen is already cleared in ExecuteOneFrame
		}
	} else
	{
		if (m_pLevel)
		{
			m_pLevel->Draw();
		}
	}

	m_pMessageProcessor->Draw();
}

/**
*
* Returns the application instance.
*
* @return Returns a handle to the instance of the application
*
*/
HINSTANCE
CGame::GetAppInstance()
{
	return (m_hApplicationInstance);
}

/**
*
* Returns the instance of CGame
*
* @return Returns a reference to the instance of CGame.
*
*/
CGame &
CGame::GetInstance()
{
	if (s_pGame == 0)
	{
		s_pGame = new CGame;
	}
	return (*s_pGame);
}

/**
*
* Returns a handle to the active window.
*
* @return Returns a handle to the active window.
*
*/
HWND
CGame::GetWindow()
{
	return (m_hMainWindow);
}

/**
*
* Destroys the current instance of CGame. Must be
* called to prevent memory leaks.
*
*
*/
void
CGame::DestroyInstance()
{
	if (s_pGame != 0)
	{
		delete s_pGame;
		s_pGame = 0;
	}
}

/**
*
* Returns the height of the active window.
*
* @return returns the height of the active window, in
* pixels.
*
*/
int
CGame::GetWindowHeight()
{
	return (m_iWindowHeight);
}

/**
*
* Returns the width of the active window.
*
* @return returns the width of the active window, in
* pixels.
*
*/
int
CGame::GetWindowWidth()
{
	return (m_iWindowWidth);
}

/**
*
* Returns the game's clock.
*
* @return returns a pointer to the game's clock.
*
*/
CClock *
CGame::GetClock()
{
	return (m_pClock);
}

/**
*
* Processes the keyboard controls for CGame.
*
* @param _fDeltaTick: elapsed time (in seconds) since the previous frame.
* @return void.
*
*/
void
CGame::ProcessKeyboardControls(float _fDeltaTick)
{
	if (!m_pD3DRenderer || !CMessageProcessor::GetInstance())
	{
		return;
	}

	if (CMessageProcessor::GetInstance()->CheckForKeyMessage(MESSAGE_PRESSED_KEY_F11))
	{
		m_pD3DRenderer->DumpBackBufferToDisk("ScreenShot1.bmp");
	}
#ifdef _DEBUG
	if (CMessageProcessor::GetInstance()->CheckForKeyMessage(MESSAGE_PRESSED_KEY_F9))
	{
		if (!m_bBoundingKeyPressed)
		{
			CMesh::ToggleBoundingVolumes();
		}
		m_bBoundingKeyPressed = true;
	} else
	{
		m_bBoundingKeyPressed = false;
	}
	if (CMessageProcessor::GetInstance()->CheckForKeyMessage(MESSAGE_PRESSED_KEY_F8))
	{
		if (!m_bWireKeyPressed)
		{
			CMesh::ToggleWireframe();
		}
		m_bWireKeyPressed = true;
	} else
	{
		m_bWireKeyPressed = false;
	}
	if (CMessageProcessor::GetInstance()->CheckForKeyMessage(MESSAGE_PRESSED_KEY_F7))
	{
		if (!m_bDebugKeyPressed)
		{
			if (!m_bIsTitleScreen)
			{
				m_arraypViewports[1].SetActiveCamera(1);
			}
		}
		m_bDebugKeyPressed = true;
	} else
	{
		m_bDebugKeyPressed = false;
	}
#endif
}

/**
*
* Initialises the in game renderer
*
* @return Returns true if the function succeeded.
*
*/
bool
CGame::InitialiseRenderer()
{
	OutputDebugStringA("  InitialiseRenderer: START\n");
	bool bFailure = false;

	if (!m_pD3DRenderer)
	{
		OutputDebugStringA("  InitialiseRenderer: Creating CD3DRenderer...\n");
		m_pD3DRenderer = new CD3DRenderer;
		if (!m_pD3DRenderer)
		{
			OutputDebugStringA("  InitialiseRenderer: First allocation failed, retrying...\n");
			// Initialisation failure. Retry.
			m_pD3DRenderer = new CD3DRenderer;
			if (!m_pD3DRenderer)
			{
				OutputDebugStringA("  InitialiseRenderer: Second allocation failed! FATAL\n");
				// Initialisation failure. Death.
				bFailure = true;
				return (!bFailure);
			}
		}
	}

	if (m_bIsFullScreen)
	{
		OutputDebugStringA("  InitialiseRenderer: Hiding cursor for fullscreen\n");
		ShowCursor(false);
	}

	char buf[256];
	sprintf_s(buf, "  InitialiseRenderer: Calling D3DRenderer->Initialise(%d, %d, hwnd, %s)...\n",
	          m_iWindowWidth, m_iWindowHeight, m_bIsFullScreen ? "FULLSCREEN" : "WINDOWED");
	OutputDebugStringA(buf);

	bool tempBool = m_pD3DRenderer->Initialise(m_iWindowWidth, m_iWindowHeight, m_hMainWindow, m_bIsFullScreen);

	if (!tempBool)
	{
		OutputDebugStringA("  InitialiseRenderer: D3DRenderer->Initialise FAILED!\n");
		bFailure = true;
	} else
	{
		OutputDebugStringA("  InitialiseRenderer: D3DRenderer->Initialise succeeded!\n");
		OutputDebugStringA("  InitialiseRenderer: Setting fog...\n");
		m_pD3DRenderer->SetFog(ON);
		OutputDebugStringA("  InitialiseRenderer: Fog set successfully\n");
	}

	if (!bFailure)
	{
		OutputDebugStringA("  InitialiseRenderer: SUCCESS\n");
	} else
	{
		OutputDebugStringA("  InitialiseRenderer: FAILED\n");
	}

	return (!bFailure);
}

/**
*
* Initialises the level(s).
*
* @return Returns true if the function call succeeded.
*
*/
bool
CGame::InitialiseLevel()
{
	OutputDebugStringA("=== InitialiseLevel: START ===\n");
	bool bFailure = false;

	CMessageProcessor::GetInstance()->SendMessage("GAME:Initialising Level...");

	// INITIALISE LEVEL.
	if (!m_pTitleScreen)
	{
		OutputDebugStringA("InitialiseLevel: Creating CTitleScreen...\n");
		m_pTitleScreen = new CTitleScreen;
		if (!m_pTitleScreen)
		{
			OutputDebugStringA("InitialiseLevel: ERROR - Failed to allocate CTitleScreen!\n");
			MessageBoxA(NULL, "Failed to allocate CTitleScreen", "Error", MB_OK);
			return false;
		}
	}

	D3DXVECTOR3 tempTitlePosition;
	ZeroMemory(&tempTitlePosition, sizeof(D3DXVECTOR3));
	D3DXVECTOR3 tempTargetPosition;
	ZeroMemory(&tempTargetPosition, sizeof(D3DXVECTOR3));
	tempTitlePosition.x = kfDEFAULT_TITLE_START_X;
	tempTitlePosition.y = kfDEFAULT_TITLE_START_Y;
	tempTitlePosition.z = kfDEFAULT_TITLE_START_Z;
	tempTargetPosition.x = kfDEFAULT_TARGET_START_X;
	tempTargetPosition.y = kfDEFAULT_TARGET_START_Y;
	tempTargetPosition.z = kfDEFAULT_TARGET_START_Z;

	if (m_pIniParser)
	{
		OutputDebugStringA("InitialiseLevel: Reading positions from INI...\n");
		m_pIniParser->GetFloatValue("titlescreen", "titlestartpositionx", tempTitlePosition.x);
		m_pIniParser->GetFloatValue("titlescreen", "titlestartpositiony", tempTitlePosition.y);
		m_pIniParser->GetFloatValue("titlescreen", "titlestartpositionz", tempTitlePosition.z);
		m_pIniParser->GetFloatValue("titlescreen", "targetstartpositionx", tempTargetPosition.x);
		m_pIniParser->GetFloatValue("titlescreen", "targetstartpositiony", tempTargetPosition.y);
		m_pIniParser->GetFloatValue("titlescreen", "targetstartpositionz", tempTargetPosition.z);
	}

	// CHECK IF FILES EXIST BEFORE ATTEMPTING TO LOAD
	OutputDebugStringA("InitialiseLevel: Checking for required media files...\n");

	const wchar_t *requiredFiles[] = {
		L"media\\titlescreen.x",
		L"media\\titlescreen.png",
		L"media\\target.x",
		L"media\\target.png"
	};

	bool allFilesExist = true;
	for (int i = 0; i < 4; i++)
	{
		if (FileExists(requiredFiles[i]))
		{
			char buf[256];
			sprintf_s(buf, "InitialiseLevel: Found: %S\n", requiredFiles[i]);
			OutputDebugStringA(buf);
		} else
		{
			char buf[256];
			sprintf_s(buf, "InitialiseLevel: MISSING: %S\n", requiredFiles[i]);
			OutputDebugStringA(buf);
			allFilesExist = false;
		}
	}

	if (!allFilesExist)
	{
		OutputDebugStringA("InitialiseLevel: ERROR - Some media files are missing!\n");
		MessageBoxA(NULL,
		            "Missing media files!\n\n"
		            "Required files:\n"
		            "- media\\titlescreen.x\n"
		            "- media\\titlescreen.png\n"
		            "- media\\target.x\n"
		            "- media\\target.png\n\n"
		            "Create dummy files or comment out InitialiseLevel to test rendering.",
		            "Media Files Missing",
		            MB_OK | MB_ICONERROR);

		// OPTION 1: Return false (current behavior)
		// return false;

		// OPTION 2: Continue without title screen (for testing D3D)
		OutputDebugStringA("InitialiseLevel: Continuing without title screen for testing...\n");
		return true;
	}

	OutputDebugStringA("InitialiseLevel: All files found, initializing title screen...\n");
	bFailure = !m_pTitleScreen->Initialise(L"media\\titlescreen.x",
	                                       L"media\\titlescreen.png",
	                                       L"media\\target.x",
	                                       L"media\\target.png",
	                                       tempTargetPosition,
	                                       tempTitlePosition,
	                                       1.6f,
	                                       m_pD3DRenderer->GetDevice());

	if (bFailure)
	{
		OutputDebugStringA("InitialiseLevel: ERROR - CTitleScreen->Initialise FAILED!\n");
		MessageBoxA(NULL,
		            "CTitleScreen->Initialise() failed!\n\n"
		            "This could be due to:\n"
		            "- Corrupt mesh files (.x files)\n"
		            "- Invalid texture files\n"
		            "- D3DX library issues",
		            "Title Screen Init Failed",
		            MB_OK | MB_ICONERROR);
	} else
	{
		OutputDebugStringA("InitialiseLevel: SUCCESS - Title screen initialized!\n");
	}

	return (!bFailure);
}

/**
*
* Initialises the Viewport(s).
*
* @return Returns true if the function call succeeded.
*
*/
bool
CGame::InitialiseViewport()
{
	OutputDebugStringA("=== InitialiseViewport: START ===\n");
	CMessageProcessor::GetInstance()->SendMessage("GAME:Initialising Viewports...");

	bool bFailure = false;

	if (!m_arraypViewports)
	{
		OutputDebugStringA("InitialiseViewport: Allocating viewport array...\n");
		m_arraypViewports = new CIGViewport[MAX_PLAYERS];

		for (Int32 i = 0; i < MAX_PLAYERS; ++i)
		{
			char buf[128];
			sprintf_s(buf, "InitialiseViewport: Initializing viewport %d...\n", i);
			OutputDebugStringA(buf);

			if (!i)
			{
				bFailure = !m_arraypViewports[i].Initialise(0,
				                                            0,
				                                            m_iWindowWidth,
				                                            m_iWindowHeight,
				                                            D3DCOLOR_XRGB(0, 0, 0),
				                                            m_pD3DRenderer);
			} else
			{
				bFailure = !m_arraypViewports[i].Initialise(0,
				                                            m_iWindowHeight,
				                                            m_iWindowWidth,
				                                            m_iWindowHeight / 2,
				                                            D3DCOLOR_XRGB(0, 0, 0),
				                                            m_pD3DRenderer);
			}

			if (bFailure)
			{
				sprintf_s(buf, "InitialiseViewport: ERROR - Viewport %d failed to initialize!\n", i);
				OutputDebugStringA(buf);
				CMessageProcessor::GetInstance()->SendMessage("GAME:     ERROR: COULD NOT INITIALISE VIEWPORTS.");
				MessageBoxA(NULL, buf, "Viewport Init Failed", MB_OK);
				break;
			}

			OutputDebugStringA("InitialiseViewport: Creating cameras...\n");

			UInt32 uiCameraId = 0;
			CTopDownCamera *ptempCamera = reinterpret_cast<CTopDownCamera *>(
				m_arraypViewports[i].CreateCamera(
					D3DXToRadian(60.0f),
					CAMMODE_PERSPECTIVE,
					CAMERA_TOPDOWN,
					0.01f,
					200.0f,
					uiCameraId));

			if (!ptempCamera)
			{
				sprintf_s(buf, "InitialiseViewport: ERROR - Failed to create camera for viewport %d!\n", i);
				OutputDebugStringA(buf);
				MessageBoxA(NULL, buf, "Camera Creation Failed", MB_OK);
				bFailure = true;
				break;
			}

			ptempCamera->SetDistance(0);
			ptempCamera->SetHeight(0);
			D3DXMATRIX tempMatrix;
			D3DXMatrixIdentity(&tempMatrix);
			ptempCamera->SetFollow(&tempMatrix);

			m_arraypViewports[i].SetActiveCamera(0);
			sprintf_s(buf, "InitialiseViewport: Viewport %d initialized successfully\n", i);
			OutputDebugStringA(buf);
		}

#ifdef _DEBUG
		if (!bFailure)
		{
			OutputDebugStringA("InitialiseViewport: Creating debug camera...\n");
			UInt32 CamID = 1;
			CDebugCamera *tempCamera = static_cast<CDebugCamera *>(
				m_arraypViewports[1].CreateCamera(D3DXToRadian(60.0f),
				                                  CAMMODE_PERSPECTIVE,
				                                  CAMERA_DEBUG,
				                                  0.01f,
				                                  200.0f,
				                                  CamID));

			if (tempCamera)
			{
				OutputDebugStringA("InitialiseViewport: Debug camera created\n");
			} else
			{
				OutputDebugStringA("InitialiseViewport: WARNING - Debug camera creation failed\n");
			}
		}
#endif
	}

	if (!bFailure)
	{
		OutputDebugStringA("=== InitialiseViewport: SUCCESS ===\n");
		CMessageProcessor::GetInstance()->SendMessage("GAME:Initialised Viewport Successfully.");
	} else
	{
		OutputDebugStringA("=== InitialiseViewport: FAILED ===\n");
	}

	return (!bFailure);
}

/**
*
* Initialises the logging system.
*
* @return Returns true if the function call succeeded.
*
*/
bool
CGame::InitialiseLogging()
{
	bool bFailure = false;

	m_pMessageProcessor = CMessageProcessor::GetInstance();
	m_pMessageProcessor->Initialise(m_pD3DRenderer->GetDevice());

	return (!bFailure);
}


/**
*
* Initialises the input system.
*
* @return Returns true if the function call succeeded.
*
*/
bool
CGame::InitialiseInput()
{
	CMessageProcessor::GetInstance()->SendMessage("GAME:Initialising Input...");
	bool bFailure = false;
	m_pPlayerInput = &CPlayerInput::GetInstance();
	if (!m_pPlayerInput)
	{
		bFailure = true;
		CMessageProcessor::GetInstance()->SendMessage("GAME:     ERROR: Instance of Player Input not found.");
	} else
	{
		bFailure = !m_pPlayerInput->Initialise();
		if (bFailure)
		{
			CMessageProcessor::GetInstance()->SendMessage("GAME:     ERROR: Player input failed to initialise.");
		}
	}
	return (!bFailure);
}

/**
*
* Processes the title screen for CGame.
*
* @param _fDeltaTick: elapsed time (in seconds) since the previous frame.
* @return void.
*
*/
void
CGame::ProcessTitleScreen(float _fDeltaTick)
{
	// Safety check
	if (!m_pTitleScreen)
	{
		OutputDebugStringA("ProcessTitleScreen: ERROR - TitleScreen is null!\n");
		return;
	}

	if (!m_pMessageProcessor)
	{
		OutputDebugStringA("ProcessTitleScreen: ERROR - MessageProcessor is null!\n");
		return;
	}

	if (!m_arraypViewports)
	{
		OutputDebugStringA("ProcessTitleScreen: ERROR - Viewports array is null!\n");
		return;
	}

	m_pTitleScreen->Process(_fDeltaTick);

	bool EnterPressed = m_pMessageProcessor->CheckForKeyMessage(MESSAGE_PRESSED_KEY_ENTER);
	XInputInformation *tempInformation = m_pMessageProcessor->GetXInputControllerState(0);
	bool ControllerAPressed = tempInformation->bAButton;

	int ViewportZeroHeight = m_arraypViewports[0].GetHeight();
	D3DVIEWPORT9 *tempViewPort = m_arraypViewports[0].GetD3DViewport();
	bool bViewportChanged = false;

	if (m_pTitleScreen->GetSelection() == TITLE_START_2_PLAYER)
	{
		//Split the screen.
		if (ViewportZeroHeight > (m_iWindowHeight / 2))
		{
			tempViewPort->Height -= static_cast<DWORD>(m_iWindowHeight / 2 * _fDeltaTick);
			m_arraypViewports[0].RecalculateProjectionMatrix();
			bViewportChanged = true;
		}
		if (static_cast<int>(tempViewPort->Height) < m_iWindowHeight / 2)
		{
			tempViewPort->Height = m_iWindowHeight / 2;
			m_arraypViewports[0].RecalculateProjectionMatrix();
			bViewportChanged = true;
		}
	} else
	{
		if (ViewportZeroHeight < m_iWindowHeight - 1)
		{
			tempViewPort->Height += static_cast<DWORD>(m_iWindowHeight / 2 * _fDeltaTick);
			m_arraypViewports[0].RecalculateProjectionMatrix();
			bViewportChanged = true;
		}
		if (static_cast<int>(tempViewPort->Height) > m_iWindowHeight - 1)
		{
			tempViewPort->Height = m_iWindowHeight - 1;
			m_arraypViewports[0].RecalculateProjectionMatrix();
			bViewportChanged = true;
		}
	}

	if (EnterPressed || ControllerAPressed)
	{
		switch (m_pTitleScreen->GetSelection())
		{
			case TITLE_START_1_PLAYER:
			{
				tempViewPort->Height = m_iWindowHeight;
				m_arraypViewports[0].RecalculateProjectionMatrix();
				bViewportChanged = true;
				if (!m_pLevel)
				{
					m_pLevel = new CLevel;
				}
				assert(m_pLevel);
				CTopDownCamera *tempCameraArray[MAX_PLAYERS];
				for (UInt8 i = 0; i < MAX_PLAYERS; ++i)
				{
					tempCameraArray[i] = reinterpret_cast<CTopDownCamera *>(m_arraypViewports[i].GetActiveCamera());
				}
				m_pLevel->Initialise(1, tempCameraArray, m_pD3DRenderer);
				m_bIsTitleScreen = false;
			}
			break;
			case TITLE_START_2_PLAYER:
			{
				tempViewPort->Height = m_iWindowHeight / 2;
				m_arraypViewports[0].RecalculateProjectionMatrix();
				bViewportChanged = true;
				if (!m_pLevel)
				{
					m_pLevel = new CLevel;
				}
				assert(m_pLevel);
				CTopDownCamera *tempCameraArray[MAX_PLAYERS];
				m_pClock->GetDeltaTick();
				for (UInt8 i = 0; i < MAX_PLAYERS; ++i)
				{
					tempCameraArray[i] = reinterpret_cast<CTopDownCamera *>(m_arraypViewports[i].GetActiveCamera());
				}
				m_pLevel->Initialise(2, tempCameraArray, m_pD3DRenderer);
				m_bIsTitleScreen = false;
				m_pClock->GetDeltaTick();
			}
			break;

			case TITLE_EXIT:
			{
				SendMessage(m_hMainWindow, WM_CLOSE, 0, 0);
			}
			break;
		}
	}

	if (bViewportChanged == true)
	{
		D3DVIEWPORT9 *tempViewPort2 = m_arraypViewports[1].GetD3DViewport();
		tempViewPort2->Y = tempViewPort->Height - 1;
		tempViewPort2->Height = m_iWindowHeight - tempViewPort->Height + 1;
		if (tempViewPort2->Height > 0)
		{
			m_arraypViewports[1].RecalculateProjectionMatrix();
		}
	}
}

bool
CGame::IsTitleScreen()
{
	return (m_bIsTitleScreen);
}

void
CGame::SetTitleScreen(bool _bIsTitleScreen)
{
	m_bIsTitleScreen = _bIsTitleScreen;
}

void
CGame::ResetClock()
{
	m_pClock->Process();
}



