#pragma once

#ifndef SPACESHOOTER3D_LOGMANAGER_H
#define SPACESHOOTER3D_LOGMANAGER_H

// Library Includes
#include<vector>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
class ILogTarget;

class CLogManager
{
	// Member Functions
public:
	CLogManager();
	~CLogManager();

	bool AddLogTarget(ILogTarget* _pLogTarget);
	bool RemoveLogTarget(ILogTarget* _pLogTarget);
	void Write(const Int8* _pcMessage);
	void WriteLine(const Int8* _pcMessage);

protected:

private:

	// Member Variables
public:

protected:

private:
	std::vector<ILogTarget*> m_LogTarget;
};
#endif // SPACESHOOTER3D_LOGMANAGER_H



