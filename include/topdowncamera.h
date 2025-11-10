#pragma once

#ifndef SPACESHOOTER3D_TOPDOWNCAMERA_H
#define SPACESHOOTER3D_TOPDOWNCAMERA_H

// Library Includes

// Local Includes
#include "igcamera.h"

// Types

// Constants

// Prototypes


class CTopDownCamera : public CIGCamera
{
   //Member Functions
public:
	CTopDownCamera();
	virtual ~CTopDownCamera();

	//This function takes 3 coordinates for the position that the camera will follow, and three coordinates
	//that form a normalised vector to specify the direction of the object to be followed.
	void SetFollow(D3DXMATRIX* _pFollowObject);

	void SetDistance(float _fDistance);
	void SetHeight(float _fHeight);

	virtual void Process(float _fDeltaTick);
protected:

private:

   //Member Variables
public:

protected:

private:
	
	D3DXMATRIX* m_pPointToFollow;
	D3DXMATRIX m_matWorld;

	//The distance to the object being followed.
	float m_fDistance;

	float m_fHeight;

};

#endif // SPACESHOOTER3D_TOPDOWNCAMERA_H



