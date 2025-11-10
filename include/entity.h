#pragma once

#ifndef SPACESHOOTER3D_ENTITY_H
#define SPACESHOOTER3D_ENTITY_H

// Library Includes

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes

class IEntity
{
   //Member Functions
public:
	IEntity()
	{
	}
	virtual ~IEntity()
	{
	}
	
	/**
	*
	* This function processes the entity by specified amount.
	*
	* @param _fDeltaTick: The change in time since the previous frame.
	*
	*/
	virtual void Process(Float32 _fDeltaTick) = 0;

protected:

private:

   //Member Variables
public:

protected:

private:

};

#endif // SPACESHOOTER3D_ENTITY_H



