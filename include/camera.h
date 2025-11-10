#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>

// Local Includes
#include "defines.h"

// Types
typedef enum
{
 INVALID_CAMERAMODE,
 CAMMODE_ORTHOGONAL,
 CAMMODE_PERSPECTIVE,
 MAX_CAMERAMODE
} ECameraMode;

typedef enum
{
 INVALID_CAMERATYPE,
 CAMERA_DEBUG,
 CAMERA_TOPDOWN,
 MAX_CAMERATYPE
} ECameraType;

typedef enum
{
 INVALID_CAMERA_ATTRIBUTE,
 ROTATION_MOVE = 5,
 CAMERA_MOVE = 100,
 MAX_CAMERA_ATTRIBUTE
} ECameraAttribute;

// Prototypes
class IViewport;
class IRenderer;

class ICamera
{
 // Member Functions
public:
 ICamera() = default;

 virtual ~ICamera() = default;

 /**
 *
 * This function initialises the camera system.
 *
 */
 virtual bool Initialise(IViewport *_pViewport,
                         Float32 _fFOV,
                         ECameraMode _eMode,
                         ECameraType _Type,
                         Float32 _fNearClip,
                         Float32 _fFarClip,
                         UInt32 _uiID,
                         IRenderer *_pRenderer) = 0;

 virtual void Process(Float32 _fDeltaTick) = 0;

 virtual void SetClipPlanes(Float32 _fNear, Float32 _fFar) = 0;

 virtual Float32 GetNearPlane() const = 0;

 virtual Float32 GetFarPlane() const = 0;

 virtual void SetFOV(Float32 _fFOV) = 0;

 virtual void SetView3D(D3DXVECTOR3 &_rvectRight,
                        D3DXVECTOR3 &_rvectUp,
                        D3DXVECTOR3 &_rvectDir,
                        D3DXVECTOR3 &_rvectPos) = 0;

 virtual void SetViewLookAt(D3DXVECTOR3 &_rvectPos,
                            D3DXVECTOR3 &_rvectLook,
                            D3DXVECTOR3 &_rvectUp) = 0;

 virtual void SetWorldMatrix(D3DXMATRIX &_rmatWorld) = 0;

 virtual void SetViewMatrix(D3DXMATRIX &_rmatView) = 0;

 virtual void SetProjMatrix(D3DXMATRIX &_rmatProj) = 0;

 virtual void SetMode(ECameraMode _eMode) = 0;

 virtual void SetType(ECameraType _eType) = 0;

 virtual void CalcViewProj() = 0;

 virtual void CalcWorldViewProj() = 0;

 virtual void ResetMatrices() = 0;

 virtual D3DXVECTOR3 &GetPosition() = 0;

 virtual D3DXMATRIX *GetViewMatrix() = 0;

 virtual D3DXMATRIX *GetProjMatrix() = 0;

 virtual D3DXMATRIX *GetWorldMatrix() = 0;

 virtual void UpdateRenderer() = 0;

protected:

private:
 // Member Variables
public:

protected:

private:
};
#endif // __CAMERA_H__
