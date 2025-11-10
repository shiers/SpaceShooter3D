#pragma once

#ifndef SPACESHOOTER3D_MESSAGEPROCESSOR_H
#define SPACESHOOTER3D_MESSAGEPROCESSOR_H

// Library Includes
#include <queue>
#include <map>
#include <d3d9.h>

// Local Includes
#include "defines.h"

// Types
typedef void (*functionPointer)(void);

// Constants
const int k_iMAX_XINPUT_CONTROLLERS = 4;

// Prototypes
class CLogManager;
class CLogD3DFB;

enum EMessage
{
	//All the key messages must go first.
	MESSAGE_PRESSED_KEY_LEFT,
	MESSAGE_PRESSED_KEY_RIGHT,
	MESSAGE_PRESSED_KEY_UP,
	MESSAGE_PRESSED_KEY_DOWN,
	MESSAGE_PRESSED_KEY_W,
	MESSAGE_PRESSED_KEY_S,
	MESSAGE_PRESSED_KEY_A,
	MESSAGE_PRESSED_KEY_D,
	MESSAGE_PRESSED_KEY_R,
	MESSAGE_PRESSED_KEY_F,
	MESSAGE_PRESSED_KEY_N,
	MESSAGE_PRESSED_KEY_M,
	MESSAGE_PRESSED_KEY_F7,
	MESSAGE_PRESSED_KEY_F8,
	MESSAGE_PRESSED_KEY_F9,
	MESSAGE_PRESSED_KEY_F10,
	MESSAGE_PRESSED_KEY_F11,
	MESSAGE_PRESSED_KEY_ENTER,
	MESSAGE_PRESSED_KEY_ESC,
	//Max key should never be sent.
	MAX_KEY,
	MESSAGE_MISC,
	MESSAGE_OBJECTINSTANTIATION,
	MESSAGE_ERROR_INPUT_SYSTEM_COLLAPSE,
	MESSAGE_ERROR_FATAL,
	MESSAGE_MOUSE_MOVE,
	MESSAGE_MOUSE_CLICK_LEFT,
	MESSAGE_MOUSE_CLICK_RIGHT,
	MESSAGE_XINPUT_CONTROLLER_FOUND,
	MESSAGE_DINPUT_CONTROLLER_FOUND,
	MESSAGE_XINPUT_CONTROLLER_RUMBLE_ON,
	MESSAGE_XINPUT_CONTROLLER_RUMBLE_OFF,
};

struct XInputInformation
{
	float fLeftControllerX;
	float fLeftControllerY;
	float fRightControllerX;
	float fRightControllerY;
	bool bAButton;
	bool bBackButton;
};

class CMessageProcessor
{
   //Member Functions
public:
	~CMessageProcessor();

	bool Initialise(IDirect3DDevice9* _pDevice);

	void SendMessage(const Int8* _pLogMessage, EMessage _eSystemMessage);
	void SendMessage(const Int8* _pLogMessage);
	void SendMessage(EMessage _eSystemMessage);

	void RegisterForMessage(EMessage _eSystemMessage, void (*FunctionToCall)(void));

	void Process(Float32 _fDeltaTick);

	void SetMouseX(Int32 _iX);
	void SetMouseY(Int32 _iY);
	int GetMouseX();
	int GetMouseY();

	void SetXInputControllerState(int _iID, XInputInformation* _pInformation);
	XInputInformation* GetXInputControllerState(int _iID);
	
	void Draw();

	static CMessageProcessor* GetInstance();
	static void DestroyInstance();

	bool CheckForKeyMessage(EMessage _eKeyDown);

protected:

private:
	CMessageProcessor();

	void CheckFrontMessage();

   //Member Variables
public:

protected:

private:
	std::queue<EMessage> m_messageQueue;

	//When a function registers for a specific message, it goes in here.
	std::map<Int32 , EMessage> m_messageArray;
	std::map<Int32 , functionPointer>  m_pFunctionArray;

	bool m_ArrayKeys[MAX_KEY];

	Int32 m_iNumberOfEntrys;
	Int32 m_iMouseX;
	Int32 m_iMouseY;

	CLogManager* m_pLogManager;
	CLogD3DFB* m_pScreenLogger;

	XInputInformation m_XInputInformation[k_iMAX_XINPUT_CONTROLLERS];

	static CMessageProcessor* m_pMessageProcessor;

};

#endif // SPACESHOOTER3D_MESSAGEPROCESSOR_H



