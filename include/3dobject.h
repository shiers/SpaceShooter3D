#pragma once

#ifndef __STATICENTITY_H__
#define __STATICENTITY_H__

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "defines.h"
#include "entity.h"


// Types

// Constants

// Prototypes
class CMesh;
class CD3DRenderer;


class C3DObject : public IEntity
{
   //Member Functions
public:

	C3DObject();
	virtual ~C3DObject();

	virtual bool Initialise(CMesh* _pMesh, CD3DRenderer* _pRenderer);
	virtual bool Initialise(const WChar16* _pFileName, const WChar16* _pTextureFileName, CD3DRenderer* _pRenderer);

	virtual void Process(Float32 _fDeltaTick);
	virtual void Draw();

	void SetScale(Float32 _fAmount);
	void SetXYZ(Float32 _fX, Float32 _fY, Float32 _fZ);
	Float32 GetX();
	Float32 GetY();
	Float32 GetZ();

	void Yaw(Float32 _fAmount);
	void Pitch(Float32 _fAmount);
	void Roll(Float32 _fAmount);

	D3DXMATRIX* GetMatrix();

protected:

private:

   //Member Variables
public:

protected:
	D3DXMATRIX m_matWorld;
	CMesh* m_pMesh;
	CD3DRenderer* m_pRenderer{};

	Float32 m_fYaw;
	Float32 m_fPitch;
	Float32 m_fRoll;

private:

};

#endif //___STATICENTITY_H__

