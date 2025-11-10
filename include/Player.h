#pragma once

#ifndef __PLAYER_H__
#define __PLAYER_H__

// Library Includes
#include <d3dx9.h>

// Local Includes
#include "defines.h"
#include "dynamicentity.h"
#include "messageprocessor.h"

// Types

// Constants
const EMessage kDEFAULT_MESSAGE_LEFT = MESSAGE_PRESSED_KEY_LEFT;
const EMessage kDEFAULT_MESSAGE_RIGHT = MESSAGE_PRESSED_KEY_RIGHT;
const EMessage kDEFAULT_MESSAGE_UP = MESSAGE_PRESSED_KEY_UP;
const EMessage kDEFAULT_MESSAGE_DOWN = MESSAGE_PRESSED_KEY_DOWN;

// Prototypes

class CPlayer : public CDynamicEntity
{
	// Member Functions
public:
	CPlayer();
	virtual ~CPlayer();

	bool Initialise(D3DXVECTOR3 _vec3Position, 
					UInt8 _cNumberOfLives, 
					const WChar16* _pcXFileName, 
					const WChar16* _pcTextureFileName,
					CD3DRenderer* _pRenderer,
					Float32 _fMaxSpeed,
					Float32 _fTurnSpeed,
					Float32 _fFriction,
					Int32 _iId,
					Float32 _fReloadTime,
					EMessage _messageLeft = MESSAGE_PRESSED_KEY_LEFT,
					EMessage _messageRight = MESSAGE_PRESSED_KEY_RIGHT,
					EMessage _messageUp = MESSAGE_PRESSED_KEY_UP,
					EMessage _messageDown = MESSAGE_PRESSED_KEY_DOWN);
	void Process(Float32 _fDeltatick);
	
	void Draw();

	void AddToScore(Int32 _uiScore);
	UInt32* GetScore();
	
	void SetLives(UInt8 _usLives);
	UInt8* GetLives();

	void SetBulletPowerUpTime(Float32 _fBulletPowerUpTime);

	bool IsFiring();
	
	D3DXVECTOR3 GetFiringDirection();
	
	void SetActive(bool _bIsActive);
	bool IsActive();

	void SetSpeedPowerUpTime(Float32 _fAmount);

protected:

private:

	// Member Variables
public:

protected:
	bool m_IsActive;

	UInt8 m_ucLives;
	UInt32 m_uiScore;
	UInt8 m_ucPlayerStrength;
	Float32 m_fBulletPowerUpTime;
	Float32 m_fSpeedPowerUpTime;

	CMessageProcessor* m_pMessageProcessor;
	
	Float32 m_fMaxSpeed;
	Float32 m_fMaxReloadTime;
	Float32 m_fCurrentReloadTime;

	Int32 m_iId;

	EMessage m_messageLeft;
	EMessage m_messageRight;
	EMessage m_messageUp;
	EMessage m_messageDown;

	D3DXVECTOR3 m_vec3FiringDirection;
	bool m_bIsFiring;

private:

};
#endif // __PLAYER_H__



