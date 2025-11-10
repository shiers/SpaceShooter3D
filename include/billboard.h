#pragma once

#ifndef SPACESHOOTER3D_BILLBOARD_H
#define SPACESHOOTER3D_BILLBOARD_H

// Library Includes

// Local Includes
#include "3dobject.h"
#include "defines.h"

// Types

// Constants

// Prototypes
class CMesh;
class CD3DRenderer;

class CBillboard : public C3DObject
{
   //Member Functions
public:
	
	CBillboard();
	~CBillboard();

	virtual bool Initialise(CMesh* _pMesh, CD3DRenderer* _pRenderer);
	virtual bool Initialise(CD3DRenderer* _pRenderer, Float32 _fWidth,
							Float32 _fHeight, Float32 _fDepth,
							const WChar16* _ptexFilename);
	virtual void Process(Float32 _fDeltaTick);

protected:

private:

   //Member Variables
public:

protected:

private:

};

#endif // SPACESHOOTER3D_BILLBOARD_H



