#pragma once

#ifndef SPACESHOOTER3D_CLOCK_H
#define SPACESHOOTER3D_CLOCK_H

// Library Includes

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes

class CClock
{
	// Member Functions
public:
	CClock();
	virtual ~CClock();

	virtual bool Initialise();
	
	virtual void Process();
	
	Float32 GetDeltaTick();

protected:

private:
	CClock(const CClock& _kr);
	CClock& operator=(const CClock& _kr);

	// Member Variables
public:

protected:
	Float32 m_fTimeElapsed;
	Float32 m_fDeltaTime;
	Float32 m_fLastTime;
	Float32 m_fCurrentTime;

private:

};
#endif // SPACESHOOTER3D_CLOCK_H



