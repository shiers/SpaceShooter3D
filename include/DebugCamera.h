#pragma once

#ifndef __DEBUGCAMERA_H__
#define __DEBUGCAMERA_H__

// Library Includes

// Local Includes
#include "igcamera.h"

// Types

// Constants

// Prototypes
class CIGCamera;

class CDebugCamera : public CIGCamera
{
	// Member Functions
public:
	CDebugCamera();
	~CDebugCamera();

	void Process (float _fDeltaTick);


protected:

private:

	// Member Variables
public:

protected:

private:

};
#endif // __DEBUGCAMERA_H__



