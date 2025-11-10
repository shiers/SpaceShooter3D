#define DIRECTINPUT_VERSION 0x0800
#define INITGUID
#include <d3d9.h>
#include <xinput.h>

// Local Includes
#include "../include/directinputcontroller.h"
#include "../include/dikeyboard.h"
#include "../include/messageprocessor.h"
#include "../include/xinputcontroller.h"

// This Includes
#include "../include/playerinput.h"

// Static Variables
CPlayerInput *CPlayerInput::s_pPlayerInput = 0;

// Static Function Prototypes

// Implementation

CPlayerInput::CPlayerInput()
	: m_iNumDIControllers(0)
{
	for (int i = 0; i < ki_MAX_NUM_CONTROLLERS; ++i)
	{
		m_pXInputController[i] = 0;
		m_pvecDInputControllers[i] = 0;
	}
}

CPlayerInput::~CPlayerInput()
{
	for (int i = 0; i < ki_MAX_NUM_CONTROLLERS; ++i)
	{
		if (m_pXInputController[i] != 0)
		{
			delete m_pXInputController[i];
			m_pXInputController[i] = 0;
		}
		if (m_pvecDInputControllers[i] != 0)
		{
			delete m_pvecDInputControllers[i];
			m_pvecDInputControllers[i] = 0;
			--m_iNumDIControllers;
		}
	}
}

/**
*
* This function Initialises the player Input.
*
* @return Returns true if successful.
*
*/
bool
CPlayerInput::Initialise()
{
	bool bFailure = false;

	CDIKeyboard *pKeyboard = new CDIKeyboard;
	bFailure = !pKeyboard->Initialise();
	if (bFailure)
	{
		CMessageProcessor::GetInstance()->SendMessage("ERROR:\tDINPUT: Keyboard initialisation failure.");
	} else
	{
		m_pvecDInputControllers[m_iNumDIControllers] = pKeyboard;
		++m_iNumDIControllers;
	}

	EnumerateControllers();

	return (!bFailure);
}

/**
*
* This function does any processing that the player input requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CPlayerInput::Process(float _fDeltaTick)
{
	for (int i = 0; i < m_iNumDIControllers; ++i)
	{
		if (m_pvecDInputControllers[i] != 0)
		{
			m_pvecDInputControllers[i]->Process(_fDeltaTick);
		}
		if (m_pXInputController[i] != 0)
		{
			m_pXInputController[i]->Process(_fDeltaTick);
		}
	}

	EnumerateControllers();
}

/**
*
* This function gets an instance of the player input.
*
* @return Returns an instance of the player input.
*
*/
CPlayerInput &
CPlayerInput::GetInstance()
{
	if (!s_pPlayerInput)
	{
		s_pPlayerInput = new CPlayerInput;
		s_pPlayerInput->Initialise();
	}

	return (*s_pPlayerInput);
}

/**
*
* This function destroys an instance of the player input.
*
* @return Returns void.
*
*/
void
CPlayerInput::DestroyInstance()
{
	delete s_pPlayerInput;
	s_pPlayerInput = 0;
}

/**
*
* This function enumerate the controllers.
*
* @return Returns void.
*
*/
void
CPlayerInput::EnumerateControllers()
{
	XINPUT_STATE tempState;
	DWORD hr;
	for (unsigned int i = 0; i < ki_MAX_NUM_CONTROLLERS; ++i)
	{
		hr = XInputGetState(i, &tempState);
		if (hr == ERROR_SUCCESS && m_pXInputController[i] == 0)
		{
			//Create the controller.
			m_pXInputController[i] = new CXInputController;
			m_pXInputController[i]->Initialise(i);
		}
	}
}



