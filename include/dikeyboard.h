#pragma once

#ifndef SPACESHOOTER3D_DIKEYBOARD_H
#define SPACESHOOTER3D_DIKEYBOARD_H

// Library Includes
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// Local Includes
#include "directinputcontroller.h"

// Types

// Constants
const int ki_KEYBOARD_BUFFER_SIZE = 256;

// Prototypes
#define IFKEYAPRESSEDTHENB(a, b) if(m_cKeyboardStatus[a] & 0x80) pMessageProcessor->SendMessage(b)

class CDIKeyboard : public CDirectInputController
{
   //Member Functions
public:
	CDIKeyboard();
	virtual ~CDIKeyboard();

	virtual bool Initialise();
	virtual void Process(float _fDeltaTick);

protected:

private:
	
	void ProcessKeyboardStatus();

   //Member Variables
public:

protected:

private:
	char* m_cKeyboardStatus;

};

#endif // SPACESHOOTER3D_DIKEYBOARD_H



