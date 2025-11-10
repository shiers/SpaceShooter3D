#include <d3d9.h>
#include <windows.h>

// Local Includes
#include "../include/logmanager.h"
#include "../include/logd3dfb.h"
#include "../include/logfile.h"
#include "../include/logoutputwindow.h"

// This Includes
#include "../include/messageprocessor.h"

// Static Variables
CMessageProcessor *CMessageProcessor::m_pMessageProcessor = 0;

// Static Function Prototypes

// Implementation
CMessageProcessor::CMessageProcessor()
	: m_iNumberOfEntrys(0)
	  , m_iMouseX(0)
	  , m_iMouseY(0)
{
	//Constructor.
	m_messageQueue.empty();
	ZeroMemory(m_ArrayKeys, MAX_KEY);
	ZeroMemory(&m_XInputInformation, sizeof(XInputInformation)*k_iMAX_XINPUT_CONTROLLERS);
}

CMessageProcessor::~CMessageProcessor()
{
	//Deconstructor.
	m_pLogManager->WriteLine("-------");
	m_pLogManager->WriteLine("END LOG");
	m_pLogManager->WriteLine("-------");
	delete m_pLogManager;
}

/**
*
* This function initialises the message processor
*
* @param _pDevice: Pointer to an IDirect3DDevice9* Interface.
* @return: Returns true if the function succeeded
*
*/
bool
CMessageProcessor::Initialise(IDirect3DDevice9 *_pDevice)
{
	bool bFailure = false;

	m_messageArray.clear();
	m_messageArray.empty();
	m_pFunctionArray.clear();
	m_pFunctionArray.empty();

	m_pScreenLogger = new CLogD3DFB;
	m_pScreenLogger->Initialise(_pDevice);

	CLogFile *pFileLogger = new CLogFile;
	CreateDirectoryA("log", NULL);
	pFileLogger->Initialise("log\\log.onx");

	CLogOutputWindow *pTempOutputWindowLogger = new CLogOutputWindow;

	m_pLogManager = new CLogManager;
	m_pLogManager->AddLogTarget(m_pScreenLogger);
	m_pLogManager->AddLogTarget(pFileLogger);
	m_pLogManager->AddLogTarget(pTempOutputWindowLogger);

	m_pLogManager->WriteLine("-----------");
	m_pLogManager->WriteLine("LOG STARTED");
	m_pLogManager->WriteLine("-----------");

	return (!bFailure);
}

/**
*
* A class can register a function with the message processor.
* If the message is sent, the message processor will call that
* function.
*
* @param _eSystemMessage: This is the message that will trigger the
* function call.
* @param FunctionToCall: This is the function that will be called.
* @return void.
*
*/
void
CMessageProcessor::RegisterForMessage(EMessage _eSystemMessage, void (*FunctionToCall)(void))
{
	m_messageArray[m_iNumberOfEntrys] = _eSystemMessage;
	m_pFunctionArray[m_iNumberOfEntrys] = FunctionToCall;

	++m_iNumberOfEntrys;
}

/**
*
* This function allows parts of the program to send messages
* to the message processor.
*
* @param _pLogMessage: Log Message which will be sent to the log manager.
* @param _eSystemMessage: System message which describes the log message.
* @return: void.
*
*/
void
CMessageProcessor::SendMessage(const char *_pLogMessage, EMessage _eSystemMessage)
{
	m_pLogManager->WriteLine(_pLogMessage);
	m_messageQueue.push(_eSystemMessage);

	for (int i = 0; i < m_iNumberOfEntrys; ++i)
	{
		if (m_messageArray[i] == _eSystemMessage)
		{
			//If something has registered for a message.
			//Call the corresponding function.
			m_pFunctionArray[i]();
		}
	}
}

/**
*
* This function allows parts of the program to send messages
* to the message processor.
*
* @param _pLogMessage: Log Message which will be sent to the log manager.
* @return: void.
*
*/
void
CMessageProcessor::SendMessage(const char *_pLogMessage)
{
	m_pLogManager->WriteLine(_pLogMessage);
}

/**
*
* This function allows parts of the program to send messages
* to the message processor.
*
* @param _eSystemMessage: System message which describes the log message.
* @return: void.
*
*/
void
CMessageProcessor::SendMessage(EMessage _eSystemMessage)
{
	m_messageQueue.push(_eSystemMessage);
}

/**
*
* This function processes the message manager.
*
* @param _fDeltaTick: Delta Tick component that represents
* the time elapsed since the previous frame
* @return: void.
*
*/
void
CMessageProcessor::Process(float _fDeltaTick)
{
#ifdef _DEBUG
	m_pScreenLogger->Process(_fDeltaTick);
#endif

	ZeroMemory(m_ArrayKeys, MAX_KEY);

	while (m_messageQueue.size() != 0)
	{
		for (int i = 0; i < m_iNumberOfEntrys; ++i)
		{
			if (m_messageArray[i] == m_messageQueue.front())
			{
				//If something has registered for a message.
				//Call the corresponding function.
				m_pFunctionArray[i]();
			}
		}

		CheckFrontMessage();

		m_messageQueue.pop();
	}
}

/**
*
* This function Draws the Screen Logger.
*
* @return: void.
*
*/
void
CMessageProcessor::Draw()
{
#ifdef _DEBUG
	m_pScreenLogger->Draw();
#endif
}

/**
*
* This function returns the instance of the Message Processor.
*
* @return: Returns the instance of the Message Processor.
*
*/
CMessageProcessor *
CMessageProcessor::GetInstance()
{
	if (!m_pMessageProcessor)
	{
		m_pMessageProcessor = new CMessageProcessor;
	}
	return (m_pMessageProcessor);
}

/**
*
* This function destroys the instance of the Message Processor.
*
* @return: void.
*
*/
void
CMessageProcessor::DestroyInstance()
{
	delete m_pMessageProcessor;
}

/**
*
* Checks the front message in the message queue. If
* It is a key press message, a boolean flag is set.
*
* @return void.
*
*/
void
CMessageProcessor::CheckFrontMessage()
{
	if (m_messageQueue.front() < MAX_KEY)
	{
		m_ArrayKeys[m_messageQueue.front()] = true;
	}
}

/**
*
* If the specified key has been pressed, this function returns true.
*
* @param _eKeyDown: The key to be checked.
* @return If the specified key has been pressed, this function returns true.
*
*/
bool
CMessageProcessor::CheckForKeyMessage(EMessage _eKeyDown)
{
	return (m_ArrayKeys[_eKeyDown]);
}


/**
*
* This function sets the current mouseX position.
* This function should only be touched by the input manager.
*
* @param _iX: The new mouseX coordinate.
* @return void.
*
*/
void
CMessageProcessor::SetMouseX(int _iX)
{
	m_iMouseX = _iX;
}

/**
*
* This function sets the current mouseY position.
* This function should only be touched by the input manager.
*
* @param _iY: The new mouseY coordinate.
* @return void.
*
*/
void
CMessageProcessor::SetMouseY(int _iY)
{
	m_iMouseY = _iY;
}

/**
*
* This function returns the current mouseX coordinate
*
* @return Returns the current mouseX coordinate.
*
*/
int
CMessageProcessor::GetMouseX()
{
	return (m_iMouseX);
}

/**
*
* This function returns the current mouseY coordinate
*
* @return Returns the current mouseY coordinate.
*
*/
int
CMessageProcessor::GetMouseY()
{
	return (m_iMouseY);
}

/**
*
* This function sets the XInput controller state.
*
* @param _iID: The ID of the XInput controller.
* @param _pInformation: Pointer to an XInputInformation structure
* that contains information about an XInput controller.
* @return void.
*
*/
void
CMessageProcessor::SetXInputControllerState(int _iID, XInputInformation *_pInformation)
{
	m_XInputInformation[_iID] = *_pInformation;
}

/**
*
* returns the state of the specified XInput controller
*
* @param _iID: The ID of the controller being queried.
* @return Returns a pointer to an XInputInformation stucture
* that contains data on the specified controller.
*
*/
XInputInformation *
CMessageProcessor::GetXInputControllerState(int _iID)
{
	return (&m_XInputInformation[_iID]);
}



