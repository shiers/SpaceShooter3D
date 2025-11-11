#include "../include/defines.h"

// This Includes
#include "../include/mesh.h"

// Static Variables
bool CMesh::m_bBoundingSpheresOn = false;
bool CMesh::m_bWireFrameOn = false;

// Static Function Prototypes

// Implementation

CMesh::CMesh()
	: m_pMesh(0)
	  , m_dNumberOfMaterials(0)
	  , m_pMaterialBuffer(0)
	  , m_pAdjacencyBuffer(0)
	  , m_pTexture(0)
	  , m_fRadius(0.0f)
	  , m_pBoundingSphereMesh(0)
{
}

CMesh::~CMesh()
{
	if (m_pMaterialBuffer)
	{
		delete[] m_pMaterialBuffer;
		m_pMaterialBuffer = 0;
	}
	if (m_pAdjacencyBuffer)
	{
		m_pAdjacencyBuffer->Release();
	}
	if (m_pMesh)
	{
		m_pMesh->Release();
	}
	if (m_pTexture)
	{
		m_pTexture->Release();
	}
	if (m_pBoundingSphereMesh)
	{
		m_pBoundingSphereMesh->Release();
	}
}

bool
CMesh::Initialise(const WChar16 *_pFilename, const WChar16 *_pTextureFileName, IDirect3DDevice9 *_pDevice)
{
	bool bFailure = false;

	m_pDevice = _pDevice;

	ID3DXBuffer *tempBuffer = 0;

	HRESULT hr = D3DXLoadMeshFromXW(_pFilename,
	                                D3DXMESH_MANAGED,
	                                m_pDevice,
	                                &m_pAdjacencyBuffer,
	                                &tempBuffer,
	                                NULL,
	                                &m_dNumberOfMaterials,
	                                &m_pMesh);

	if (FAILED(hr))
	{
		bFailure = true;
	} else
	{
		D3DXMATERIAL *tempMaterial = reinterpret_cast<D3DXMATERIAL *>(tempBuffer->GetBufferPointer());
		if (!m_pMaterialBuffer)
		{
			m_pMaterialBuffer = new D3DMATERIAL9[m_dNumberOfMaterials];
		}
		for (UInt32 i = 0; i < m_dNumberOfMaterials; ++i)
		{
			m_pMaterialBuffer[i] = tempMaterial[i].MatD3D;
		}
		tempBuffer->Release();

		hr = D3DXCreateTextureFromFileW(m_pDevice, _pTextureFileName, &m_pTexture);
		if (FAILED(hr))
		{
			bFailure = true;
		}
	}

	ComputeBoundingSphere();

	return (!bFailure);
}

void
CMesh::Draw()
{
	HRESULT hr;

	if (m_bWireFrameOn)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	} else
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	m_pDevice->SetTexture(0, m_pTexture);

	if (m_dNumberOfMaterials > 0)
	{
		for (UInt32 i = 0; i < m_dNumberOfMaterials; ++i)
		{
			m_pDevice->SetMaterial(&m_pMaterialBuffer[i]);
			hr = m_pMesh->DrawSubset(i);
		}
	}
	if (m_bBoundingSpheresOn)
	{
		if (m_pBoundingSphereMesh)
		{
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			m_pBoundingSphereMesh->DrawSubset(0);

			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		}
	}

	m_pDevice->SetTexture(0, 0);
}

ID3DXMesh *
CMesh::GetMesh()
{
	return (m_pMesh);
}

void
CMesh::ComputeBoundingSphere()
{
	if (m_pMesh)
	{
		IDirect3DVertexBuffer9 *tempVertexBuffer = 0;
		HRESULT hr;
		void *pBuffer = 0;
		D3DXVECTOR3 *vec3FirstPosition = 0;
		ZeroMemory(&m_vec3BoundingSphereCenter, sizeof(D3DXVECTOR3));

		hr = m_pMesh->GetVertexBuffer(&tempVertexBuffer);

		if (SUCCEEDED(hr))
		{
			hr = tempVertexBuffer->Lock(0, 0, &pBuffer, D3DLOCK_READONLY);

			if (SUCCEEDED(hr))
			{
				vec3FirstPosition = reinterpret_cast<D3DXVECTOR3 *>(pBuffer);

				UInt32 numVerts = m_pMesh->GetNumVertices();
				UInt32 sizeofVert = m_pMesh->GetNumBytesPerVertex();

				hr = D3DXComputeBoundingSphere(vec3FirstPosition,
				                               numVerts,
				                               sizeofVert,
				                               &m_vec3BoundingSphereCenter,
				                               &m_fRadius);
				tempVertexBuffer->Unlock();

				if (SUCCEEDED(hr))
				{
					hr = D3DXCreateSphere(m_pDevice,
					                      m_fRadius,
					                      16,
					                      16,
					                      &m_pBoundingSphereMesh,
					                      0);
					if (SUCCEEDED(hr))
					{
						int g = 0;
					} else
					{
						int g = 0;
					}
				}
			}

			tempVertexBuffer->Release();
		}
	}
}

void
CMesh::ToggleBoundingVolumes(bool _bOnOrOff)
{
	m_bBoundingSpheresOn = _bOnOrOff;
}

bool
CMesh::AreBoundingVolumesVisible()
{
	return (m_bBoundingSpheresOn);
}

Float32
CMesh::GetBoundingSphereRadius()
{
	return (m_fRadius);
}

IDirect3DTexture9 *
CMesh::GetTexture()
{
	return (m_pTexture);
}

void
CMesh::DrawMeshSubset(UInt32 _i)
{
	if (_i < m_dNumberOfMaterials)
	{
		m_pDevice->SetMaterial(&m_pMaterialBuffer[_i]);
		m_pMesh->DrawSubset(_i);
	}
}

void
CMesh::ToggleWireframe(bool _bOnOrOff)
{
	m_bWireFrameOn = _bOnOrOff;
}

bool
CMesh::IsWireframeEnabled()
{
	return (m_bWireFrameOn);
}



