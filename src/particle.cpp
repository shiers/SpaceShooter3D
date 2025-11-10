#include "../include/particle.h"

// Static Variables

// Static Function Prototypes

// Implementation
CParticle::CParticle()
	: m_bIsAlive(0)
	  , m_fFriction(0)
	  , m_fLifetime(0)
	  , m_fSpeed(0)
{
	ZeroMemory(&m_vec3Direction, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vec3Position, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vec3Acceleration, sizeof(D3DXVECTOR3));
}

CParticle::~CParticle()
{
}

bool
CParticle::Initialise(D3DXVECTOR3 _pos,
                      D3DXVECTOR3 &_velocity,
                      D3DXVECTOR3 &_acceleration,
                      float _fSpeed,
                      float _fLifeTime,
                      bool _bIsAlive,
                      float _fFriction)
{
	m_fSpeed = _fSpeed;
	m_vec3Position = _pos;
	m_vec3Direction = _velocity;
	m_bIsAlive = _bIsAlive;
	m_fFriction = _fFriction;
	m_vec3Acceleration = _acceleration;
	m_fLifetime = _fLifeTime;
	return (true);
}

void
CParticle::Process(float _fDeltaTick)
{
	if (m_bIsAlive)
	{
		m_vec3Direction.x += m_vec3Acceleration.x * _fDeltaTick;
		m_vec3Direction.y += m_vec3Acceleration.y * _fDeltaTick;
		m_vec3Direction.z += m_vec3Acceleration.z * _fDeltaTick;

		m_vec3Position.x += m_fSpeed * m_vec3Direction.x * _fDeltaTick;
		m_vec3Position.y += m_fSpeed * m_vec3Direction.y * _fDeltaTick;
		m_vec3Position.z += m_fSpeed * m_vec3Direction.z * _fDeltaTick;

		m_fSpeed *= 1 - m_fFriction;

		m_fLifetime -= _fDeltaTick;
		if (m_fLifetime < 0.0f)
		{
			m_bIsAlive = false;
		}
	}
}

D3DXVECTOR3 *
CParticle::GetPosition()
{
	return (&m_vec3Position);
}

bool
CParticle::IsAlive()
{
	return (m_bIsAlive);
}



