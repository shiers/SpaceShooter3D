#pragma once

#ifndef SPACESHOOTER3D_TITLESCREEN_H
#define SPACESHOOTER3D_TITLESCREEN_H

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "defines.h"

// Types

// Constants
const UInt8 kucDEFAULT_MAX_MENU_SELECTION = 2;

enum ETitleScreenSelection
{
	TITLE_START_1_PLAYER,
	TITLE_START_2_PLAYER,
	TITLE_EXIT,
};

// Prototypes
class CMesh;
class CMessageProcessor;

class CTitleScreen
{
   //Member Functions
public:
	CTitleScreen();
	~CTitleScreen();

	bool Initialise(const wchar_t* _MeshFileName,
					const wchar_t* _MeshTextureFileName,
					const wchar_t* _TargetFileName,
					const wchar_t* _TargetTextureFileName,
					D3DXVECTOR3 _targetStartPos,
					D3DXVECTOR3 _titleScreenStartPosition,
					Float32 _menuSelectionGap,
					IDirect3DDevice9* _pDevice,
					UInt8 _maxMenuSelection = kucDEFAULT_MAX_MENU_SELECTION);
	void Process(Float32 _fDeltaTick);
	void Draw();

	ETitleScreenSelection GetSelection();	

protected:

private:

   //Member Variables
public:

protected:

private:
	CMesh* m_pMesh;
	CMesh* m_pTargetMesh;
	CMessageProcessor* m_pMessageProcessor;
	UInt8 m_ucMenuSelection;
	UInt8 m_ucMaxSelection;

	Float32 m_fLooper;
	Float32 m_fPulsateAmount;

	D3DXVECTOR3 m_vec3TargetStartPosition;
	D3DXVECTOR3 m_vec3TitleScreenStartPosition;
	D3DXMATRIX m_rotationMatrix;
	Float32 m_fMenuSelectionGap;
	IDirect3DDevice9* m_pDevice;

	bool m_bKeyPressed;

};

#endif // SPACESHOOTER3D_TITLESCREEN_H



