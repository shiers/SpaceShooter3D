#include "../include/D3DRenderer.h"

// This Includes
#include "../include/DirectionalLight.h"

// Static Variables

// Static Function Prototypes

// Implementation
CDirectionalLight::CDirectionalLight()
	: m_pRenderer(0)
{
	ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

CDirectionalLight::~CDirectionalLight()
{
}


/**
*
* This function initialises the directional light.
*
* @param _pRenderer is a pointer to a CD3DRenderer.
* @param .
* @return Returns true if successful.
*
*/
bool
CDirectionalLight::Initialise(CD3DRenderer *_pRenderer)
{
	m_pRenderer = _pRenderer;

	// Making a directional light.
	m_Light.Type = D3DLIGHT_DIRECTIONAL;

	m_Light.Diffuse.r = 0.5f;
	m_Light.Diffuse.g = 0.5f;
	m_Light.Diffuse.b = 0.5f;
	m_Light.Diffuse.a = 1.0f;

	m_Light.Specular.r = 0.2f;
	m_Light.Specular.g = 0.2f;
	m_Light.Specular.b = 0.2f;
	m_Light.Specular.a = 1.0f;

	// The direction of the light.
	D3DVECTOR vecDirection = {0.0f, -100.0f, 0.0f};

	// Sets the direction.
	m_Light.Direction = vecDirection;

	m_pRenderer->GetDevice()->SetLight(4, &m_Light);
	m_pRenderer->GetDevice()->LightEnable(4, TRUE);

	return (true);
}

void
CDirectionalLight::Process(Float32 _fDeltaTick)
{
}



