#pragma once

#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "particleemitter.h"

// Types

// Constants

// Prototypes

class CExplosion : public CParticleEmitter
{
	// Member Functions
public:
	CExplosion();
	virtual ~CExplosion();

	virtual bool Initialise(D3DXVECTOR3* _position, 
				 D3DCOLOR _colour, 
				 int _iNumberOfParticles, 
				 IDirect3DDevice9* _pDevice,
				 float _fLifeTime,
				 float _fSpeed);

	void ResetParticles();

protected:

private:

	// Member Variables
public:

protected:

private:

};
#endif // __EXPLOSION_H__



