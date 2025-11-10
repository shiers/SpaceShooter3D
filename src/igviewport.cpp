#include <cassert>

// Local Includes
#include "../include/igcamera.h"
#include "../include/DebugCamera.h"
#include "../include/D3DRenderer.h"
#include "../include/topdowncamera.h"

// This Includes
#include "../include/igviewport.h"

// Static Variables

// Static Function Prototypes

// Implementation
CIGViewport::CIGViewport()
	: m_pD3DViewport(0)
	  , m_ClearColor(0)
	  , m_eShadeMode(INVALID_SHADEMODE)
	  , m_bZClear(true)
	  , m_uiActiveCamera(0)
{
	// Constructor
}

CIGViewport::~CIGViewport()
{
	// Deconstructor
	delete m_pD3DViewport;
	m_pD3DViewport = 0;

	for (UInt32 i = 0; i < m_mapCameras.size(); ++i)
	{
		delete m_mapCameras[i];
		m_mapCameras[i] = 0;
	}

	for (UInt32 i = 0; i < m_mapCameras.size(); ++i)
	{
		delete m_mapCameras[i];
		m_mapCameras[i] = 0;
	}
}

/**
*
* This function Initialises the Viewport.
*
* @param _uiX.
* @param _uiY.
* @param _uiWidth is the width of the viewport.
* @param _uiHeight is the height of the viewport.
* @param _ClearColor is the colour that the screen gets cleared to.
* @param _bZClear is a boolen that dictates whether or not the Z buffer
*		 should be cleared.
* @return Returns true if successful.
*
*/
bool
CIGViewport::Initialise(UInt32 _uiX,
                        UInt32 _uiY,
                        UInt32 _uiWidth,
                        UInt32 _uiHeight,
                        D3DCOLOR _ClearColor,
                        IRenderer *_pRenderer,
                        bool _bZClear)
{
	assert(_pRenderer);
	m_pRenderer = _pRenderer;

	m_pD3DViewport = new D3DVIEWPORT9;
	assert(m_pD3DViewport);

	// Set members.
	m_pD3DViewport->X = _uiX;
	m_pD3DViewport->Y = _uiY;
	m_pD3DViewport->Width = _uiWidth;
	m_pD3DViewport->Height = _uiHeight;
	m_pD3DViewport->MinZ = 0.0f;
	m_pD3DViewport->MaxZ = 1.0f;

	m_ClearColor = _ClearColor;
	m_bZClear = _bZClear;

	return (true);
}

/**
*
* This function Creates a camera.
*
* @param _fFOV This is the Field of view.
* @param _eMode This is the Camera Mode (ie. Perspective).
* @param _eType This is the type of camera (ie. Debug).
* @param _fNearClip This is the the Near clipping value.
* @param _fFarClip This is the Far clipping value.
* @param _ruiCameraID is the ID of the camera being created
*		 should be cleared.
* @return Returns a pointer to an ICamera.
*
*/
ICamera *
CIGViewport::CreateCamera(Float32 _fFOV,
                          ECameraMode _eMode,
                          ECameraType _eType,
                          Float32 _fNearClip,
                          Float32 _fFarClip,
                          UInt32 &_ruiCameraID)
{
	CIGCamera *pIGCamera = 0;

	switch (_eType)
	{
		case CAMERA_DEBUG:
		{
			pIGCamera = new CDebugCamera;
		}
		break;

		case CAMERA_TOPDOWN:
		{
			pIGCamera = new CTopDownCamera;
		}
		break;

		default:
		{
			// Log error.
		}
		break;
	}
	assert(pIGCamera);

	// Initialise the camera.
	pIGCamera->Initialise(this, _fFOV, _eMode, _eType, _fNearClip, _fFarClip, _ruiCameraID, m_pRenderer);
	pIGCamera->RecalculateProjection();
	m_mapCameras[_ruiCameraID] = pIGCamera;

	// Return the camera.
	return (pIGCamera);
}

/**
*
* This function returns a camera.
*
* @param _uiCameraID is the ID of the camera to be retrieved
* @return Returns a pointer to an ICamera.
*
*/
ICamera *
CIGViewport::GetCamera(UInt32 _uiCameraID)
{
	// Return the requested camera.
	return (m_mapCameras[_uiCameraID]);
}

/**
*
* This function deletes a camera.
*
* @param _uiCameraID is the ID of the camera to be deleted.
* @return Returns void.
*
*/
void
CIGViewport::DeleteCamera(UInt32 _uiCameraID)
{
	delete m_mapCameras[_uiCameraID];
	m_mapCameras[_uiCameraID] = 0;
}

/**
*
* This function sets the active camera according to the specified ID.
*
* @param _uiCameraID is the ID of the camera to be retrieved
* @return Returns true if successful.
*
*/
bool
CIGViewport::SetActiveCamera(UInt32 _uiCameraID)
{
	// Set the requested camera to be the active camera.
	m_uiActiveCamera = _uiCameraID;
	return (true);
}

/**
*
* This function returns the camera.
*
* @return Returns a pointer to an ICamera.
*
*/
ICamera *
CIGViewport::GetActiveCamera()
{
	// Return the active camera.
	return (m_mapCameras[m_uiActiveCamera]);
}

/**
*
* This function processes the active camera.
*
* @return Returns void.
*
*/
void
CIGViewport::ProcessActiveCamera(Float32 _fDeltaTick)
{
	reinterpret_cast<CD3DRenderer *>(m_pRenderer)->GetDevice()->SetViewport(m_pD3DViewport);
	m_mapCameras[m_uiActiveCamera]->Process(_fDeltaTick);
}

/**
*
* This function returns a D3DVIEWPORT9.
*
* @return Returns a pointer to a D3DVIEWPORT9.
*
*/
D3DVIEWPORT9 *
CIGViewport::GetD3DViewport()
{
	return (m_pD3DViewport);
}

/**
*
* This function returns the viewport's width.
*
* @return Returns the viewport's width.
*
*/
UInt32
CIGViewport::GetWidth() const
{
	return (m_pD3DViewport->Width);
}

/**
*
* This function returns the viewport's height.
*
* @return Returns the viewport's height.
*
*/
UInt32
CIGViewport::GetHeight() const
{
	return (m_pD3DViewport->Height);
}

/**
*
* This function returns the viewport's X position.
*
* @return Returns the X position of the viewport.
*
*/
UInt32
CIGViewport::GetPosX() const
{
	return (m_pD3DViewport->X);
}

/**
*
* This function returns the viewport's Y position.
*
* @return Returns the Y position of the viewport.
*
*/
UInt32
CIGViewport::GetPosY() const
{
	return (m_pD3DViewport->Y);
}

/**
*
* This function specifies whether or not the Z buffer should be cleared.
*
* @return Returns true if successful.
*
*/
bool
CIGViewport::DoZClear() const
{
	return (m_bZClear);
}

/**
*
* This function gets the colour that the screen should be cleared to.
*
* @return Returns a D3DCOLOR which specifies the colour that the screen should
*		  be cleared to.
*
*/
D3DCOLOR
CIGViewport::GetClearColor() const
{
	return (m_ClearColor);
}

/**
*
* This function sets the colour of the wire frame.
*
* @return Returns void.
*
*/
void
CIGViewport::SetWireColor(D3DCOLORVALUE &_rColorWire)
{
	m_rColorWire = _rColorWire;
}

/**
*
* This function gets the colour of the wire frame.
*
* @return Returns a reference to a D3DCOLORVALUE.
*
*/
D3DCOLORVALUE &
CIGViewport::GetWireColor()
{
	return (m_rColorWire);
}

/**
*
* This function gets the shade mode.
*
* @return Returns a ShadeMode.
*
*/
EShadeMode
CIGViewport::GetShadeMode() const
{
	return (m_eShadeMode);
}

/**
*
* This function sets the shade mode.
* @param _eShadeMode is the shade mode to be set.
* @return Returns void.
*
*/
void
CIGViewport::SetShadeMode(EShadeMode _eShadeMode)
{
	m_eShadeMode = _eShadeMode;
}

/**
*
* This function Recalculates the Projection Matrix.
*
* @return Returns void.
*
*/
void
CIGViewport::RecalculateProjectionMatrix()
{
	m_mapCameras[m_uiActiveCamera]->RecalculateProjection();
}



