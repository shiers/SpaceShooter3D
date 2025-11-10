#include <cassert>
#include <cmath>

// Local Includes
#include "../include/mesh.h"
#include "../include/messageprocessor.h"

// This Includes
#include "../include/titlescreen.h"

// Static Variables

// Static Function Prototypes

// Implementation
CTitleScreen::CTitleScreen()
	: m_pMesh(0)
	  , m_pTargetMesh(0)
	  , m_pMessageProcessor(0)
	  , m_ucMenuSelection(0)
	  , m_ucMaxSelection(0)
	  , m_fMenuSelectionGap(0)
	  , m_fLooper(0)
	  , m_bKeyPressed(false)
	  , m_fPulsateAmount(0)
{
	ZeroMemory(&m_vec3TargetStartPosition, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vec3TitleScreenStartPosition, sizeof(D3DXVECTOR3));
	D3DXMatrixIdentity(&m_rotationMatrix);
}

CTitleScreen::~CTitleScreen()
{
	delete m_pMesh;
	m_pMesh = 0;
	delete m_pTargetMesh;
	m_pTargetMesh = 0;
}

bool
CTitleScreen::Initialise(const wchar_t *_MeshFileName,
                         const wchar_t *_MeshTextureFileName,
                         const wchar_t *_TargetFileName,
                         const wchar_t *_TargetTextureFileName,
                         D3DXVECTOR3 _targetStartPos,
                         D3DXVECTOR3 _titleScreenStartPosition,
                         Float32 _menuSelectionGap,
                         IDirect3DDevice9 *_pDevice,
                         UInt8 _maxMenuSelection)
{
	bool bFailure = false;

	m_pDevice = _pDevice;
	assert(m_pDevice);
	m_pMessageProcessor = CMessageProcessor::GetInstance();
	assert(m_pMessageProcessor);

	m_fMenuSelectionGap = _menuSelectionGap;
	m_ucMaxSelection = _maxMenuSelection;

	//Initialise the first mesh.
	if (!m_pMesh)
	{
		m_pMesh = new CMesh;
	}
	bFailure = !m_pMesh->Initialise(_MeshFileName, _MeshTextureFileName, m_pDevice);
	if (!bFailure)
	{
		m_vec3TitleScreenStartPosition = _titleScreenStartPosition;
		m_pMessageProcessor->SendMessage("GAME:     TITLESCREEN: Title screen mesh initialised successfully.");
		if (!m_pTargetMesh)
		{
			m_pTargetMesh = new CMesh;
		}
		bFailure = !m_pTargetMesh->Initialise(_TargetFileName, _TargetTextureFileName, m_pDevice);
		if (!bFailure)
		{
			m_vec3TargetStartPosition = _targetStartPos;
			m_pMessageProcessor->SendMessage("GAME:     TITLESCREEN: Target mesh initialised successfully.");
		} else
		{
			m_pMessageProcessor->SendMessage("ERROR:     GAME:     TITLESCREEN: Target mesh initialisation failure.");
		}
	} else
	{
		m_pMessageProcessor->SendMessage("ERROR:    GAME:     TITLESCREEN: Title screen mesh initialisation FAILURE.");
	}

	return (!bFailure);
}

void
CTitleScreen::Process(Float32 _fDeltaTick)
{
	m_fLooper += _fDeltaTick;
	if (m_fLooper > 2 * PI)
	{
		m_fLooper -= 2 * PI;
	}
	m_fPulsateAmount = 0.02f * sinf(m_fLooper);

	D3DXMatrixRotationY(&m_rotationMatrix, m_fLooper);

	XInputInformation *tempInformation = m_pMessageProcessor->GetXInputControllerState(0);

	bool bDownKeyPressed = m_pMessageProcessor->CheckForKeyMessage(MESSAGE_PRESSED_KEY_DOWN);
	bool bUpKeyPressed = m_pMessageProcessor->CheckForKeyMessage(MESSAGE_PRESSED_KEY_UP);
	float controllerY = tempInformation->fLeftControllerY;

	if (!m_bKeyPressed)
	{
		if (bDownKeyPressed || controllerY <= -0.9f)
		{
			if (m_ucMenuSelection < m_ucMaxSelection)
			{
				m_pMessageProcessor->SendMessage("GAME:     TITLESCREEN: Menu Selection Incremented.");
				++m_ucMenuSelection;
				m_bKeyPressed = true;
			}
		} else if (bUpKeyPressed || controllerY >= 0.9f)
		{
			if (m_ucMenuSelection > 0)
			{
				m_pMessageProcessor->SendMessage("GAME:     TITLESCREEN: Menu Selection Decremented.");
				--m_ucMenuSelection;
				m_bKeyPressed = true;
			}
		}
	}

	if (bDownKeyPressed == false && bUpKeyPressed == false && tempInformation->fLeftControllerY == 0)
	{
		m_bKeyPressed = false;
	}
}

void
CTitleScreen::Draw()
{
	D3DXMATRIX tempworldMatrix;
	D3DXMatrixIdentity(&tempworldMatrix);

	tempworldMatrix._11 += m_fPulsateAmount;
	tempworldMatrix._22 += m_fPulsateAmount;
	tempworldMatrix._33 += m_fPulsateAmount;
	tempworldMatrix._41 = m_vec3TitleScreenStartPosition.x;
	tempworldMatrix._42 = m_vec3TitleScreenStartPosition.y;
	tempworldMatrix._43 = m_vec3TitleScreenStartPosition.z;
	m_pDevice->SetTransform(D3DTS_WORLD, &tempworldMatrix);

	m_pMesh->Draw();

	D3DXMatrixIdentity(&tempworldMatrix);
	D3DXMatrixMultiply(&tempworldMatrix, &m_rotationMatrix, &tempworldMatrix);
	tempworldMatrix._41 = m_vec3TargetStartPosition.x;
	tempworldMatrix._42 = m_vec3TargetStartPosition.y - static_cast<float>(m_ucMenuSelection) * m_fMenuSelectionGap;
	tempworldMatrix._43 = m_vec3TargetStartPosition.z;
	m_pDevice->SetTransform(D3DTS_WORLD, &tempworldMatrix);

	m_pTargetMesh->Draw();
}

ETitleScreenSelection
CTitleScreen::GetSelection()
{
	return (static_cast<ETitleScreenSelection>(m_ucMenuSelection));
}



