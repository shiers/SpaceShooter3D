#include "../include/D3DRenderer.h"

// This Includes
#include "../include/PointLight.h"

// Static Variables

// Static Function Prototypes

// Implementation
CPointLight::CPointLight()
	: m_pVec3Position(0)
	  , m_pRenderer(0)
	  , m_ucId(0)
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

CPointLight::~CPointLight()
{
}

/**
*
* This function initialises the point light.
*
* @param _pRenderer is a pointer to a CD3DRenderer.
* @param _pVec3Position is a pointer a player's position.
* @return Returns true if successful.
*
*/
bool
CPointLight::Initialise(CD3DRenderer *_pRenderer,
                        D3DXVECTOR3 *_pVec3Position,
                        Float32 _fSafeZoneRadius,
                        UInt8 _id)
{
	m_pRenderer = _pRenderer;
	m_pVec3Position = _pVec3Position;

	m_ucId = _id;

	m_Light.Type = D3DLIGHT_POINT;

	m_Light.Diffuse.r = 1.0f;
	m_Light.Diffuse.g = 0.0f;
	m_Light.Diffuse.b = 0.5f;
	m_Light.Diffuse.a = 1.0f;

	m_Light.Range = _fSafeZoneRadius / 2;

	m_Light.Position.x = m_pVec3Position->x;
	m_Light.Position.y = m_pVec3Position->y;
	m_Light.Position.z = m_pVec3Position->z;
	m_Light.Position.y += 10.0f;

	m_pRenderer->GetDevice()->SetLight(m_ucId, &m_Light);
	m_pRenderer->GetDevice()->LightEnable(m_ucId, TRUE);

	return (true);
}

void
CPointLight::Process(Float32 _fDeltaTick)
{
	m_Light.Position.x = m_pVec3Position->x;
	m_Light.Position.y = m_pVec3Position->y;
	m_Light.Position.z = m_pVec3Position->z;
	m_pRenderer->GetDevice()->SetLight(m_ucId, &m_Light);
	m_Light.Position.y += 10.0f;
}



