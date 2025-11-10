#pragma once

#ifndef SPACESHOOTER3D_INIPARSER_H
#define SPACESHOOTER3D_INIPARSER_H

//   Library Includes
#include <iostream>
#include <map>
#include <fstream>

//   Local Includes
#include "defines.h"

//   Types

//   Constants

//   Prototypes

class CINIParser
{
	// Member Functions
public:
	CINIParser();
	~CINIParser();

	// Load an INI file.
	// Populate the map with keys and values using
	// the add value function.
	bool LoadIniFile(const Int8* _pcFilename);

	// Save the INI file.
	bool SaveIniFile();

	// Adds a value to the map.
	// Combines the _pcSection, with _pcKey to create a
	// key for the map.
	bool AddValue(const Int8* _pcSection, const Int8* _pcKey,
				  const Int8* _pcValue);

	// GetStringValue
	// Returns true if the value was found.
	// _rStrValue will be populated with the correct data if
	// the key is found in the map.
	bool GetStringValue(const Int8* _pcSection, const Int8* _pcKey,
						std::string& _rStrValue);

	// GetIntValue
	// Returns true if the value was found.
	// _riValue will be populated with the correct data if
	// the key is found in the map.
	bool GetIntValue(const Int8* _pcSection, const Int8* _pcKey,
					 Int32& _riValue);


	bool GetIntValue(const Int8* _pcSection, const Int8* _pcKey,
					 UInt8& _riValue);

	bool GetIntValue(const Int8* _pcSection, const Int8* _pcKey,
					 UInt16& _riValue);
	
	// GetFloatValue
	// Returns true if the value was found.
	// _rfValue will be populated with the correct data if
	// the key is found in the map.
	bool GetFloatValue(const Int8* _pcSection, const Int8* _pcKey,
					   Float32& _rfValue);

	// GetBoolValue
	// Returns true if the value was found.
	// _rbValue will be populated with the correct data if
	// the key is found in the map.
	bool GetBoolValue(const Int8* _pcSection, const Int8* _pcKey,
					  bool& _rbValue);

protected:

private:

	// Member Variables
public:

protected:

private:
	// Create a map member variable to store the ini file.
	std::map<std::string, std::string> m_mapPairs;

	// Store the filename.
	std::string m_strFilename;

	// Store the filestream.
	std::fstream m_filestream;
};
#endif // SPACESHOOTER3D_INIPARSER_H



