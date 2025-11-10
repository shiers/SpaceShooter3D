#include "../include/game.h"
#include "../include/defines.h"

// This Includes
#include "../include/directinputcontroller.h"

// Static Variables
IDirectInput8 *CDirectInputController::m_pDirectInputInstance = 0;
int CDirectInputController::m_iNumControllers = 0;

BOOL CALLBACK Callback(LPCDIDEVICEINSTANCE _pDeviceInstance, LPVOID _pVoidPointer);

// Static Function Prototypes

// Implementation

CDirectInputController::CDirectInputController()
	: m_ControllerType(INVALID_CONTROLLERTYPE)
	  , m_pLocalDirectInputInstance(0)
	  , m_Devclass(0)
{
	++m_iNumControllers;
}

CDirectInputController::~CDirectInputController()
{
	--m_iNumControllers;
	if (!m_iNumControllers)
	{
		m_pDirectInputInstance->Release();
	}
	m_pDevice->Unacquire();
	m_pDevice->Release();
}

/**
*
* This function Initialises the Direct Input controller.
*
* @return Returns true if successful.
*
*/
bool
CDirectInputController::Initialise()
{
	bool bFailure = false;
	m_pLocalDirectInputInstance = CDirectInputController::GetDirectInputInstance();

	m_DeviceDescription = new DescribeMe;
	m_DeviceDescription->pToMe = this;
	m_DeviceDescription->myControllerType = m_ControllerType;
	m_DeviceDescription->bAttachedToComputer = false;

	m_pLocalDirectInputInstance->EnumDevices(m_Devclass,
	                                         Callback,
	                                         static_cast<void *>(m_DeviceDescription),
	                                         DIEDFL_ATTACHEDONLY);

	return (!bFailure);
}


BOOL CALLBACK
Callback(LPCDIDEVICEINSTANCE _pDeviceInstance, LPVOID _pVoidPointer)
{
	//If this function is hit, it means that the device was found.
	HRESULT hr;
	bool bFailure = false;

	DescribeMe *DeviceDescription = reinterpret_cast<DescribeMe *>(_pVoidPointer);

	//Now all we have to do is create a device, and let a sub class of CDirectInputController
	//handle the rest.
	IDirectInputDevice8 *tempDevice;
	hr = CDirectInputController::GetDirectInputInstance()->CreateDevice(_pDeviceInstance->guidProduct, &tempDevice,
	                                                                    NULL);
	CHECKHRESULT(hr, bFailure);
	if (!bFailure)
	{
		//Everything has gone fine.
		DeviceDescription->bAttachedToComputer = true;
		DeviceDescription->pDevice = tempDevice;
	}

	return (!bFailure);
}

/**
*
* This function gets an instance of the direct input controller.
*
* @return Returns a pointer to an instance of the direct input controller.
*
*/
IDirectInput8 *
CDirectInputController::GetDirectInputInstance()
{
	if (!m_pDirectInputInstance)
	{
		HRESULT hr = DirectInput8Create(CGame::GetInstance().GetAppInstance(),
		                                DIRECTINPUT_VERSION,
		                                IID_IDirectInput8,
		                                reinterpret_cast<void **>(&m_pDirectInputInstance),
		                                NULL);
	}
	return (m_pDirectInputInstance);
}



