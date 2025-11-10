#include "../include/entity.h"

// This Includes
#include "../include/EntityManager.h"

// Static Variables

// Static Function Prototypes

// Implementation
CEntityManager::CEntityManager()
	: m_vecpEntities(0)
{
}

CEntityManager::~CEntityManager()
{
	for (UInt32 i = 0; i < m_vecpEntities.size(); ++i)
	{
		delete m_vecpEntities[i];
		m_vecpEntities[i] = 0;
	}
}

/**
*
* This function initialises the Entity manager.
*
* @param 
* @return Returns true if successful.
*
*/
bool
CEntityManager::Initialise()
{
	return (true);
}

/**
*
* This function is resposible for doing any processing that the Entity
* Manager requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CEntityManager::Process(Float32 _fDeltatick)
{
}

/**
*
* This function adds an entity to the manager.
*
* @param _pEntity is a pointer to an entity.
* @return Returns void.
*
*/
void
CEntityManager::AddEntity(IEntity *_pEntity)
{
	m_vecpEntities.push_back(_pEntity);
}

/**
*
* This function removes an entity from the manager.
*
* @param 
* @return Returns void.
*
*/
void
CEntityManager::RemoveEntity()
{
}



