#include <string.h>

// Local Includes
#include "../include/LogManager.h"
#include "../include/utils.h"

// This Includes
#include "../include/logd3dfb.h"

// Static Variables

// Static Function Prototypes

// Implementation
CLogD3DFB::CLogD3DFB()
	: m_pvecMessage(0)
	  , m_pFont(0)
	  , m_fScrollTimer(0.0f)
	  , m_fScrollRate(0.0f)
	  , m_pDevice(0)
{
}

CLogD3DFB::~CLogD3DFB()
{
	m_pFont->Release();
	m_pFont = 0;
}

/**
*
* This function Initialises the LogD3DFB.
*
* @param _pDevice is a pointer to an IDirect3DDevice9
* @return Returns true if successful.
*
*/
bool
CLogD3DFB::Initialise(LPDIRECT3DDEVICE9 _pDevice)
{
	HRESULT hr;

	m_pDevice = _pDevice;

	m_pFontRect.top = FONT_RECT_TOP;
	m_pFontRect.bottom = FONT_RECT_BOTTOM;
	m_pFontRect.left = FONT_RECT_LEFT;
	m_pFontRect.right = FONT_RECT_RIGHT;

	ZeroMemory(&m_pFontDesc, sizeof(m_pFontDesc));

	m_pFontDesc.Height = FONT_HEIGHT;
	m_pFontDesc.Width = FONT_WIDTH;
	m_pFontDesc.Weight = FONT_WEIGHT;
	m_pFontDesc.MipLevels = D3DX_DEFAULT;

	strcpy_s(m_pFontDesc.FaceName, 32, "Arial");

	hr = D3DXCreateFontIndirectA(m_pDevice, &m_pFontDesc, &m_pFont);

	return (true);
}

/**
*
* This function is resposible for doing any processing that is
* required with respect to messeges being logged to the frame buffer.
*
* @param _fDeltaTick is the amount of time elapsed between frames.
* @return Returns void.
*
*/
void
CLogD3DFB::Process(Float32 _fDeltaTick)
{
	m_fScrollTimer += _fDeltaTick;
	if (m_fScrollTimer >= MESSAGE_SPEED)
	{
		if (m_pvecMessage.size() > 0)
		{
			m_pvecMessage.erase(m_pvecMessage.begin());
		}
		m_fScrollTimer = 0;
	}
}

/**
*
* This function draws a message to the frame buffer.
*
* @return Returns void.
*
*/
void
CLogD3DFB::Draw()
{
	// Call DrawText() on the vector of messages.
	for (UInt32 i = 0; i < m_pvecMessage.size(); ++i)
	{
		if (m_pvecMessage[i])
		{
			m_pFontRect.top = FONT_HEIGHT * i;
			m_pFont->DrawTextA(0,
			                   m_pvecMessage[i],
			                   -1,
			                   &m_pFontRect,
			                   DT_LEFT | DT_TOP,
			                   D3DCOLOR_XRGB(255, 0, 0));
		}
	}
	m_pFontRect.top = 0;
}

/**
*
* This function is resposible for writing a message.
*
* @param _pcMessage is the message to be written.
* @return Returns void.
*
*/
void
CLogD3DFB::Write(const Int8 *_pcMessage)
{
	if (*_pcMessage != '\n')
	{
		m_pvecMessage.push_back(_pcMessage);
	}
}



