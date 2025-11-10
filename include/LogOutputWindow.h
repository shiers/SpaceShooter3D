#pragma once

#ifndef SPACESHOOTER3D_LOGOUTPUTWINDOW_H
#define SPACESHOOTER3D_LOGOUTPUTWINDOW_H

// Library Includes

// Local Includes
#include "defines.h"
#include "LogTarget.h"
#include "LogManager.h"

// Types

// Constants

// Prototypes

class CLogOutputWindow : public ILogTarget
{
	// Member Functions
public:
	CLogOutputWindow();
	~CLogOutputWindow();
	
	void Write(const Int8* _pcMessage);

protected:

private:

	// Member Variables
public:

protected:

private:

};
#endif // SPACESHOOTER3D_LOGOUTPUTWINDOW_H



