#pragma once

#ifndef __VIEWPORT_H__
#define __VIEWPORT_H__

// Library Includes

// Local Includes
#include "defines.h"
#include "camera.h"

// Types
typedef enum
{
 INVALID_SHADEMODE,
 SHADEMODE_1,
 SHADEMODE_2,
 MAX_SHADEMODE
}EShadeMode;

// Constants

// Prototypes

class IViewport
{
 // Member Functions
public:
	IViewport()
 {
  // Constructor.
 }

	virtual ~IViewport()
 {
  // Destructor.
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
 virtual bool Initialise(UInt32 _uiX,
       UInt32 _uiY,
       UInt32 _uiWidth,
       UInt32 _uiHeight,
       D3DCOLOR _ClearColor,
       IRenderer* _pRenderer,
       bool _bZClear = true) = 0;

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
 virtual ICamera* CreateCamera(Float32 _fFOV,
          ECameraMode _eMode,
          ECameraType _eType,
          Float32 _fNearClip,
          Float32 _fFarClip,
          UInt32& _ruiCameraID) = 0;

 /**
 *
 * This function returns a camera.
 *
 * @param _uiCameraID is the ID of the camera to be retrieved
 * @return Returns a pointer to an ICamera.
 *
 */
 virtual ICamera* GetCamera(UInt32 _uiCameraID) = 0;

 /**
 *
 * This function sets the active camera according to the specified ID.
 *
 * @param _uiCameraID is the ID of the camera to be retrieved
 * @return Returns true if successful.
 *
 */
 virtual bool SetActiveCamera(UInt32 _uiCameraID) = 0;

 /**
 *
 * This function returns the camera.
 *
 * @return Returns a pointer to an ICamera.
 *
 */
 virtual ICamera* GetActiveCamera() = 0;

 /**
 *
 * This function returns a D3DVIEWPORT9.
 *
 * @return Returns a pointer to a D3DVIEWPORT9.
 *
 */
 virtual D3DVIEWPORT9* GetD3DViewport() = 0;

 /**
 *
 * This function returns the viewport's width.
 *
 * @return Returns the viewport's width.
 *
 */
 virtual UInt32 GetWidth() const = 0;

 /**
 *
 * This function returns the viewport's height.
 *
 * @return Returns the viewport's height.
 *
 */
 virtual UInt32 GetHeight() const = 0;

 /**
 *
 * This function returns the viewport's X position.
 *
 * @return Returns the X position of the viewport.
 *
 */
 virtual UInt32 GetPosX() const = 0;

 /**
 *
 * This function returns the viewport's Y position.
 *
 * @return Returns the Y position of the viewport.
 *
 */
 virtual UInt32 GetPosY() const = 0;

 /**
 *
 * This function specifies whether or not the Z buffer should be cleared.
 *
 * @return Returns true if successful.
 *
 */
 virtual bool DoZClear() const = 0;

 /**
 *
 * This function gets the colour that the screen should be cleared to.
 *
 * @return Returns a D3DCOLOR which specifies the colour that the screen should
 *		  be cleared to.
 *
 */
 virtual D3DCOLOR GetClearColor() const = 0;

 /**
 *
 * This function sets the colour of the wire frame.
 *
 * @return Returns void.
 *
 */
 virtual void SetWireColor(D3DCOLORVALUE& _rColorWire) = 0;

 /**
 *
 * This function gets the colour of the wire frame.
 *
 * @return Returns a reference to a D3DCOLORVALUE.
 *
 */
 virtual D3DCOLORVALUE& GetWireColor() = 0;

 // Shade mode functions

 /**
 *
 * This function gets the shade mode.
 *
 * @return Returns a ShadeMode.
 *
 */
 virtual EShadeMode GetShadeMode() const  = 0;

 /**
 *
 * This function sets the shade mode.
 *
 * @return Returns void.
 *
 */
 virtual void SetShadeMode(EShadeMode _eShadeMode) = 0;

protected:

private:

 // Member Variables
public:

protected:

private:

};
#endif // __VIEWPORT_H__
