#pragma once

#ifndef __BULLET_H__
#define __BULLET_H__

// Library Includes

// Local Includes
#include "defines.h"
#include "dynamicentity.h"

// Types
typedef enum
{
	INVALID_BULLET_TYPE,
	BULLET_STANDARD_TYPE,
	BULLET_HIPOWER_TYPE,
	MAX_BULLET_TYPE,
}EBulletType;

// Constants
const Float32 kfDEFAULT_LIFE_TIME = 5.0f;

// Prototypes

class CBullet : public CDynamicEntity
{
	// Member Functions
public:
	CBullet();
	~CBullet();

	bool Initialise(const WChar16* _pFileName,
					const WChar16* _pTextureFileName,
					CD3DRenderer* _pRenderer,
					UInt8 _ucBulletDamage,
					Float32 _fBulletSpeed,
					bool _bIsActive,
					Float32 _fLifeTime = kfDEFAULT_LIFE_TIME);

	void Process(Float32 _fDeltaTick);

	EBulletType GetBulletType() const;
	void SetBulletType(EBulletType _eBulletType);

	UInt8 GetBulletDamage();

	bool IsBulletActive() const;
	void SetBulletActivity(bool _bIsActive, Float32 _fLifeTime = kfDEFAULT_LIFE_TIME);


protected:

private:

	// Member Variables
public:

protected:

	EBulletType m_eBulletType;
	UInt8 m_ucBulletDamage;
	bool m_bIsActive;
	Float32 m_fLifeTime;

private:

};
#endif // __BULLET_H__



