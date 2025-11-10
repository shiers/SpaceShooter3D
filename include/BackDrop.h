#pragma once

#ifndef __BACKDROP_H__
#define __BACKDROP_H__

// Library Includes

// Local Includes
#include "defines.h"
#include "3dobject.h"

// Types

// Constants

// Prototypes

class CBackDrop : public C3DObject
{
	// Member Functions
public:
	CBackDrop();
	~CBackDrop();

	virtual bool Initialise(const WChar16* _pFileName,
							const WChar16* _pTextureFileName,
							CD3DRenderer* _pRenderer,
							Float32 _fWidth,
							Float32 _fDepth);

protected:

private:

	// Member Variables
public:

protected:

private:

};
#endif // __BACKDROP_H__



