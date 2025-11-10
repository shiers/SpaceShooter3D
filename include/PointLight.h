#pragma once

#ifndef __POINTLIGHT_H__
#define __POINTLIGHT_H__

// Library Includes

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes

class CPointLight
{
	// Member Functions
public:
	CPointLight();
	~CPointLight();

	bool Initialise(CD3DRenderer* _pRenderer,
					D3DXVECTOR3* _pVec3Position,
					Float32 _fSafeZoneRadius,
					UInt8 _id);
		
	void Process(Float32 _fDeltaTick);

protected:

private:

	// Member Variables
public:

protected:
	D3DXVECTOR3* m_pVec3Position;
	CD3DRenderer* m_pRenderer;

	D3DLIGHT9 m_Light;

	UInt8 m_ucId;
private:

};
#endif // __POINTLIGHT_H__



