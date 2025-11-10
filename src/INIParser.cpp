#include <iostream>
#include <fstream>
#include <string>

//   Local Includes

//   This Includes
#include "../include/INIParser.h"

//   Static Variables

//   Static Function Prototypes

//   Implementation
CINIParser::CINIParser()
{
}

CINIParser::~CINIParser()
{
}

/**
*
* This function loads the .ini file.
*
* @param _pcFilename is the name of the name of the file being loaded.
* @Returns true if successful.
*
*/
bool
CINIParser::LoadIniFile(const Int8 *_pcFilename)
{
	std::string strLine = ""; // Stores the whole current line.
	std::string strKey = ""; // Stores the current key.
	std::string strValue = ""; // Store the value of the current string.
	std::string strSection = ""; // Stores the current Section.

	std::string strItem;

	m_filestream.open(_pcFilename, std::ios::in);

	if (m_filestream.is_open())
	{
		while (!m_filestream.eof())
		{
			getline(m_filestream, strLine);
			if (strLine[0] == ';')
			{
				continue;
			} else if (strLine[0] == '[')
			{
				strSection = ""; // Initialise the current Section.
				for (UInt32 i = 1; i < strLine.length(); ++i)
				{
					if (strLine[i] == ']')
					{
						for (UInt32 j = 1; j < i; ++j)
						{
							strSection = strSection + strLine[j];
						}
					}
				}
			} else if (strLine[0] != ' ')
			{
				for (UInt32 i = 0; i < strLine.length();)
				{
					strKey = ""; // Initialise the new Key
					strValue = ""; // Initialise the new Value.

					// '9' is the ASCII code for TAB.
					while (strLine[i] != ' ' && strLine[i] != 9
					       && strLine[i] != '=')
					{
						strKey = strKey + strLine[i];
						++i;
					}

					while (strLine[i] == ' ' || strLine[i] == 9
					       || strLine[i] == '=')
					{
						++i;
					}

					while (strLine.length() > i && strLine[i] != ' '
					       && strLine[i] != 9)
					{
						strValue = strValue + strLine[i];
						++i;
					}
					AddValue(strSection.c_str(), strKey.c_str(),
					         strValue.c_str());
				}
			}
		}
	}

	m_filestream.close();

	if (!m_filestream.good())
	{
		m_filestream.clear();
	}

	return (true);
}

/**
*
* This function saves the .ini file.
*
* @param _pcFilename is the name of the name of the file being loaded.
* @Returns true if successful.
*
*/
bool
CINIParser::SaveIniFile()
{
	bool bSaveSuccess = true;

	m_filestream.open(m_strFilename.c_str(), std::ios::out);

	if (m_filestream.is_open() == false)
	{
		bSaveSuccess = false;
	} else
	{
		std::map<std::string, std::string>::iterator
				currentItem = m_mapPairs.begin();
		std::string strSection = "";
		std::string strOldSection = "";
		std::string strKey = "";

		for (; currentItem != m_mapPairs.end(); ++currentItem)
		{
			Int32 iSplicePoint = 0;
			std::string strTemp = "";
			strTemp = currentItem->first;

			iSplicePoint = static_cast<Int32>(strTemp.find_first_of('|'));
			strSection = strTemp.substr(0, iSplicePoint);
			strKey = strTemp.substr(iSplicePoint + 1, strTemp.length());

			// This line is just to see if the section
			// and key have been seperated properly.
			std::cout << strSection << " " << strKey << std::endl;

			if (strSection != strOldSection)
			{
				m_filestream << "\n[" << strSection << "]" << std::endl;
				strOldSection = strSection;
			}
			m_filestream << strKey << " = " << currentItem->second << std::endl;
		}
	}

	m_filestream.close();

	if (!m_filestream.good())
	{
		m_filestream.clear();
	}

	return (bSaveSuccess);
}

/**
*
* This function adds data to .ini file.
*
* @param _pcSection.
* @param _pcKey.
* @param _pcValue.
* @Returns true if successful.
*
*/
bool
CINIParser::AddValue(const Int8 *_pcSection, const Int8 *_pcKey,
                     const Int8 *_pcValue)
{
	std::string strSection = _pcSection;
	std::string strKey = _pcKey;
	std::string strMapKey = strSection + "|" + strKey;
	m_mapPairs[strMapKey] = _pcValue;
	return (true);
}

/**
*
* This function gets a string value.
*
* @param _pcSection.
* @param _pcKey.
* @param _strValue.
* @Returns true if successful.
*
*/
bool
CINIParser::GetStringValue(const Int8 *_pcSection, const Int8 *_pcKey,
                           std::string &_rStrValue)
{
	std::string strSection = _pcSection;
	std::string strKey = _pcKey;
	std::string strMapKey = strSection + "|" + strKey;

	bool bMapFound = false;

	if (m_mapPairs[strMapKey] != "")
	{
		_rStrValue = m_mapPairs[strMapKey];
		bMapFound = true;
	}

	return (bMapFound);
}

/**
*
* This function gets an Int32 value.
*
* @param _pcSection.
* @param _pcKey.
* @param _riValue
* @Returns true if successful.
*
*/
bool
CINIParser::GetIntValue(const Int8 *_pcSection, const Int8 *_pcKey,
                        Int32 &_riValue)
{
	std::string strSection = _pcSection;
	std::string strKey = _pcKey;
	std::string strMapKey = strSection + "|" + strKey;

	bool bMapFound = false;

	if (m_mapPairs[strMapKey] != "")
	{
		_riValue = atoi(m_mapPairs[strMapKey].c_str());
		bMapFound = true;
	}

	return (bMapFound);
}

/**
*
* This function gets an Int8 value.
*
* @param _pcSection.
* @param _pcKey.
* @param _riValue
* @Returns true if successful.
*
*/
bool
CINIParser::GetIntValue(const Int8 *_pcSection, const Int8 *_pcKey,
                        UInt8 &_riValue)
{
	std::string strSection = _pcSection;
	std::string strKey = _pcKey;
	std::string strMapKey = strSection + "|" + strKey;

	bool bMapFound = false;

	if (m_mapPairs[strMapKey] != "")
	{
		_riValue = static_cast<UInt8>(atoi(m_mapPairs[strMapKey].c_str()));
		bMapFound = true;
	}

	return (bMapFound);
}

/**
*
* This function gets an Int16 value.
*
* @param _pcSection.
* @param _pcKey.
* @param _riValue
* @Returns true if successful.
*
*/
bool
CINIParser::GetIntValue(const Int8 *_pcSection, const Int8 *_pcKey,
                        UInt16 &_riValue)
{
	std::string strSection = _pcSection;
	std::string strKey = _pcKey;
	std::string strMapKey = strSection + "|" + strKey;

	bool bMapFound = false;

	if (m_mapPairs[strMapKey] != "")
	{
		_riValue = static_cast<UInt16>(atoi(m_mapPairs[strMapKey].c_str()));
		bMapFound = true;
	}

	return (bMapFound);
}

/**
*
* This function gets a Float32 value.
*
* @param _pcSection.
* @param _pcKey.
* @param _rfValue
* @Returns true if successful.
*
*/
bool
CINIParser::GetFloatValue(const Int8 *_pcSection, const Int8 *_pcKey,
                          Float32 &_rfValue)
{
	std::string strSection = _pcSection;
	std::string strKey = _pcKey;
	std::string strMapKey = strSection + "|" + strKey;

	bool bMapFound = false;

	if (m_mapPairs[strMapKey] != "")
	{
		_rfValue = static_cast<Float32>(atof(m_mapPairs[strMapKey].c_str()));
		bMapFound = true;
	}

	return (bMapFound);
}

/**
*
* This function gets a bool. value.
*
* @param _pcSection.
* @param _pcKey.
* @param _pcValue.
* @param _rbValue
* @Returns true if successful.
*
*/
bool
CINIParser::GetBoolValue(const Int8 *_pcSection, const Int8 *_pcKey,
                         bool &_rbValue)
{
	std::string strSection = _pcSection;
	std::string strKey = _pcKey;
	std::string strMapKey = strSection + "|" + strKey;

	bool bMapFound = false;

	if (m_mapPairs[strMapKey] != "")
	{
		if (m_mapPairs[strMapKey] == "true")
		{
			_rbValue = true;
		} else if (m_mapPairs[strMapKey] == "1")
		{
			_rbValue = 1;
		} else if (m_mapPairs[strMapKey] == "false")
		{
			_rbValue = false;
		} else if (m_mapPairs[strMapKey] == "0")
		{
			_rbValue = 0;
		}

		bMapFound = true;
	}
	return (bMapFound);
}



