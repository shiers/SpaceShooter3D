#pragma once

#ifndef SPACESHOOTER3D_MESH_H
#define SPACESHOOTER3D_MESH_H

// Library Includes
#include <d3dx9.h>
#include <d3d9.h>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes

class CMesh
{
   //Member Functions
public:
	CMesh();
	~CMesh();

	bool Initialise(const WChar16* _pFilename, 
					const WChar16* _pTextureFileName, 
					IDirect3DDevice9* _pDevice);
	void Draw();	

	ID3DXMesh* GetMesh();
	IDirect3DTexture9* GetTexture();

	Float32 GetBoundingSphereRadius();
	static void ToggleBoundingVolumes(bool _bOnOrOff = !m_bBoundingSpheresOn);
	static void ToggleWireframe(bool _bOnOrOff = !m_bWireFrameOn);

	void DrawMeshSubset(UInt32 _i);

protected:
	void ComputeBoundingSphere();

private:

   //Member Variables
public:

protected:

private:
	static bool m_bBoundingSpheresOn;
	static bool m_bWireFrameOn;

	D3DXVECTOR3 m_vec3BoundingSphereCenter;
	Float32 m_fRadius;

	ID3DXMesh* m_pBoundingSphereMesh;
	ID3DXMesh* m_pMesh;
	IDirect3DDevice9* m_pDevice;
	ID3DXBuffer* m_pAdjacencyBuffer;
	D3DMATERIAL9* m_pMaterialBuffer;
	IDirect3DTexture9* m_pTexture;

	DWORD m_dNumberOfMaterials;
};

#endif // SPACESHOOTER3D_MESH_H



