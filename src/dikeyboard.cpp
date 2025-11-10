#include "../include/defines.h"
#include "../include/game.h"
#include "../include/messageprocessor.h"

// This Includes
#include "../include/dikeyboard.h"

// Static Variables

// Static Function Prototypes

// Implementation

CDIKeyboard::CDIKeyboard()
{
	//Constructor.
}

CDIKeyboard::~CDIKeyboard()
{
	//Deconstructor.
	delete m_DeviceDescription;
	delete[] m_cKeyboardStatus;
}

/**
*
* This function Initialises the Direct Input keyboard.
*
* @return Returns true if successful.
*
*/
bool
CDIKeyboard::Initialise()
{
	//Data member initialisation.
	bool bFailure = false;
	m_ControllerType = CONTROLLER_KEYBOARD;
	HRESULT hr;
	m_Devclass = DI8DEVCLASS_KEYBOARD;

	//Generic Initialisation.
	bFailure = !CDirectInputController::Initialise();

	if (bFailure)
	{
		CMessageProcessor::GetInstance()->SendMessage(
			"INPUT:     ERROR: CDirectInputController Initialisation failure.");
	} else
	{
		bFailure = !m_DeviceDescription->bAttachedToComputer;
		if (bFailure)
		{
			CMessageProcessor::GetInstance()->SendMessage("INPUT:     ERROR: Keyboard not attached to computer.");
		} else
		{
			m_pDevice = m_DeviceDescription->pDevice;
			hr = m_pDevice->SetDataFormat(&c_dfDIKeyboard);
			CHECKHRESULT(hr, bFailure);
			if (bFailure)
			{
				CMessageProcessor::GetInstance()->SendMessage("INPUT     ERROR: Failed to set data format");
			} else
			{
				hr = m_pDevice->SetCooperativeLevel(CGame::GetInstance().GetWindow(),
				                                    DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
				CHECKHRESULT(hr, bFailure);
				if (bFailure)
				{
					CMessageProcessor::GetInstance()->SendMessage("INPUT:     ERROR: Failed to set cooperative level");
				}
			}
		}
	}

	m_cKeyboardStatus = new char[ki_KEYBOARD_BUFFER_SIZE];

	return (!bFailure);
}

/**
*
* This function does any processing that the DI Keyboard requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CDIKeyboard::Process(float _fDeltaTick)
{
	HRESULT hr;
	ZeroMemory(m_cKeyboardStatus, ki_KEYBOARD_BUFFER_SIZE - 1);
	hr = m_pDevice->GetDeviceState(ki_KEYBOARD_BUFFER_SIZE, m_cKeyboardStatus);
	if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
	{
		if (hr == DIERR_INPUTLOST)
		{
			CMessageProcessor::GetInstance()->SendMessage("INPUT: Lost Device!");
		}
		hr = m_pDevice->Acquire();
		if (SUCCEEDED(hr))
		{
			CMessageProcessor::GetInstance()->SendMessage("INPUT: Device Regained.");
		}
	}
	ProcessKeyboardStatus();
}

/**
*
* This function checks important keys and sends messages off to the
* message processor.
*
* @return void.
*
*/
void
CDIKeyboard::ProcessKeyboardStatus()
{
	//MUST BE DECLARED HERE FOR MACRO TO WORK.
	CMessageProcessor *pMessageProcessor = CMessageProcessor::GetInstance();

	IFKEYAPRESSEDTHENB(DIK_LEFTARROW, MESSAGE_PRESSED_KEY_LEFT);
	IFKEYAPRESSEDTHENB(DIK_RIGHTARROW, MESSAGE_PRESSED_KEY_RIGHT);
	IFKEYAPRESSEDTHENB(DIK_UPARROW, MESSAGE_PRESSED_KEY_UP);
	IFKEYAPRESSEDTHENB(DIK_DOWNARROW, MESSAGE_PRESSED_KEY_DOWN);
	IFKEYAPRESSEDTHENB(DIK_A, MESSAGE_PRESSED_KEY_A);
	IFKEYAPRESSEDTHENB(DIK_S, MESSAGE_PRESSED_KEY_S);
	IFKEYAPRESSEDTHENB(DIK_W, MESSAGE_PRESSED_KEY_W);
	IFKEYAPRESSEDTHENB(DIK_D, MESSAGE_PRESSED_KEY_D);
	IFKEYAPRESSEDTHENB(DIK_R, MESSAGE_PRESSED_KEY_R);
	IFKEYAPRESSEDTHENB(DIK_F, MESSAGE_PRESSED_KEY_F);
	IFKEYAPRESSEDTHENB(DIK_M, MESSAGE_PRESSED_KEY_M);
	IFKEYAPRESSEDTHENB(DIK_N, MESSAGE_PRESSED_KEY_N);
	IFKEYAPRESSEDTHENB(DIK_F7, MESSAGE_PRESSED_KEY_F7);
	IFKEYAPRESSEDTHENB(DIK_F8, MESSAGE_PRESSED_KEY_F8);
	IFKEYAPRESSEDTHENB(DIK_F9, MESSAGE_PRESSED_KEY_F9);
	IFKEYAPRESSEDTHENB(DIK_F10, MESSAGE_PRESSED_KEY_F10);
	IFKEYAPRESSEDTHENB(DIK_F11, MESSAGE_PRESSED_KEY_F11);
	IFKEYAPRESSEDTHENB(DIK_RETURN, MESSAGE_PRESSED_KEY_ENTER);
	IFKEYAPRESSEDTHENB(DIK_ESCAPE, MESSAGE_PRESSED_KEY_ESC);
}



