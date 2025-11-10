#pragma once

#ifndef __SEEKERENEMY_H__
#define __SEEKERENEMY_H__

// Library Includes

// Local Includes
#include "enemy.h"
#include "defines.h"

// Types

// Constants

// Prototypes

class CSeekerEnemy : public CEnemy
{
	// Member Functions
public:
	CSeekerEnemy();
	~CSeekerEnemy();

	void Process(Float32 _fDeltaTick);
	virtual bool IsReadyToShoot();
	
protected:

private:

	// Member Variables
public:

protected:
	UInt8 m_ucSeekerEnemyLife;
	static UInt16 m_usSeekerEnemyValue;

private:

};
#endif // __SEEKERENEMY_H__



