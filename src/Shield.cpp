#include "../include/mesh.h"
#include "../include/D3DRenderer.h"

// This Includes
#include "../include/Shield.h"

// Static Variables

// Static Function Prototypes

// Implementation
CShield::CShield()
 : m_bIsAlive(false)
   , m_fLifeTime(0.0f)
   , m_fLooper(0.0f)
   , m_pD3DMesh(0)
   , m_pTexture(0)
   , m_pTexture2(0)
{
 D3DXMatrixIdentity(&m_matTransformation);
}

CShield::~CShield()
{
 if (m_pTexture2)
 {
  m_pTexture2->Release();
  m_pTexture2 = 0;
 }
}

/**
*
* This function initialises the shield.
*
* @param _bIsAlive is a boolen which specifies whether or not a shield is active.
* @param _fLifeTime specifies the life of a shield.
* @return Returns true if successful.
*
*/
bool
CShield::Initialise(CD3DRenderer *_pRenderer,
                    D3DXVECTOR3 *_pPointToFollow,
                    bool _bIsAlive,
                    Float32 _fLifeTime)
{
 m_bIsAlive = _bIsAlive;
 m_fLifeTime = _fLifeTime;

 m_pPointToFollow = _pPointToFollow;

 CDynamicEntity::Initialise(L"media\\shield.x", L"media\\shield.png", _pRenderer);

 m_pD3DMesh = m_pMesh->GetMesh();
 m_pTexture = m_pMesh->GetTexture();

 m_pDevice = _pRenderer->GetDevice();

 D3DXCreateTextureFromFile(m_pDevice, "media\\shield2.png", &m_pTexture2);

 m_pDevice->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);

 return (true);
}

/**
*
* This function will return if a shield is active or not.
*
* @param
* @return Returns true if successful.
*
*/
bool
CShield::IsAlive()
{
 return (m_bIsAlive);
}

/**
*
* This function sets the amount of time that a shield is active for.
*
* @param _fLifeTime specifies the life of a shield.
* @return Returns void.
*
*/
void
CShield::SetLifeTime(float _fLifeTime)
{
 m_fLifeTime = _fLifeTime;
}

void
CShield::Process(float _fDeltaTick)
{
 if (m_bIsAlive)
 {
  m_vec3Position = *m_pPointToFollow;

  m_fLooper += _fDeltaTick;

  if (m_fLooper > kf_DEFAULT_MAX_TICKER_VALUE)
  {
   m_fLooper = 0.0f;
  }

  m_matTransformation._31 = 2 * m_fLooper;
  m_matTransformation._32 = m_fLooper;

  m_fLifeTime -= _fDeltaTick;
  if (m_fLifeTime <= 0.0f)
  {
   m_bIsAlive = false;
  }
 } else
 {
  ZeroMemory(&m_vec3Position, sizeof(D3DXVECTOR3));
 }
 if (m_fLifeTime > 0.0f)
 {
  m_bIsAlive = true;
 }
}

void
CShield::Draw()
{
 if (m_bIsAlive)
 {
  D3DXMatrixIdentity(&m_matWorld);

  m_matWorld._41 = m_pPointToFollow->x;
  m_matWorld._42 = m_pPointToFollow->y;
  m_matWorld._43 = m_pPointToFollow->z;
  m_pRenderer->GetDevice()->SetTransform(D3DTS_WORLD, &m_matWorld);

  m_pDevice->SetTexture(1, m_pTexture);
  m_pDevice->SetTexture(0, m_pTexture2);

  m_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  m_pDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
  m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);

  m_pDevice->SetTransform(D3DTS_TEXTURE0, &m_matTransformation);

  m_pMesh->DrawMeshSubset(0);

  m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

  m_pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  m_pDevice->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

  m_pDevice->SetTexture(0, 0);
  m_pDevice->SetTexture(1, 0);
 }
}
