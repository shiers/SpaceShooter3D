#include "../include/D3DRenderer.h"
#include "../include/mesh.h"

// This Includes
#include "../include/billboard.h"

// Static Variables

// Static Function Prototypes

// Implementation

CBillboard::CBillboard()
{
	//Constructor;
}

CBillboard::~CBillboard()
{
	//Deconstructor.
	delete m_pMesh;
	m_pMesh = 0;
}

/**
*
* This function Initialises the billboard.
*
* @param _pMesh is a pointer to a mesh.
* @param _pRenderer is a pointer to a renderer.
* @return Returns true if successful.
*
*/
bool
CBillboard::Initialise(CMesh *_pMesh, CD3DRenderer *_pRenderer)
{
	bool bFailure = false;

	bFailure = C3DObject::Initialise(_pMesh, _pRenderer);

	return (!bFailure);
}

/**
*
* This function Initialises the billboard.
*
* @param _pRenderer is a pointer to a renderer.
* @param _fWidth is the billboards width.
* @param _fHeight is the billboards height.
* @param _fDepth is the billboards.
* @param _ptexFilename is the filename of the texture that is attached to the
*		 billboard.
* @return Returns true if successful.
*
*/
bool
CBillboard::Initialise(CD3DRenderer *_pRenderer, float _fWidth, float _fHeight, float _fDepth,
                       const wchar_t *_ptexFilename)
{
	bool bFailure = false;

	m_pRenderer = _pRenderer;

	m_pMesh = new CMesh;

	return (!bFailure);
}

/**
*
* This function is resposible for doing any processing that the
* billboard requires.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CBillboard::Process(float _fDeltaTick)
{
	D3DXMATRIX matView = m_pRenderer->GetViewMatrix();
	float fDeterminant = D3DXMatrixDeterminant(&matView);
	D3DXMatrixInverse(&matView, &fDeterminant, &matView);

	matView._41 = m_matWorld._41;
	matView._42 = m_matWorld._42;
	matView._43 = m_matWorld._43;

	m_matWorld = matView;
}



