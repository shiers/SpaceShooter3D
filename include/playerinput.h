#pragma once

#ifndef SPACESHOOTER3D_PLAYERINPUT_H
#define SPACESHOOTER3D_PLAYERINPUT_H

// Library Includes
#include <vector>

// Local Includes

// Types

// Constants
const int ki_MAX_NUM_CONTROLLERS = 4;

// Prototypes
class CXInputController;
class CDirectInputController;

class CPlayerInput
{
   //Member Functions
public:

	~CPlayerInput();

	bool Initialise();
	void Process(float _fDeltaTick);

	static CPlayerInput& GetInstance();
	static void DestroyInstance();

	void EnumerateControllers();

protected:

private:
	CPlayerInput();

   //Member Variables
public:

protected:

private:
	static CPlayerInput* s_pPlayerInput;

	CXInputController* m_pXInputController[ki_MAX_NUM_CONTROLLERS];
	CDirectInputController* m_pvecDInputControllers[ki_MAX_NUM_CONTROLLERS];

	int m_iNumDIControllers;
};

#endif // SPACESHOOTER3D_PLAYERINPUT_H



