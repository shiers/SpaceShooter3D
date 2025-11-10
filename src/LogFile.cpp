#include<iostream>

// Local Includes

// This Includes
#include "../include/LogFile.h"

// Static Variables

// Static Function Prototypes

// Implementation

CLogFile::CLogFile()
{
	// Constructor
}

CLogFile::~CLogFile()
{
	// Destructor
	if (!m_filestream.is_open())
	{
		m_filestream.close();
	}
}

/**
*
* This function Initialises the LogFile.
*
* @param _pcLogFilename is a pointer to const Int8 containing the log filename.
* @return Returns true if successful.
*
*/
bool
CLogFile::Initialise(const Int8 *_pcLogFilename)
{
	bool bInitialised = true;

	if (m_filestream.is_open())
	{
		m_filestream.close();
	}

	m_filestream.open(_pcLogFilename, std::ios::out);

	if (!m_filestream.is_open())
	{
		bInitialised = false;
	}

	return (bInitialised);
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
CLogFile::Write(const Int8 *_pcMessage)
{
	if (m_filestream.is_open())
	{
		m_filestream << _pcMessage;
	} else
	{
		std::cout << "Unable to write to file." << std::endl;
	}
}



