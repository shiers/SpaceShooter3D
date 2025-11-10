#pragma once

#ifndef SPACESHOOTER3D_LOGFILE_H
#define SPACESHOOTER3D_LOGFILE_H

// Library Includes
#include<iostream>
#include <fstream>

// Local Includes
#include "defines.h"
#include "LogTarget.h"

// Types

// Constants

// Prototypes

class CLogFile : public ILogTarget
{
	// Member Functions
public:
	CLogFile();
	virtual ~CLogFile();

	bool Initialise(const Int8* _pcLogFilename);
	void Write(const Int8* _pcMessage);

protected:

private:

	// Member Variables
public:

protected:

private:
	std::fstream m_filestream;
};
#endif // SPACESHOOTER3D_LOGFILE_H



