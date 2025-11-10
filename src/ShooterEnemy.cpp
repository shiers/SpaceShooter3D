#include "../include/ShooterEnemy.h"

// Static Variables
UInt16 CShooterEnemy::m_usShooterEnemyValue = 0;

// Static Function Prototypes

// Implementation
CShooterEnemy::CShooterEnemy()
	: m_ucShooterEnemyLife(0)
{
}

CShooterEnemy::~CShooterEnemy()
{
}

/**
*
* This function is resposible for doing any processing that the Shooter
* Enemy requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CShooterEnemy::Process(Float32 _fDeltaTick)
{
	D3DXVECTOR3 tempHeader;

	tempHeader.x = m_pmatPointToFollow->_41 - m_matWorld._41;
	tempHeader.y = m_pmatPointToFollow->_42 - m_matWorld._42;
	tempHeader.z = m_pmatPointToFollow->_43 - m_matWorld._43;

	m_vec3Heading = tempHeader;

	if (CEnemy::IsReadyToShoot() == true)
	{
		m_fCurrentRefireRate = m_fMaxRefireRate;
	}

	CEnemy::Process(_fDeltaTick);
}

/**
*
* This function informs the calling function if a shooter enemy is able to shoot.
* @param
* @return Returns a boolean that determines if an shooter enemy can shoot or not.
*
*/
bool
CShooterEnemy::IsReadyToShoot()
{
	return (CEnemy::IsReadyToShoot());
}



