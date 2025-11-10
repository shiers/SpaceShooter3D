#include <d3dx9.h>

// Local Includes

// This Includes
#include "../include/topdowncamera.h"

// Static Variables

// Static Function Prototypes

// Implementation

CTopDownCamera::CTopDownCamera()
	: m_fDistance(0)
	  , m_fHeight(0)
{
	//Constructor.
}

CTopDownCamera::~CTopDownCamera()
{
	//Deconstructor.
}

/**
*
* This function sets an object for the camera to follow.
*
* @return Returns void.
*
*/
void
CTopDownCamera::SetFollow(D3DXMATRIX *_pFollowObject)
{
	m_pPointToFollow = _pFollowObject;
}

/**
*
* This function is resposible for doing any processing that the
* top down camera requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CTopDownCamera::Process(float _fDeltaTick)
{
	CIGCamera::Process(_fDeltaTick);

	D3DXMatrixIdentity(&m_matWorld);
	m_matWorld._41 = m_pPointToFollow->_41;
	m_matWorld._42 = m_pPointToFollow->_42;
	m_matWorld._43 = m_pPointToFollow->_43;
	m_matWorld._42 = m_fHeight;

	D3DXMATRIX tempMatrix;
	D3DXMatrixIdentity(&tempMatrix);

	float fAngle = -1 * atan(m_fHeight / m_fDistance);
	if (m_fHeight)
	{
		fAngle = D3DXToRadian(85.0f);
	}
	//else
	//{
	//	fAngle = D3DXToRadian(0.0f);
	//}

	D3DXMatrixRotationX(&tempMatrix, fAngle);

	D3DXMatrixMultiply(&m_matWorld, &tempMatrix, &m_matWorld);

	float fDeterminant = D3DXMatrixDeterminant(&m_matWorld);
	D3DXMatrixInverse(&m_matView, &fDeterminant, &m_matWorld);

	UpdateRenderer();
}

/**
*
* This function sets the horizontal distance between the camera and the
* avatar.
*
* @param _fDistance is the horizontal distance between the camera and the
*		 avatar.
* @return Returns void.
*
*/
void
CTopDownCamera::SetDistance(float _fDistance)
{
	m_fDistance = _fDistance;
}

/**
*
* This function sets the vertical distance between the camera and the
* avatar.
*
* @param _fHeight is the vertical distance between the camera and the
*		 avatar.
* @return Returns void.
*
*/
void
CTopDownCamera::SetHeight(float _fHeight)
{
	m_fHeight = _fHeight;
}



