#include <cstdlib>

// Local Includes
#include "../include/dynamicentity.h"

// This Includes
#include "../include/RandomEnemy.h"

// Static Variables
UInt16 CRandomEnemy::m_usRandomEnemyValue = 0;

// Static Function Prototypes

// Implementation
CRandomEnemy::CRandomEnemy()
	: m_ucRandomEnemyLife(0)
{
}

CRandomEnemy::~CRandomEnemy()
{
}

/**
*
* This function is resposible for doing any processing that the Random
* Enemy requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CRandomEnemy::Process(Float32 _fDeltaTick)
{
	if (m_fCurrentRefireRate <= 0.0f)
	{
		Float32 cRandomXPos = (static_cast<float>(rand() % 100) / 100.0f) - 0.5f;
		Float32 cRandomZPos = (static_cast<float>(rand() % 100) / 100.0f) - 0.5f;

		m_vec3Heading.x = cRandomXPos;
		m_vec3Heading.z = cRandomZPos;

		D3DXVec3Normalize(&m_vec3Heading, &m_vec3Heading);
		m_fCurrentRefireRate = m_fMaxRefireRate;
	}
	if (m_bHitLeftBoundary)
	{
		if (m_vec3Heading.x < 0.0f)
		{
			m_vec3Heading.x *= -1;
		}
	}
	if (m_bHitRightBoundary)
	{
		if (m_vec3Heading.x > 0.0f)
		{
			m_vec3Heading.x *= -1;
		}
	}
	if (m_bHitBottomBoundary)
	{
		if (m_vec3Heading.z < 0.0f)
		{
			m_vec3Heading.z *= -1;
		}
	}
	if (m_bHitTopBoundary)
	{
		if (m_vec3Heading.z > 0.0f)
		{
			m_vec3Heading.z *= -1;
		}
	}

	CEnemy::Process(_fDeltaTick);
}

/**
*
* This function informs the calling function if a random enemy is able to shoot.
* @param
* @return Returns a boolean that determines if an random enemy can shoot or not.
*
*/
bool
CRandomEnemy::IsReadyToShoot()
{
	return (false);
}



