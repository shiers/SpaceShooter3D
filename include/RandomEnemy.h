#pragma once

#ifndef __RANDOMENEMY_H__
#define __RANDOMENEMY_H__

// Library Includes

// Local Includes
#include "Enemy.h"
#include "defines.h"

// Types

// Constants

// Prototypes

class CRandomEnemy : public CEnemy
{
	// Member Functions
public:
	CRandomEnemy();
	~CRandomEnemy();

	void Process(Float32 _fDeltaTick);
	bool IsReadyToShoot();

protected:

private:

	// Member Variables
public:

protected:
	UInt8 m_ucRandomEnemyLife;
	static UInt16 m_usRandomEnemyValue;
protected:

private:

	// Member Variables
public:

protected:

private:

};
#endif // __RANDOMENEMY_H__



