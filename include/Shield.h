#pragma once

#ifndef __SHIELD_H__
#define __SHIELD_H__

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>

// Local Includes
#include "defines.h"
#include "dynamicentity.h"

// Types

// Constants
const Float32 kf_DEFAULT_MAX_TICKER_VALUE = 512.0f;

// Prototypes

class CShield : public CDynamicEntity
{
	// Member Functions
public:
	CShield();
	virtual ~CShield();

	bool Initialise(CD3DRenderer* _pRenderer, 
					D3DXVECTOR3* _pPointToFollow,
					bool _bIsAlive = true,
					Float32 _fLifeTime = 5.0f);
	bool IsAlive();
	void SetLifeTime(Float32 _fLifeTime);

	virtual void Process(float _fDeltaTick);
	void Draw();

protected:

private:

	// Member Variables
public:

protected:
	D3DXVECTOR3* m_pPointToFollow;
	IDirect3DDevice9* m_pDevice;
	bool m_bIsAlive;
	Float32 m_fLifeTime;

	D3DXMATRIX m_matTransformation;

	ID3DXMesh* m_pD3DMesh;
	IDirect3DTexture9* m_pTexture;
	IDirect3DTexture9* m_pTexture2;

	Float32 m_fLooper;

private:

};
#endif // __SHIELD_H__



