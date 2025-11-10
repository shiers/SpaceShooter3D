#include <d3dx9.h>

// Local Includes

// This Includes
#include "../include/SeekerEnemy.h"

// Static Variables
UInt16 CSeekerEnemy::m_usSeekerEnemyValue = 0;

// Static Function Prototypes

// Implementation
CSeekerEnemy::CSeekerEnemy()
	: m_ucSeekerEnemyLife(0)
{
}

CSeekerEnemy::~CSeekerEnemy()
{
}

/**
*
* This function is resposible for doing any processing that the Seeker
* Enemy requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CSeekerEnemy::Process(Float32 _fDeltaTick)
{
	D3DXVECTOR3 tempHeader;

	tempHeader.x = m_pmatPointToFollow->_41 - m_matWorld._41;
	tempHeader.y = m_pmatPointToFollow->_42 - m_matWorld._42;
	tempHeader.z = m_pmatPointToFollow->_43 - m_matWorld._43;
	D3DXVec3Normalize(&tempHeader, &tempHeader);

	m_vec3Heading = tempHeader;

	CEnemy::Process(_fDeltaTick);
}

bool
CSeekerEnemy::IsReadyToShoot()
{
	return (false);
}



