#pragma once

#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

// Library Includes
#include <vector>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
class IEntity;

class CEntityManager
{
	// Member Functions
public:
	CEntityManager();
	~CEntityManager();

	bool Initialise();
	void Process(Float32 _fDeltatick);
	void AddEntity(IEntity* _pEntity);
	void RemoveEntity();

protected:

private:

	// Member Variables
public:

protected:
	std::vector<IEntity*> m_vecpEntities;
private:

};
#endif // __ENTITYMANAGER_H__



