#pragma once

#ifndef __DIRECTIONALLIGHT_H__
#define __DIRECTIONALLIGHT_H__

// Library Includes

// Local Includes

// Types

// Constants

// Prototypes

class CDirectionalLight
{
	// Member Functions
public:
	CDirectionalLight();
	~CDirectionalLight();

	bool Initialise(CD3DRenderer* _pRenderer);

	void Process(Float32 _fDeltaTick);

protected:
	CD3DRenderer* m_pRenderer;

	D3DLIGHT9 m_Light;

private:

	// Member Variables
public:

protected:

private:

};
#endif // __DIRECTIONALLIGHT_H__



