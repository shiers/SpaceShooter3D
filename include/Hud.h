#pragma once

#ifndef __HUD_H__
#define __HUD_H__

// Library Includes
#include <d3d9.h>
#include <d3dx9.h>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
class CBillboard;
class CHighScore;

class CHud
{
	// Member Functions
public:
	CHud();
	~CHud();

	bool Initialise(UInt32 _fScreenWidth, 
	                UInt32 _fScreenHeight, 
	                UInt8 _ucNumPlayers, 
	                UInt32** _pArrayOfScores, 
	                UInt8** _pArrayOfLives,
	                IDirect3DDevice9* _pDevice);


	void SetHighScore(UInt32 _uiHighScore);

	void Draw();

protected:

private:

	// Member Variables
public:

protected:
	Int32 m_uiHighScore;
	UInt32** m_puiScore;
	UInt8** m_pusLives;

	UInt32 m_fScreenWidth;
	UInt32 m_fScreenHeight;

	UInt8 m_ucNumberOfPlayers;

	RECT* m_arrayRects;

	IDirect3DDevice9* m_pDevice;
	D3DXFONT_DESC m_FontDescription;
	ID3DXFont* m_pFont;

	CHighScore* m_pHighScoreParser;

private:

};
#endif // __HUD_H__



