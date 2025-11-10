#pragma once

#ifndef SPACESHOOTER3D_GAME_H
#define SPACESHOOTER3D_GAME_H

// Library Includes
#include <d3dx9.h>
#include <windows.h>
#include <windowsx.h>
#include <vector>

// Local Includes
#include "defines.h"

// Types
typedef enum
{
	INVALID_GAME_ATTRIBUTES,
	MAX_PLAYERS = 2,
	MAX_GAME_ATTRIBUTES,
}EGameAttributes;

// Constants
#define ON 1
#define OFF 0

const float kfDEFAULT_TITLE_START_X = 0.0f;
const float kfDEFAULT_TITLE_START_Y = 0.0f;
const float kfDEFAULT_TITLE_START_Z = 15.0f;
const float kfDEFAULT_TARGET_START_X = -7.0f;
const float kfDEFAULT_TARGET_START_Y = 1.8f;
const float kfDEFAULT_TARGET_START_Z = 10.0f;

// Prototypes
class CClock;
class CBackBuffer;
class CD3DRenderer;
class CDebugCamera;
class CTopDownCamera;
class CIGViewport;
class CDynamicEntity;
class CINIParser;
class CMessageProcessor;
class CPlayerInput;
class CBillboard;
class CMesh;
class CTitleScreen;
class CLevel;

class CGame
{
   //Member Functions
public:
	~CGame();

	bool Initialise(HINSTANCE _hInstance, HWND _hwnd, int _iWidth, int _iHeight);

	bool IsTitleScreen();
	void SetTitleScreen(bool _bIsTitleScreen);

	void Draw();
	void Process(float _fDeltaTick);
	void ProcessTitleScreen(float _fDeltaTick);
	void ProcessKeyboardControls(float _fDeltaTick);
	void ExecuteOneFrame();

	void ResetClock();

	int					GetWindowHeight();
	int					GetWindowWidth();
	HINSTANCE			GetAppInstance();
	HWND				GetWindow();
	CClock*				GetClock();

	//Singleton methods
	static CGame& GetInstance();
	static void DestroyInstance();
	
protected:

private:
	CGame();
	CGame(const CGame& _kr);
	CGame& operator=(const CGame&);

	bool InitialiseLevel();
	bool InitialiseRenderer();
	bool InitialiseViewport();
	bool InitialiseLogging();
	bool InitialiseInput();

   //Member Variables
public:

protected:
	
private:
	static CGame* s_pGame;

	CClock* m_pClock;
	CPlayerInput* m_pPlayerInput;
	CLevel* m_pLevel;
	
	CD3DRenderer* m_pD3DRenderer;
	HINSTANCE m_hApplicationInstance;
	HWND m_hMainWindow;

	CINIParser* m_pIniParser;

	CMessageProcessor* m_pMessageProcessor;

	CTitleScreen* m_pTitleScreen;

	CIGViewport* m_arraypViewports;

	int m_iViewPortX;
	int m_iViewPortY;
	int m_iWindowWidth;
	int m_iWindowHeight;

	float m_fDegrees;

	bool m_bIsTitleScreen;
	bool m_bBoundingKeyPressed;
	bool m_bWireKeyPressed;
	bool m_bDebugKeyPressed;
	bool m_bWasBackButtonPressedLastFrame;
	bool m_bIsFullScreen;
};

#endif // SPACESHOOTER3D_GAME_H



