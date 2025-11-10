#include <iostream>

// Local Includes
#include "../include/LogTarget.h"
#include "../include/defines.h"

// This Includes
#include "../include/LogManager.h"

// Static Variables

// Static Function Prototypes

// Implementation

CLogManager::CLogManager()
{
	// Constructor
}

CLogManager::~CLogManager()
{
	// Destructor
	for (UInt32 i = 0; i < m_LogTarget.size(); ++i)
	{
		delete m_LogTarget[i];
	}
	m_LogTarget.clear();
}

/**
*
* This function adds a log target.
*
* @param _pLogTarget is a log target.
* @return Returns true if successful.
*
*/
bool
CLogManager::AddLogTarget(ILogTarget *_pLogTarget)
{
	m_LogTarget.push_back(_pLogTarget);

	return (true);
}

/**
*
* This function removes a log target.
*
* @param _pLogTarget is a log target.
* @return Returns true if successful.
*
*/
bool
CLogManager::RemoveLogTarget(ILogTarget *_pLogTarget)
{
	std::vector<ILogTarget *>::iterator currentPosition = m_LogTarget.begin();

	while (*currentPosition != _pLogTarget && currentPosition < m_LogTarget.end())
	{
		++currentPosition;
	}

	m_LogTarget.erase(currentPosition);

	return (true);
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
CLogManager::Write(const Int8 *_pcMessage)
{
	for (UInt32 i = 0; i < m_LogTarget.size(); ++i)
	{
		m_LogTarget[i]->Write(_pcMessage);
	}
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
CLogManager::WriteLine(const Int8 *_pcMessage)
{
	for (UInt32 i = 0; i < m_LogTarget.size(); ++i)
	{
		m_LogTarget[i]->Write(_pcMessage);
		m_LogTarget[i]->Write("\n");
	}
}



