#include "../include/particleemitter.h"

// Static Variables

// Static Function Prototypes

// Implementation

CParticleEmitter::CParticleEmitter()
	: m_arraypParticles(0)
	  , m_bIsAlive(false)
	  , m_iNumParticles(0)
	  , m_pVertexBuffer(0)
	  , m_fLifeTime(0)
	  , m_colour(0)
	  , m_pDevice(0)
	  , m_fSpeed(0)
	  , m_position(0)
{
	//Constructor.
}

CParticleEmitter::~CParticleEmitter()
{
	//Deconstructor.
	delete[] m_arraypParticles;
	m_arraypParticles = 0;

	m_pVertexBuffer->Release();
}

bool
CParticleEmitter::Initialise(D3DXVECTOR3 *_position,
                             D3DCOLOR _colour,
                             int _iNumberOfParticles,
                             IDirect3DDevice9 *_pDevice,
                             float _fLifeTime,
                             float _fSpeed)
{
	bool bFailure = false;

	m_fSpeed = _fSpeed;
	m_pDevice = _pDevice;

	m_iNumParticles = _iNumberOfParticles;
	m_bIsAlive = true;
	m_fLifeTime = _fLifeTime;
	m_colour = _colour;

	m_position = _position;

	HRESULT hr;
	if (!m_pVertexBuffer)
	{
		hr = m_pDevice->CreateVertexBuffer(sizeof(ParticleVertex) * m_iNumParticles,
		                                   D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS,
		                                   D3DFVF_XYZ | D3DFVF_DIFFUSE,
		                                   D3DPOOL_DEFAULT,
		                                   &m_pVertexBuffer,
		                                   0);
	}

	return (!bFailure);
}

void
CParticleEmitter::Process(float _fDeltaTick)
{
	if (m_bIsAlive)
	{
		for (int i = 0; i < m_iNumParticles; ++i)
		{
			//Update particle position.
			m_arraypParticles[i].Process(_fDeltaTick);
		}
		m_fLifeTime -= _fDeltaTick;
		if (m_fLifeTime < 0.0f)
		{
			m_fLifeTime = 5.0f;
			m_bIsAlive = false;
		}
	}
}

void
CParticleEmitter::Draw()
{
	m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
	if (m_bIsAlive)
	{
		D3DXMATRIX tempMatrix;
		D3DXMatrixIdentity(&tempMatrix);

		tempMatrix._41 = m_position->x;
		tempMatrix._42 = m_position->y;
		tempMatrix._43 = m_position->z;

		m_pDevice->SetTransform(D3DTS_WORLD, &tempMatrix);

		ParticleVertex *m_pData;
		m_pVertexBuffer->Lock(0, 0, reinterpret_cast<void **>(&m_pData), D3DLOCK_DISCARD);

		for (int i = 0; i < m_iNumParticles; ++i)
		{
			ZeroMemory(m_pData, sizeof(ParticleVertex));
			if (m_arraypParticles[i].IsAlive())
			{
				m_pData->position = *m_arraypParticles[i].GetPosition();
				m_pData->colour = m_colour;
			}
			++m_pData;
		}

		m_pVertexBuffer->Unlock();

		m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(ParticleVertex));
		m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_iNumParticles);
	}
	m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

bool
CParticleEmitter::IsAlive()
{
	return (m_bIsAlive);
}

void
CParticleEmitter::SetAlive(bool _bIsAlive)
{
	m_bIsAlive = _bIsAlive;
}

void
CParticleEmitter::SetPosition(D3DXVECTOR3 *_pPosition)
{
	m_position = _pPosition;
}



