#pragma once

#ifndef SPACESHOOTER3D_PARTICLE_H
#define SPACESHOOTER3D_PARTICLE_H

// Library Includes
#include <d3dx9.h>

// Local Includes

// Types

// Constants
const float kfDEFAULT_PARTICLE_LIFETIME = 10.0f;
const float kfDEFAULT_FRICTION = 0.00f;

// Prototypes
struct ParticleVertex
{
	D3DXVECTOR3 position;
	D3DCOLOR colour;
};

class CParticle
{
   //Member Functions
public:
	CParticle();
	~CParticle();

	bool Initialise(D3DXVECTOR3 _pos, 
		D3DXVECTOR3& _velocity, 
		D3DXVECTOR3& _acceleration,
		float _fSpeed,
		float _fLifeTime = kfDEFAULT_PARTICLE_LIFETIME,
		bool _bIsAlive = true,
		float _fFriction = kfDEFAULT_FRICTION);
	void Process(float _fDeltaTick);

	D3DXVECTOR3* GetPosition();

	bool IsAlive();

protected:

private:

   //Member Variables
public:

protected:

private:

	D3DXVECTOR3 m_vec3Position;
	D3DXVECTOR3 m_vec3Direction;
	D3DXVECTOR3 m_vec3Acceleration;
	float m_fSpeed;
	float m_fLifetime;
	bool m_bIsAlive;
	float m_fFriction;

};

#endif // SPACESHOOTER3D_PARTICLE_H



