#pragma once

#ifndef SPACESHOOTER3D_DYNAMICENTITY_H
#define SPACESHOOTER3D_DYNAMICENTITY_H

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "defines.h"
#include "3dobject.h"

// Types

// Constants

// Prototypes


class CDynamicEntity : public C3DObject
{
   //Member Functions
public:
	CDynamicEntity();
	virtual ~CDynamicEntity();

	virtual void Process(Float32 _fDeltaTick);

	void SetMoveFriction(Float32 _fAmount);
	void SetTurnFriction(Float32 _fAmount);
	void SetSpeed(Float32 _fAmount);
	void SetXYZDirection(D3DXVECTOR3* _vec3Position, D3DXVECTOR3 _vec3Direction);
	void SetBoundary(RECT _rect);
	
	D3DXVECTOR3 GetHeadingVector() const;
	D3DXVECTOR3 GetDirectionVector() const;

	D3DXVECTOR3* GetPosition();

	static bool IsCollision(CDynamicEntity* _pEntity1, CDynamicEntity* _pEntity2);

	Float32 GetRadius();

protected:

private:

   //Member Variables
public:

protected:
	bool m_bHitBoundaries;
	bool m_bHitRightBoundary;
	bool m_bHitLeftBoundary;
	bool m_bHitTopBoundary;
	bool m_bHitBottomBoundary;
	RECT m_boundaryRect;
	Float32 m_fMoveFriction;
	Float32 m_fTurnFriction;
	Float32 m_fSpeed;
	Float32 m_fTurnSpeed;

	D3DXVECTOR3 m_vec3Heading;
	D3DXVECTOR3 m_vec3Direction;
	D3DXVECTOR3 m_vec3Position;

private:
};

#endif // SPACESHOOTER3D_DYNAMICENTITY_H



