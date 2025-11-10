#pragma once

#ifndef __LOGD3DFB_H__
#define __LOGD3DFB_H__

// Library Includes
#include <d3dx9.h>
#include <vector>

// Local Includes
#include "defines.h"
#include "LogTarget.h"

// Types
typedef enum
{
	INVALID_MSG_ATTRIBUTE,
	MESSAGE_SPEED = 2,
	FONT_HEIGHT = 16,
	FONT_WIDTH = 6,
	FONT_WEIGHT = 300,
	FONT_RECT_TOP = 10,
	FONT_RECT_LEFT = 10,
	FONT_RECT_BOTTOM = 300,
	FONT_RECT_RIGHT = 600,
	MAX_MESSAGE_ATTRIBUTE,
}EMsgAttribute;

// Constants

// Prototypes
class CLogManager;

class CLogD3DFB : public ILogTarget
{
	// Member Functions
public:
	CLogD3DFB();
	virtual ~CLogD3DFB();

	bool Initialise(LPDIRECT3DDEVICE9 _pDevice);

	void Process(Float32 _fDeltaTick);

	void Draw();

	void Write(const Int8* _pcMessage);

protected:

private:

	// Member Variables
public:

protected:
	std::vector<const Int8*> m_pvecMessage;
	std::vector<Int32> m_iTimeInQueue;

	LPD3DXFONT m_pFont;
	D3DXFONT_DESCA m_pFontDesc;

	RECT m_pFontRect;

	Float32 m_fScrollTimer;
	Float32 m_fScrollRate;

	LPDIRECT3DDEVICE9 m_pDevice;

	

private:

};
#endif // __LOGD3DFB_H__



