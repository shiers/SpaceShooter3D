#pragma once

#ifndef __SHOOTERENEMY_H__
#define __SHOOTERENEMY_H__

// Library Includes
#include "defines.h"

// Local Includes
#include "Enemy.h"
// Types

// Constants

// Prototypes

class CShooterEnemy : public CEnemy
{
	// Member Functions
public:
	CShooterEnemy();
	~CShooterEnemy();

	void Process(Float32 _fDeltatick);
	bool IsReadyToShoot();

protected:
	
private:

	// Member Variables
public:

protected:
	UInt8 m_ucShooterEnemyLife;
	static UInt16 m_usShooterEnemyValue;
private:

};
#endif // __SHOOTERENEMY_H__



